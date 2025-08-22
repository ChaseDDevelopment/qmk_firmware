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

// Internal guard to allow firmware to clear Caps Lock while blocking all other sources
static bool allow_caps_toggle_internal = false;
static bool caps_seen_on = false;
static bool physical_lshift_down = false;
static bool physical_rshift_down = false;
static uint32_t shift_stuck_since = 0;

enum custom_keycodes {
    KC_LOWER = SAFE_RANGE,
    KC_RAISE,
    KC_ADJUST,
    KC_D_MUTE,
    KC_GRV_INV,
    OS_TOG,
    CTL_WIN
};

// Tap-Hold keys
#define CTL_ESC MT(MOD_LCTL, KC_ESC)  // Tap for Esc, Hold for Ctrl (Windows/Linux)
#define OPT_ESC MT(MOD_LALT, KC_ESC)  // Tap for Esc, Hold for Option/Alt (macOS)
#define CTL_GUI MT(MOD_LGUI, KC_LCTL) // Tap for Ctrl, Hold for GUI/Meta (legacy)

// State for custom Ctrl/Win dual-role key
static bool ctrl_win_down = false;
static bool ctrl_win_sent_ctrl = false;
static bool ctrl_win_sent_gui = false;
static uint16_t ctrl_win_timer = 0;

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
                              CTL_WIN, KC_LOWER, KC_ENT,      KC_SPC, KC_RAISE, KC_RCTL
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
  KC_LCTL,  KC_INS,  KC_PSCR, KC_APP,  C(KC_A), XXXXXXX,                   KC_EQL,  KC_MINS, KC_BSLS, KC_PIPE, KC_HOME, KC_END,
  KC_LSFT,  C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX,                   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_DEL,  TG(_GAMING),
                              KC_LALT, _______, KC_ENT,       KC_SPC, _______, KC_RGUI
),

/* GAMING
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |                    |   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------+------+------+------+------+------+-----------------------------------'
 *                                     | Ctrl  | Space| Enter|      | Space| RAISE|GAMING|
 *                                     `--------------------'      `--------------------'
 */
[_GAMING] = LAYOUT_split_3x6_3(
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  KC_ESC,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                     KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  TG(_GAMING),
                                KC_LCTL, KC_SPC,  KC_ENT,      KC_SPC, KC_RAISE, KC_ESC
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
  KC_LALT,  KC_INS,  KC_PSCR, KC_APP,  G(KC_A), XXXXXXX,                   KC_EQL,  KC_MINS, KC_BSLS, KC_PIPE, KC_GRV,  KC_TILD,
  KC_LSFT,  G(KC_Z), G(KC_X), G(KC_C), G(KC_V), C(S(KC_C)),                KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_DEL,  C(S(KC_V)),
                              KC_LGUI, _______, KC_ENT,       KC_SPC, _______, KC_RCTL
),

/* ADJUST - Settings layer (LOWER + RAISE) */
[_ADJUST] = LAYOUT_split_3x6_3(
  QK_BOOT,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLD, KC_VOLU, KC_MUTE,
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
    
    // If CTL_WIN is being held and another key is pressed before long-hold threshold,
    // turn it into a Ctrl modifier for chording (copy/paste, etc.)
    if (record->event.pressed && ctrl_win_down && !ctrl_win_sent_gui && !ctrl_win_sent_ctrl && keycode != CTL_WIN) {
        register_code(KC_LCTL);
        ctrl_win_sent_ctrl = true;
        send_keyboard_report();
    }

    switch (keycode) {
        case CTL_WIN: {
            if (record->event.pressed) {
                ctrl_win_down = true;
                ctrl_win_sent_ctrl = false;
                ctrl_win_sent_gui = false;
                ctrl_win_timer = timer_read();
            } else {
                // On release: if GUI was sent, unregister; if Ctrl was sent, unregister
                if (ctrl_win_sent_gui) {
                    unregister_code(KC_LGUI);
                }
                if (ctrl_win_sent_ctrl) {
                    unregister_code(KC_LCTL);
                }
                ctrl_win_down = false;
            }
            chased_oled_on_ctrl(record->event.pressed);
            return false;
        }
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
        case CTL_GUI:
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

    // Custom Ctrl/Win dual-role: during hold, after threshold, send GUI; on quick tap/hold chord, send Ctrl
    if (ctrl_win_down) {
        uint16_t elapsed = timer_elapsed(ctrl_win_timer);
        // If already sent GUI or CTRL, nothing to do until release
        if (!ctrl_win_sent_gui && !ctrl_win_sent_ctrl) {
            if (elapsed > 300) {
                // Long hold: send GUI
                register_code(KC_LGUI);
                ctrl_win_sent_gui = true;
                send_keyboard_report();
            }
        }
    }
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
    switch (keycode) {
        case CTL_WIN:
            return 300; // longer hold required for Meta
        default:
            return TAPPING_TERM;
    }
}