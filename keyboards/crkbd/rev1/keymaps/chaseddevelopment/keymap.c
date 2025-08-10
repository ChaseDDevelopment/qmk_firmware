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
                              KC_LGUI, KC_LOWER, KC_ENT,      KC_SPC, KC_RAISE, KC_RALT
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
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_F12,
  KC_LCTL,  KC_F1,  KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_HOME, KC_END,
  KC_LSFT,  KC_F6,  KC_F7,   KC_F8,   KC_F9,   KC_F10,                    KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_PGUP, KC_PGDN,
                              KC_LGUI, _______, KC_ENT,       KC_SPC, _______, KC_RALT
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
  KC_LCTL,  KC_INS,  KC_PSCR, KC_APP,  C(KC_A), KC_CAPS,                   KC_EQL,  KC_MINS, KC_BSLS, KC_PIPE, KC_GRV,  KC_TILD,
  KC_LSFT,  C(KC_Z), C(S(KC_X)), C(S(KC_C)), C(S(KC_V)), XXXXXXX,         KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_DEL,  TG(_GAMING),
                              KC_LGUI, _______, KC_ENT,       KC_SPC, _______, KC_RALT
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
        
        // Light up outer columns and thumb clusters for layer indication
        for (uint8_t i = led_min; i < led_max; i++) {
            switch (i) {
                case 0: case 6: case 12: case 18:  // Left outer column + thumb
                case 21: case 27: case 33: case 39: // Right outer column + thumb
                    rgb_matrix_set_color(i, r, g, b);
                    break;
            }
        }
    }
    return false;
}
#endif

#ifdef OLED_ENABLE

/* KEYBOARD PET START */

/* settings */
#define MIN_WALK_SPEED      10
#define MIN_RUN_SPEED       40

/* advanced settings */
#define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#define ANIM_SIZE           96   // number of bytes in array

/* timers */
uint32_t anim_timer = 0;

/* current frame */
uint8_t current_frame = 0;

/* status variables */
int   current_wpm = 0;
led_t led_usb_state;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

/* logic */
static bool isBarking = false;

static void animate_luna(void) {
    // Animation state changes based on WPM and caps lock
    if (led_usb_state.caps_lock) {
        isBarking = true;
        isSneaking = false;
        isJumping = false;
    } else {
        isBarking = false;
        if (current_wpm > MIN_RUN_SPEED) {
            isSneaking = false;
            isJumping = false;
        } else if (current_wpm > MIN_WALK_SPEED) {
            isSneaking = false;
            isJumping = false;
        } else {
            isSneaking = false;
            isJumping = false;
        }
    }
}

