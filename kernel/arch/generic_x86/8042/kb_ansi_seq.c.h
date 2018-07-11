

static const struct {

   u32 key;
   char seq[8];

} ansi_sequences[] = {

   {KEY_UP,           "\033[A"},
   {KEY_DOWN,         "\033[B"},
   {KEY_RIGHT,        "\033[C"},
   {KEY_LEFT,         "\033[D"},

   {KEY_NUMPAD_UP,    "\033[A"},
   {KEY_NUMPAD_DOWN,  "\033[B"},
   {KEY_NUMPAD_RIGHT, "\033[C"},
   {KEY_NUMPAD_LEFT,  "\033[D"},

   {KEY_PAGE_UP,      "\033[5~"},
   {KEY_PAGE_DOWN,    "\033[6~"},

   {KEY_INS,          "\033[2~"},
   {KEY_DEL,          "\033[3~"},
   {KEY_HOME,         "\033[H"},
   {KEY_END,          "\033[F"},

   {KEY_F1,           "\033OP"},
   {KEY_F2,           "\033OQ"},
   {KEY_F3,           "\033OR"},
   {KEY_F4,           "\033OS"},
   {KEY_F5,           "\033[15~"},
   {KEY_F6,           "\033[17~"},
   {KEY_F7,           "\033[18~"},
   {KEY_F8,           "\033[19~"},
   {KEY_F9,           "\033[20~"},
   {KEY_F10,          "\033[21~"},
   {KEY_F11,          "\033[23~"},
   {KEY_F12,          "\033[24~"}
};

bool kb_scancode_to_ansi_seq(u32 key, u32 modifiers, char *seq)
{
   const char *base_seq = NULL;
   char *p;
   u32 sl;

   ASSERT(modifiers < 8);

   for (u32 i = 0; i < ARRAY_SIZE(ansi_sequences); i++)
      if (ansi_sequences[i].key == key) {
         base_seq = ansi_sequences[i].seq;
         break;
      }

   if (!base_seq)
      return false;

   sl = strlen(base_seq);
   memcpy(seq, base_seq, sl + 1);

   if (!modifiers)
      return true;

   if (seq[1] != '[') {

      if (seq[1] == 'O') {

         /*
          * The sequence is like F1: \033OP. Before appending the modifiers,
          * we need to replace 'O' with [1.
          */

         char end_char = seq[2];

         seq[1] = '[';
         seq[2] = '1';
         seq[3] = end_char;
         seq[4] = 0;

         sl++; /* we increased the size by 1 */

      } else {

         /* Don't know how to deal with such a sequence */
         return false;
      }

   } else if (!isdigit(seq[2])) {

      /*
       * Here the seq starts with ESC [, but a non-digit follows '['.
       * Example: the home KEY: \033[H
       * In this case, we need to insert a "1;" between '[' and H.
       */

      if (sl != 3) {
         /* Don't support UNKNOWN cases like \033[XY: they should not exist */
         return false;
      }

      char end_char = seq[2];
      seq[2] = '1';
      seq[3] = end_char;
      seq[4] = 0;
      sl++;

      /* Now we have seq like \033[1H: the code below will complete the job */
   }

   /*
    * If the seq is like ESC [ <something> <end char>, then we can just
    * insert after "; <modifiers+1>" between <something> and <end char>.
    */

   char end_char = seq[sl - 1];

   p = &seq[sl - 1];
   *p++ = ';';
   *p++ = '1' + modifiers;
   *p++ = end_char;
   *p = 0;

   return true;
}