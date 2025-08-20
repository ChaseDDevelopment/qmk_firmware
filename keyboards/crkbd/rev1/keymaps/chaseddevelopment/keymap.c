/* Copyright 2021 Dane Evans
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// CRKBD (Corne) - Adapted from Sofle RGB
#include <stdio.h>

#include QMK_KEYBOARD_H

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

// RGB Matrix settings for per-key lighting
#ifdef RGB_MATRIX_ENABLE
#define RGB_MATRIX_INDICATOR_BRIGHTNESS 30

// Helper macros for RGB Matrix (adapted from RGBLIGHT patterns)
#define SET_RGB_INDICATORS(r, g, b) \
    {0, 1, r, g, b}, \
    {21, 1, r, g, b}

#define SET_RGB_OUTER_COL(r, g, b) \
    {5, 1, r, g, b}, {11, 1, r, g, b}, {17, 1, r, g, b}, \
    {26, 1, r, g, b}, {32, 1, r, g, b}, {38, 1, r, g, b}

#define SET_RGB_INNER_COL(r, g, b) \
    {4, 1, r, g, b}, {10, 1, r, g, b}, {16, 1, r, g, b}, \
    {25, 1, r, g, b}, {31, 1, r, g, b}, {37, 1, r, g, b}

#define SET_RGB_THUMB_CLUSTER(r, g, b) \
    {18, 3, r, g, b}, \
    {39, 3, r, g, b}

#define SET_RGB_NUMBER_ROW(r, g, b) \
    {0, 6, r, g, b}, \
    {21, 6, r, g, b}
#endif

enum crkbd_layers {
    _DEFAULTS = 0,
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _GAMING,
    _QWERTY_MAC,
    _LOWER_MAC,
    _RAISE_MAC,
    _ADJUST
};

enum os_modes {
    OS_WINDOWS_LINUX,
    OS_MACOS
};

uint8_t current_os = OS_WINDOWS_LINUX;

enum custom_keycodes {
    KC_LOWER = SAFE_RANGE,
    KC_RAISE,
    KC_ADJUST,
    KC_D_MUTE,
    KC_GRV_INV,
    OS_TOG
};

// Tap-Hold keys
#define CTL_ESC MT(MOD_LCTL, KC_ESC)  // Tap for Esc, Hold for Ctrl (Windows/Linux)
#define OPT_ESC MT(MOD_LALT, KC_ESC)  // Tap for Esc, Hold for Option/Alt (macOS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY (Base Layer) - Corne Layout
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |                    |   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | LGUI | LOWER| Enter|      | Space| RAISE| RAlt |
 *                                     `--------------------'      `--------------------'
 */
[_QWERTY] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  CTL_ESC,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                     KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                              KC_LCTL, KC_LOWER, KC_ENT,      KC_SPC, KC_RAISE, KC_RCTL
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Ctrl |  F1  |  F2  |  F3  |  F4  |  F5  |                    | Left | Down |  Up  |Right | Home | End  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Shift|  F6  |  F7  |  F8  |  F9  | F10  |                    |  {   |  }   |  [   |  ]   | PgUp | PgDn |
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | LGUI | LOWER| Enter|      | Space| RAISE| RAlt |
 *                                     `--------------------'      `--------------------'
 */
[_LOWER] = LAYOUT_split_3x6_3(
  KC_GRV_INV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_F12,
  KC_LCTL,  KC_F1,  KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_END,
  KC_LSFT,  KC_F6,  KC_F7,   KC_F8,   KC_F9,   KC_F10,                    KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_PGUP, KC_PGDN,
                              KC_LALT, _______, KC_ENT,       KC_SPC, _______, KC_RGUI
),
/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Tab  |  !   |  @   |  #   |  $   |  %   |                    |  ^   |  &   |  *   |  (   |  )   | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Ctrl | Ins  |Pscr  | Menu |      | Caps |                    |  =   |  -   |  \   |  |   |  `   |  ~   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Shift|Undo  | Cut  | Copy |Paste |      |                    |  _   |  +   |  {   |  }   | Del  |GAMING|
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | LGUI | LOWER| Enter|      | Space| RAISE| RAlt |
 *                                     `--------------------'      `--------------------'
 */
[_RAISE] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  KC_LCTL,  KC_INS,  KC_PSCR, KC_APP,  C(KC_A), KC_CAPS,                   KC_EQL,  KC_MINS, KC_BSLS, KC_PIPE, KC_HOME, KC_END,
  KC_LSFT,  C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX,                   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_DEL,  TG(_GAMING),
                              KC_LALT, _______, KC_ENT,       KC_SPC, _______, KC_RGUI
),

/* GAMING
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |                    |   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | Alt  | Space| Enter|      | Space| RAISE|GAMING|
 *                                     `--------------------'      `--------------------'
 */
[_GAMING] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                     KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                              KC_LALT, KC_SPC,  KC_ENT,      KC_SPC, KC_RAISE, TG(_GAMING)
),

/* QWERTY_MAC - macOS layout
 * Same as QWERTY but with Option/Alt on Esc key and Ctrl on right thumb
 */
[_QWERTY_MAC] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  OPT_ESC,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                     KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                              KC_LGUI, KC_LOWER, KC_ENT,      KC_SPC, KC_RAISE, KC_RCTL
),