static void render_luna(int LUNA_X, int LUNA_Y) {
    /* Sit */
    static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },
                                                   /* 'sit2', 32x22px */
                                                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

    /* Walk */
    static const char PROGMEM walk[2][ANIM_SIZE] = {/* 'walk1', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },
                                                    /* 'walk2', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Run */
    static const char PROGMEM run[2][ANIM_SIZE] = {/* 'run1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },
                                                   /* 'run2', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   }};

    /* Sneak */
    static const char PROGMEM sneak[2][ANIM_SIZE] = {/* 'sneak1', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                     },
                                                     /* 'sneak2', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     }};
    
    /* Bark */
    static const char PROGMEM bark[2][ANIM_SIZE] = {/* 'bark1', 32x22px */
                                                    {
                                                        0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },
                                                    /* 'bark2', 32x22px */
                                                    {
                                                        0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};
    
    // Clear jump area if needed
    if (isJumping || !showedJump) {
        oled_set_cursor(LUNA_X, LUNA_Y + 2);
        oled_write("     ", false);
        oled_set_cursor(LUNA_X, LUNA_Y - 1);
        showedJump = true;
    } else {
        oled_set_cursor(LUNA_X, LUNA_Y - 1);
        oled_write("     ", false);
        oled_set_cursor(LUNA_X, LUNA_Y);
    }
    
    // Render the current animation frame
    if (isBarking) {
        oled_write_raw_P(bark[current_frame], ANIM_SIZE);
    } else if (isSneaking) {
        oled_write_raw_P(sneak[current_frame], ANIM_SIZE);
    } else if (current_wpm <= MIN_WALK_SPEED) {
        oled_write_raw_P(sit[current_frame], ANIM_SIZE);
    } else if (current_wpm <= MIN_RUN_SPEED) {
        oled_write_raw_P(walk[current_frame], ANIM_SIZE);
    } else {
        oled_write_raw_P(run[current_frame], ANIM_SIZE);
    }
    
    // Switch frame for next animation cycle
    current_frame = (current_frame + 1) % 2;
}

// Commented out - not currently used
// static uint8_t ground_offset = 0;
// static uint16_t ground_timer = 0;
// static void render_ground_effect(void) { ... }

// For 32x128 vertical display (ROTATION_270)
// Each character position is 6x8 pixels, display is 5 cols x 16 rows of 8 pixels each

// Layer graphics - 30x24 pixels (5 chars wide × 3 rows tall)
static const char PROGMEM layer_default_graphic[] = {
    // Row 0 (30 bytes)
    0x00, 0x00, 0xFC, 0xFC, 0x0C, 0x0C, 0x38, 0xF0, 0xC0, 0x00,  // Q
    0x0C, 0x0C, 0xFC, 0xFC, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00,  // W  
    0xFC, 0xFC, 0x8C, 0x8C, 0x8C, 0x8C, 0x00, 0x00, 0x00, 0x00,  // E
    // Row 1 (30 bytes)
    0x00, 0x00, 0x0F, 0x0F, 0x0C, 0x0C, 0x0E, 0x07, 0x01, 0x00,
    0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
    // Row 2 (30 bytes)  
    0xFC, 0xFC, 0x8C, 0x8C, 0xCC, 0xFC, 0x78, 0x00, 0x00, 0x00,  // R
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00
};

static const char PROGMEM layer_lower_graphic[] = {
    // "123" graphic - 30x24 pixels
    // Row 0
    0x00, 0x00, 0x08, 0x08, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,  // 1
    0x00, 0x18, 0x38, 0x68, 0xC8, 0x88, 0x00, 0x00, 0x00, 0x00,  // 2
    0x00, 0x88, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x00,  // 3
    // Row 1
    0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0C, 0x0E, 0x0B, 0x09, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x08, 0x08, 0x08, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00,
    // Row 2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const char PROGMEM layer_raise_graphic[] = {
    // "!@#" symbols - 30x24 pixels
    // Row 0
    0x00, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00,  // !
    0x70, 0xF8, 0x88, 0xF8, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00,  // @
    0x20, 0xF8, 0xF8, 0x20, 0xF8, 0xF8, 0x20, 0x00, 0x00, 0x00,  // #
    // Row 1  
    0x00, 0x00, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x0F, 0x09, 0x09, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00,
    // Row 2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const char PROGMEM layer_gaming_graphic[] = {
    // Game controller - 30x24 pixels
    // Row 0
    0x00, 0xF0, 0xF8, 0x0C, 0x04, 0x04, 0x0C, 0xF8, 0xF0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xF0, 0xF8, 0x0C, 0x04, 0x04, 0x0C, 0xF8, 0xF0, 0x00,
    // Row 1
    0x00, 0x07, 0x0F, 0x18, 0x10, 0x10, 0x18, 0x0F, 0x07, 0x00,
    0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00,
    0x00, 0x07, 0x0F, 0x18, 0x10, 0x10, 0x18, 0x0F, 0x07, 0x00,
    // Row 2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const char PROGMEM layer_adjust_graphic[] = {
    // Gear/settings icon - 30x24 pixels
    // Row 0
    0x00, 0x00, 0x70, 0xF8, 0xFC, 0xFC, 0xF8, 0x70, 0x00, 0x00,
    0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0x00, 0x00,
    0x00, 0x70, 0xF8, 0xFC, 0xFC, 0xF8, 0x70, 0x00, 0x00, 0x00,
    // Row 1
    0x00, 0x00, 0x0E, 0x1F, 0x3F, 0x3F, 0x1F, 0x0E, 0x00, 0x00,
    0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00,
    0x00, 0x0E, 0x1F, 0x3F, 0x3F, 0x1F, 0x0E, 0x00, 0x00, 0x00,
    // Row 2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Modifier icons - 8x8 pixels each
static const char PROGMEM mod_shift_icon[] = {
    0x18, 0x24, 0x42, 0x81, 0xFF, 0x81, 0x81, 0xFF
};

static const char PROGMEM mod_ctrl_icon[] = {
    0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C
};

static const char PROGMEM mod_alt_icon[] = {
    0x18, 0x24, 0x42, 0x81, 0x81, 0xFF, 0x81, 0x81
};

static const char PROGMEM mod_gui_icon[] = {
    0xFF, 0x81, 0x99, 0xA5, 0xA5, 0x99, 0x81, 0xFF
};

// Copy/paste feedback
static bool show_copy_feedback = false;
static bool show_paste_feedback = false;
static uint16_t feedback_timer = 0;

// WPM graph functions - commented out as not currently used
// static uint8_t wpm_history[32] = {0};
// static uint8_t wpm_history_index = 0;
// static uint8_t max_wpm = 100;
// static void update_wpm_history(void) { ... }
// static void render_wpm_graph(void) { ... }

static void render_layer_state(void) {
    // Get current layer
    uint8_t layer = get_highest_layer(layer_state);
    
    // Map macOS layers to their Windows equivalents for display
    if (layer == _QWERTY_MAC) layer = _QWERTY;
    if (layer == _LOWER_MAC) layer = _LOWER;
    if (layer == _RAISE_MAC) layer = _RAISE;
    
    // Render layer graphic at position (0, 5) - 30x24 pixels (5x3 chars)
    oled_set_cursor(0, 5);
    
    // Write the appropriate layer graphic (90 bytes = 30x24 pixels)
    switch (layer) {
        case _LOWER:
            oled_write_raw_P(layer_lower_graphic, 90);
            break;
        case _RAISE:
            oled_write_raw_P(layer_raise_graphic, 90);
            break;
        case _GAMING:
            oled_write_raw_P(layer_gaming_graphic, 90);
            break;
        case _ADJUST:
            oled_write_raw_P(layer_adjust_graphic, 90);
            break;
        default:
            oled_write_raw_P(layer_default_graphic, 90);
            break;
    }
}

// --- Modifier status as icons (no text) ---
#define ICON8_SIZE 8
static const char PROGMEM empty_icon[ICON8_SIZE] = {0};

static void draw_icon_8x8(uint8_t x, uint8_t y, const char PROGMEM *icon, bool show) {
    oled_set_cursor(x, y);
    if (show) {
        oled_write_raw_P(icon, ICON8_SIZE);
    } else {
        oled_write_raw_P(empty_icon, ICON8_SIZE);
    }
}

static void render_mod_status(void) {
    uint8_t modifiers = get_mods() | get_oneshot_mods();

    // Two-by-two icon cluster starting at row 8
    // Top row: Shift, Ctrl
    draw_icon_8x8(0, 8, mod_shift_icon, (modifiers & MOD_MASK_SHIFT));
    draw_icon_8x8(2, 8, mod_ctrl_icon,  (modifiers & MOD_MASK_CTRL));

    // Bottom row: Alt, GUI/Cmd
    draw_icon_8x8(0, 9, mod_alt_icon,   (modifiers & MOD_MASK_ALT));
    draw_icon_8x8(2, 9, mod_gui_icon,   (modifiers & MOD_MASK_GUI));

    // Minimal status indicator (no text) at (4,9)
    if (host_keyboard_led_state().caps_lock) {
        draw_icon_8x8(4, 9, mod_ctrl_icon, true); // reuse as a filled marker
    } else if ((show_copy_feedback || show_paste_feedback) && timer_elapsed(feedback_timer) < 1000) {
        draw_icon_8x8(4, 9, mod_alt_icon, true);
    } else {
        draw_icon_8x8(4, 9, empty_icon, false);
        show_copy_feedback = false;
        show_paste_feedback = false;
    }
}

// (Removed text/logo rendering; slave renders Luna only)

static void print_status_narrow(void) {
    // Master display: graphics only
    // Rows 5-7: Layer state graphic (30x24 pixels)
    render_layer_state();
    // Rows 8-11: Modifiers as icons and minimal status
    render_mod_status();
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    } else {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    // Update status variables
    current_wpm = get_current_wpm();
    led_usb_state = host_keyboard_led_state();
    
    #if OLED_TIMEOUT > 0
    // Handle OLED timeout
    if (last_input_activity_elapsed() > OLED_TIMEOUT && last_led_activity_elapsed() > OLED_TIMEOUT) {
        oled_off();
        return false;
    } else {
        oled_on();
    }
    #endif
    
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        // Slave: Luna only, advance and render on timer to avoid tearing
        if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animate_luna();
            render_luna(0, 1);
        }
    }
    return false;
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
        case KC_LCTL:
        case KC_RCTL:
            if (record->event.pressed) {
                isSneaking = true;
            } else {
                isSneaking = false;
            }
            break;
        case KC_SPC:
            if (record->event.pressed) {
                isJumping = true;
                showedJump = false;
            } else {
                isJumping = false;
            }
            break;
    }
    
    // Detect copy/paste combinations for OLED feedback
    #ifdef OLED_ENABLE
    if (record->event.pressed) {
        uint8_t mods = get_mods();
        if (current_os == OS_MACOS) {
            // macOS uses Cmd for copy/paste
            if (mods & MOD_MASK_GUI) {
                if (keycode == KC_C) {
                    show_copy_feedback = true;
                    show_paste_feedback = false;
                    feedback_timer = timer_read();
                } else if (keycode == KC_V) {
                    show_paste_feedback = true;
                    show_copy_feedback = false;
                    feedback_timer = timer_read();
                }
            }
        } else {
            // Windows/Linux uses Ctrl+Shift for terminal copy/paste
            if ((mods & MOD_MASK_CTRL) && (mods & MOD_MASK_SHIFT)) {
                if (keycode == KC_C) {
                    show_copy_feedback = true;
                    show_paste_feedback = false;
                    feedback_timer = timer_read();
                } else if (keycode == KC_V) {
                    show_paste_feedback = true;
                    show_copy_feedback = false;
                    feedback_timer = timer_read();
                }
            }
        }
    }
    #endif
    
    return true;
}