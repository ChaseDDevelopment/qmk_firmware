#include "oled.h"

#if defined(OLED_DISPLAY_128X32)
#    define CHASED_OLED_ROWS 4
#else
#    define CHASED_OLED_ROWS 8
#endif

#ifndef CHASED_OLED_VIEW_SWAP_INTERVAL_MS
#    define CHASED_OLED_VIEW_SWAP_INTERVAL_MS 2500
#endif

#ifndef CHASED_OLED_PET_ON_SLAVE
#    define CHASED_OLED_PET_ON_SLAVE 1
#endif

// Forward decls for sprites/helpers
void chased_render_layer_block(void);
void chased_render_mods(uint8_t modifiers);
void chased_render_luna(uint8_t x, uint8_t y);
void chased_update_luna_anim(void);
void chased_render_wpm_bar_line(uint8_t line, uint8_t start_col, uint8_t width);
void chased_render_gamepad_badge_small(void);
void render_mod_status_gui_alt_row1(uint8_t modifiers);
void render_mod_status_ctrl_shift_row1(uint8_t modifiers);
void render_mod_status_gui_alt(uint8_t modifiers);
void render_mod_status_ctrl_shift(uint8_t modifiers);
#ifdef CHASED_OLED_RIGHT_SPACESHIP
void chased_render_spaceship_wpm_fullscreen(void);
#endif

/* view swap state removed to prevent flashing */

__attribute__((weak)) bool chased_is_game_layer_active(void) {
    return false;
}

oled_rotation_t chased_oled_init(oled_rotation_t rotation) {
    (void)rotation;
    return OLED_ROTATION_270;
}

static void render_master_compose(void) {
    // Luna at top (y=1 for slight offset if needed)
    chased_render_luna(0, 1);

    // Layer shift graphics at middle (y=5, below Luna)
    oled_set_cursor(0, 5);
    chased_render_layer_block();

    // Mod glyphs at bottom (y=9, after layer block)
    const uint8_t status_col = 0; // Start from left edge to avoid wrapping
    uint8_t mods = get_mods() | get_oneshot_mods();
    
    // GUI/ALT icons at row 9-10
    oled_set_cursor(status_col, 9);
    render_mod_status_gui_alt(mods);
    
    // CTRL/SHIFT icons at row 11-12
    oled_set_cursor(status_col, 11);
    render_mod_status_ctrl_shift(mods);
}

void chased_oled_task_master(void) {
    // Honor OLED timeout on master explicitly, and sync RGB Matrix state
    #if OLED_TIMEOUT > 0
    if (last_input_activity_elapsed() > OLED_TIMEOUT && last_led_activity_elapsed() > OLED_TIMEOUT) {
        oled_off();
        return;
    } else {
        oled_on();
    }
    #endif
    // Compose Luna on the left, status (layers + condensed mods) on the right
    render_master_compose();
    chased_update_luna_anim();
}

void chased_oled_task_slave(void) {
    // Honor OLED timeout on slave explicitly, and sync RGB Matrix state
    #if OLED_TIMEOUT > 0
    if (last_input_activity_elapsed() > OLED_TIMEOUT && last_led_activity_elapsed() > OLED_TIMEOUT) {
        oled_off();
        return;
    } else {
        oled_on();
    }
    #endif
    #ifdef CHASED_OLED_RIGHT_SPACESHIP
        // Full-screen spaceship WPM visualization on the right
        chased_render_spaceship_wpm_fullscreen();
    #else
        if (CHASED_OLED_PET_ON_SLAVE) {
            oled_set_cursor(0, 0);
            chased_render_luna(0, 0);
        }
        uint8_t last_line = CHASED_OLED_ROWS - 1;
        uint8_t wpm       = (uint8_t)get_current_wpm();
        oled_set_cursor(0, last_line);
        oled_write_P(PSTR("W:"), false);
        oled_write(get_u8_str(wpm, ' '), false);
        chased_render_wpm_bar_line(last_line, 6, 24);
        chased_update_luna_anim();
    #endif
}

static bool s_pet_sneak = false;
static bool s_pet_jump  = false;

void chased_oled_on_ctrl(bool pressed) { s_pet_sneak = pressed; }
void chased_oled_on_space(bool pressed) { s_pet_jump = pressed; }

bool chased_pet_is_sneaking(void) { return s_pet_sneak; }
bool chased_pet_is_jumping(void)  { return s_pet_jump; }