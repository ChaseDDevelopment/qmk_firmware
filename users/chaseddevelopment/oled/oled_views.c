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
void chased_render_caps_dot_if_any(uint8_t line);
void chased_render_gamepad_badge_small(void);

/* view swap state removed to prevent flashing */

__attribute__((weak)) bool chased_is_game_layer_active(void) {
    return false;
}

oled_rotation_t chased_oled_init(oled_rotation_t rotation) {
    (void)rotation;
    return OLED_ROTATION_270;
}

static void render_master_layer_view(void) {
    oled_set_cursor(0, 0);
    chased_render_layer_block();
    oled_set_cursor(0, CHASED_OLED_ROWS - 1);
    chased_render_caps_dot_if_any(CHASED_OLED_ROWS - 1);
}

static void render_master_mods_view(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    // Move mods further down to avoid overlap with the three-line layer block (OLED_ROTATION_270)
    oled_set_cursor(0, 7);
    chased_render_mods(mods);
    oled_set_cursor(0, CHASED_OLED_ROWS - 1);
    chased_render_caps_dot_if_any(CHASED_OLED_ROWS - 1);
}

void chased_oled_task_master(void) {
    // Render both layer indicator and modifiers without view swapping to avoid flashing
    render_master_layer_view();
    render_master_mods_view();
}

void chased_oled_task_slave(void) {
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
}

static bool s_pet_sneak = false;
static bool s_pet_jump  = false;

void chased_oled_on_ctrl(bool pressed) { s_pet_sneak = pressed; }
void chased_oled_on_space(bool pressed) { s_pet_jump = pressed; }

bool chased_pet_is_sneaking(void) { return s_pet_sneak; }
bool chased_pet_is_jumping(void)  { return s_pet_jump; }


