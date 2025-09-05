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
};

// No multi-OS layers; single QWERTY base

// Internal guard to allow firmware to clear Caps Lock while blocking all other sources
static bool allow_caps_toggle_internal = false;
static bool caps_seen_on = false;
static bool physical_lshift_down = false;
static bool physical_rshift_down = false;
static uint32_t shift_stuck_since = 0;

enum custom_keycodes {
    KC_D_MUTE = SAFE_RANGE,
    KC_GRV_INV,
    RGB_SOFT_OFF
};

// No special OS dual-role keys

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Base (QWERTY, HRMs)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | LShf |   Z  |   X  |   C  |   V  |   B  |                    |   N  |   M  |   ,  |   .  |   /  |  Esc |
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | LGUI | Lower| Enter|      | Space| Raise| RAlt |
 *                                     `--------------------'      `--------------------'
 */
[_QWERTY] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  KC_LCTL,  MT(MOD_LGUI, KC_A), MT(MOD_LALT, KC_S), MT(MOD_LSFT, KC_D), MT(MOD_LCTL, KC_F), KC_G, 
            KC_H, MT(MOD_RCTL, KC_J), MT(MOD_RSFT, KC_K), MT(MOD_RALT, KC_L), MT(MOD_RGUI, KC_SCLN), KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                     KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
                             KC_LGUI, MO(_LOWER), KC_ENT,      KC_SPC, MO(_RAISE), KC_RALT
),

/* LOWER (Numbers & Navigation)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Tab  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    | Left | Down |  Up  | Right|      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | LShf |      |      |      |      |      |                    |      |      |      |      |      |      |
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | LGUI |      | Enter|      | Space|      | RAlt |
 *                                     `--------------------'      `--------------------'
 */
[_LOWER] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5,                     KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_BSPC,
  _______,  _______,_______,_______,_______,_______,                  KC_LEFT,KC_DOWN,KC_UP,  KC_RGHT,_______,_______,
  KC_LSFT,  _______,_______,_______,_______,_______,                  _______,_______,_______,_______,_______,_______,
                             KC_LGUI, _______, KC_ENT,      KC_SPC, _______, KC_RALT
),

/* RAISE (Symbols & RGB Controls)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  !  |  @  |  #  |  $  |  %  |                    |  ^  |  &  |  *  |  (  |  )  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Ctrl |      | rgb0|      |      |      |                    |  =  |  -  |  [  |  ]  |  \ |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | LShf | RGBT | BRI+| BRI-|      |      |                    |  _  |  +  |  {  |  }  |  |  |  ~   |
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | LGUI |  GAME| Enter|      | Space|      | RAlt |
 *                                     `--------------------'      `--------------------'
 */
[_RAISE] = LAYOUT_split_3x6_3(
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  KC_LCTL, _______, RGB_SOFT_OFF, _______, _______, _______,  KC_EQL,  KC_MINS, KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
  KC_LSFT, RGB_TOG, RGB_VAI, RGB_VAD, _______, _______, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                             KC_LGUI, TG(_GAMING), KC_ENT,      KC_SPC, _______, KC_RALT
),

/* GAMING (Left active, right disabled except exit)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Esc  |   Q  |   W  |   E  |   R  |   T  |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | LShf |   Z  |   X  |   C  |   V  |   B  |                    |      |      |      |      |      |      |
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | LAlt | Space| LCtrl|      | ____ | EXIT | ____ |
 *                                     `--------------------'      `--------------------'
 */
[_GAMING] = LAYOUT_split_3x6_3(
  KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,
                             KC_LALT, KC_SPC, KC_LCTL,      KC_NO, TG(_GAMING), KC_NO
),
};

#ifdef RGB_MATRIX_ENABLE
// Gaming overlay: keep dynamic effects and highlight WASD
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (get_highest_layer(layer_state) == _GAMING) {
        const uint8_t coords[][2] = {
            {0, 1}, // W
            {1, 0}, // A
            {1, 1}, // S
            {1, 2}  // D
        };
        for (uint8_t i = 0; i < sizeof(coords) / sizeof(coords[0]); i++) {
            uint8_t row = coords[i][0];
            uint8_t col = coords[i][1];
            uint8_t idx = g_led_config.matrix_co[row][col];
            if (idx != NO_LED && idx >= led_min && idx < led_max) {
                rgb_matrix_set_color(idx, 255, 40, 40);
            }
        }
    }
    return false;
}

