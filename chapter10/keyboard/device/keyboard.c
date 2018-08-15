# include "keyboard.h"
# include "kernel/print.h"
# include "io.h"
# include "interrupt.h"
# include "global.h"

# define KEYBOARD_BUF_PORT 0x60

static void init_keyboard_handler(void) {
    uint8_t code = inb(KEYBOARD_BUF_PORT);
    put_int(code);
}

void keyboard_init(void) {
    put_str("Keyboard init start...\n");
    register_handler(0x21, init_keyboard_handler);
    put_str("Keyboard init done.\n");
}