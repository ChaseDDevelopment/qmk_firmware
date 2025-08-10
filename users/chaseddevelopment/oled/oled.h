#pragma once

#include QMK_KEYBOARD_H

// Public API for reusable OLED rendering across keyboards

// Initialize rotation; call from keymap's oled_init_user
oled_rotation_t chased_oled_init(oled_rotation_t rotation);

// Render tasks; call from keymap's oled_task_user depending on master/slave
void chased_oled_task_master(void);
void chased_oled_task_slave(void);

// Optional notifications from keymap for pet/game behavior
void chased_oled_on_ctrl(bool pressed);
void chased_oled_on_space(bool pressed);

// Optional: allow keymap to override game layer detection
__attribute__((weak)) bool chased_is_game_layer_active(void);

// Inline helpers for easy integration in keymaps
static inline oled_rotation_t oled_init_user_wrapped(oled_rotation_t rotation) {
    return chased_oled_init(rotation);
}

static inline bool oled_task_user_wrapped(void) {
    if (is_keyboard_master()) {
        chased_oled_task_master();
    } else {
        chased_oled_task_slave();
    }
    return false;
}


