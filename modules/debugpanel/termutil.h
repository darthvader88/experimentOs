/* SPDX-License-Identifier: BSD-2-Clause */

#include <tilck/common/basic_defs.h>
#include <tilck/common/string_util.h>
#include "dp_int.h"

#define ERASE_DISPLAY            "\033[2J"
#define REVERSE_VIDEO            "\033[7m"
#define ESC_COLOR_WHITE          "\033[37m"
#define ESC_COLOR_BRIGHT_WHITE   "\033[97m"
#define ESC_COLOR_BRIGHT_RED     "\033[91m"
#define ESC_COLOR_GREEN          "\033[32m"
#define ESC_COLOR_YELLOW         "\033[93m"
#define RESET_ATTRS              "\033[0m"
#define GFX_ON                   "\033(0"
#define GFX_OFF                  "\033(B"
#define HIDE_CURSOR              "\033[?25l"
#define SHOW_CURSOR              "\033[?25h"
#define USE_ALT_BUF              "\033[?1049h"
#define USE_DEF_BUF              "\033[?1049l"

#define TERM_VLINE               GFX_ON "x" GFX_OFF
#define DP_COLOR                 DEFAULT_FG_COLOR
#define DP_ESC_COLOR             ESC_COLOR_WHITE

void dp_write(int row, int col, const char *fmt, ...);
void dp_draw_rect(const char *label, int row, int col, int h, int w);

/* WARNING: dirty macro expecting a local `row` variable to be defined */
#define dp_writeln(...)  dp_write(row++, 0, __VA_ARGS__)

/* WARNING: dirty macro expecting both `row` and `col` to be defined */
#define dp_writeln2(...) dp_write(row++, col, __VA_ARGS__)

void dp_move_right(int n);
void dp_move_left(int n);
void dp_move_to_col(int n);
void dp_clear(void);
void dp_move_cursor(int row, int col);
void dp_set_cursor_enabled(bool enabled);
void dp_switch_to_alt_buffer(void);
void dp_switch_to_default_buffer(void);

static inline const char *
dp_sign_value_esc_color(sptr val)
{
   return val > 0
            ? ESC_COLOR_GREEN
            : val < 0 ? ESC_COLOR_BRIGHT_RED : DP_ESC_COLOR;
}