// Per‑layer dynamic RGB modes (swirl/rainbow)
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _LOWER: {
            // Teal single-color sweep (band brightness left->right)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_BAND_VAL);
            rgb_matrix_sethsv_noeeprom(140, 110, 200);
            rgb_matrix_set_speed_noeeprom(200);
        } break;
        case _RAISE: {
            // Magenta single-color sweep (left->right)
            rgb_matrix_mode_noeeprom(RGB_MATRIX_BAND_VAL);
            rgb_matrix_sethsv_noeeprom(210, 130, 200);
            rgb_matrix_set_speed_noeeprom(160);
        } break;
        case _GAMING: {
            rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINDROPS);
            rgb_matrix_set_speed_noeeprom(180);
        } break;
        default: {
            // Base: default rainbow swirl
            rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_SPIRAL);
        } break;
    }
    return state;
}
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
    switch (keycode) {
        case KC_D_MUTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_MEH));
                register_code(KC_UP);
            } else {
                unregister_mods(mod_config(MOD_MEH));
                unregister_code(KC_UP);
            }
            break;
        case KC_CAPS:
            // Block Caps from any source unless explicitly allowed by firmware
            if (!allow_caps_toggle_internal) {
                return false;
            }
            break;
        case KC_GRV_INV:
            if (record->event.pressed) {
                uint8_t saved_mods = get_mods();
                uint8_t saved_oneshot = get_oneshot_mods();
                clear_oneshot_mods();
                del_mods(MOD_MASK_SHIFT);
                if ((saved_mods | saved_oneshot) & MOD_MASK_SHIFT) {
                    tap_code(KC_GRV); // backtick when Shift was held/oneshot
                } else {
                    tap_code16(S(KC_GRV)); // tilde when no Shift
                }
                set_mods(saved_mods);
                set_oneshot_mods(saved_oneshot);
                send_keyboard_report();
            }
            return false;
        case KC_LCTL:
        case KC_RCTL:
            chased_oled_on_ctrl(record->event.pressed);
            break;
        case KC_LSFT:
            physical_lshift_down = record->event.pressed;
            break;
        case KC_RSFT:
            physical_rshift_down = record->event.pressed;
            break;
        case KC_SPC:
            chased_oled_on_space(record->event.pressed);
            break;
        case RGB_SOFT_OFF:
#ifdef RGB_MATRIX_ENABLE
            if (record->event.pressed) {
                hsv_t hsv = rgb_matrix_get_hsv();
                rgb_matrix_sethsv_noeeprom(hsv.h, hsv.s, 0);
            }
            return false;
#endif
    }
    
    // Copy/paste OLED feedback removed; handled by userspace if desired
    
    return true;
}

void matrix_scan_user(void) {
    // Clear Caps once per activation to avoid rapid re-toggle loops
    bool is_on = host_keyboard_led_state().caps_lock;
    if (is_on && !caps_seen_on) {
        allow_caps_toggle_internal = true;
        tap_code(KC_CAPS);
        allow_caps_toggle_internal = false;
        caps_seen_on = true;
    } else if (!is_on) {
        caps_seen_on = false;
    }

    // Anti-stuck Shift: if Shift mod is active with no physical Shift key held, clear it after debounce
    bool shift_mod_active = (get_mods() & MOD_MASK_SHIFT) != 0;
    bool any_physical_shift = physical_lshift_down || physical_rshift_down;
    if (shift_mod_active && !any_physical_shift) {
        if (shift_stuck_since == 0) {
            shift_stuck_since = timer_read32();
        } else if (timer_elapsed32(shift_stuck_since) > 150) {
            del_mods(MOD_MASK_SHIFT);
            send_keyboard_report();
            shift_stuck_since = 0;
        }
    } else {
        shift_stuck_since = 0;
    }

    // No OS dual-role handling
}

bool led_update_user(led_t led_state) {
    // Cancel Caps immediately when host tries to enable it
    if (led_state.caps_lock && !caps_seen_on) {
        allow_caps_toggle_internal = true;
        tap_code(KC_CAPS);
        allow_caps_toggle_internal = false;
        caps_seen_on = true;
        return false; // suppress propagating Caps state
    }
    if (!led_state.caps_lock) {
        caps_seen_on = false;
    }
    return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return 225; // Match ZMK tapping-term-ms
}