/* LOWER_MAC - macOS lower layer */
[_LOWER_MAC] = LAYOUT_split_3x6_3(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_F12,
  KC_LALT,  KC_F1,  KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_END,
  KC_LSFT,  KC_F6,  KC_F7,   KC_F8,   KC_F9,   KC_F10,                    KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_PGUP, KC_PGDN,
                              KC_LGUI, _______, KC_ENT,       KC_SPC, _______, KC_RCTL
),

/* RAISE_MAC - macOS raise layer with Cmd-based shortcuts */
[_RAISE_MAC] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  KC_LALT,  KC_INS,  KC_PSCR, KC_APP,  G(KC_A), KC_CAPS,                   KC_EQL,  KC_MINS, KC_BSLS, KC_PIPE, KC_GRV,  KC_TILD,
  KC_LSFT,  G(KC_Z), G(KC_X), G(KC_C), G(KC_V), C(S(KC_C)),                KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_DEL,  C(S(KC_V)),
                              KC_LGUI, _______, KC_ENT,       KC_SPC, _______, KC_RCTL
),

/* ADJUST - Settings layer (LOWER + RAISE) */
[_ADJUST] = LAYOUT_split_3x6_3(
  QK_BOOT,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  RGB_TOG,  RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, RGB_MOD,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OS_TOG,
  RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                              _______, _______, _______,       _______, _______, _______
)
};

#ifdef RGB_MATRIX_ENABLE
// RGB Matrix Layer Lighting - Adapted from RGBLIGHT patterns
const uint8_t PROGMEM rgb_matrix_layer_colors[][3] = {
    [_QWERTY] = {255, 0, 0},      // Red
    [_LOWER]  = {0, 255, 255},    // Cyan
    [_RAISE]  = {0, 0, 255},      // Blue  
    [_GAMING] = {255, 0, 255}     // Purple
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0 && layer < sizeof(rgb_matrix_layer_colors) / sizeof(rgb_matrix_layer_colors[0])) {
        uint8_t r = rgb_matrix_layer_colors[layer][0];
        uint8_t g = rgb_matrix_layer_colors[layer][1]; 
        uint8_t b = rgb_matrix_layer_colors[layer][2];
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, r, g, b);
        }
    } else {
        // Base layer: solid cyan/blue
        HSV hsv = { .h = 175, .s = 255, .v = 120 };
        RGB rgb = hsv_to_rgb(hsv);
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return false;
}

#ifdef RGB_MATRIX_ENABLE
void keyboard_post_init_user(void) {
    // Force solid color on boot, overriding any stored gradient in EEPROM
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(175, 255, 120);
}
#endif
#endif

#ifdef OLED_ENABLE
#include "users/chaseddevelopment/oled/oled.h"

// Use shared userspace OLED handlers
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return chased_oled_init(rotation);
}

bool oled_task_user(void) {
    return oled_task_user_wrapped();
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Handle ADJUST layer activation
    static bool lower_pressed = false;
    static bool raise_pressed = false;
    
    switch (keycode) {
        case KC_LOWER:
            if (record->event.pressed) {
                lower_pressed = true;
                if (current_os == OS_MACOS) {
                    layer_on(_LOWER_MAC);
                } else {
                    layer_on(_LOWER);
                }
                if (raise_pressed) {
                    layer_on(_ADJUST);
                }
            } else {
                lower_pressed = false;
                layer_off(_LOWER);
                layer_off(_LOWER_MAC);
                layer_off(_ADJUST);
            }
            return false;
            
        case KC_RAISE:
            if (record->event.pressed) {
                raise_pressed = true;
                if (current_os == OS_MACOS) {
                    layer_on(_RAISE_MAC);
                } else {
                    layer_on(_RAISE);
                }
                if (lower_pressed) {
                    layer_on(_ADJUST);
                }
            } else {
                raise_pressed = false;
                layer_off(_RAISE);
                layer_off(_RAISE_MAC);
                layer_off(_ADJUST);
            }
            return false;
            
        case OS_TOG:
            if (record->event.pressed) {
                current_os = (current_os == OS_WINDOWS_LINUX) ? OS_MACOS : OS_WINDOWS_LINUX;
                // Clear all layers and set base layer
                layer_clear();
                if (current_os == OS_MACOS) {
                    layer_on(_QWERTY_MAC);
                } else {
                    layer_on(_QWERTY);
                }
            }
            return false;
            
        case KC_D_MUTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_MEH));
                register_code(KC_UP);
            } else {
                unregister_mods(mod_config(MOD_MEH));
                unregister_code(KC_UP);
            }
            break;
        case KC_GRV_INV:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    del_mods(MOD_MASK_SHIFT);
                    tap_code16(KC_GRV); // produce backtick when Shift held
                    add_mods(MOD_MASK_SHIFT);
                } else {
                    tap_code16(S(KC_GRV)); // produce tilde when no Shift
                }
            }
            return false;
        case KC_LCTL:
        case KC_RCTL:
            chased_oled_on_ctrl(record->event.pressed);
            break;
        case KC_SPC:
            chased_oled_on_space(record->event.pressed);
            break;
    }
    
    // Copy/paste OLED feedback removed; handled by userspace if desired
    
    return true;
}