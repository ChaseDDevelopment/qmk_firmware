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

#pragma once

// OLED: explicit panel size and custom font glyphs (shared with Sofle)
#ifndef OLED_DISPLAY_128X32
#    define OLED_DISPLAY_128X32
#endif

// Many 0.91" 128x32 panels on Corne use an SH1106 controller. When driven as
// SSD1306 the output looks horizontally "squished" or duplicated. Selecting
// the correct IC (and a small column offset) fixes that rendering.
#ifndef OLED_IC
#    define OLED_IC OLED_IC_SH1106
#endif
// SH1106 has a 132‑column RAM; shift drawing right so it’s centered.
#ifndef OLED_COLUMN_OFFSET
#    define OLED_COLUMN_OFFSET 2
#endif

#ifndef OLED_FONT_H
#    define OLED_FONT_H "users/chaseddevelopment/oled/fonts/glcdfont.c"
#endif

// Increase VIA dynamic layers to allow future expansion
#ifndef DYNAMIC_KEYMAP_LAYER_COUNT
#    define DYNAMIC_KEYMAP_LAYER_COUNT 6
#endif

// Split keyboard configuration
#define SPLIT_WPM_ENABLE
#define MASTER_LEFT

// OLED configuration
#define CUSTOM_LAYER_READ
#define OLED_TIMEOUT 30000
// Right-hand (slave) OLED: render the spaceship WPM scene
// Left-hand (master) OLED: Luna + layer/mod glyphs
#define CHASED_OLED_RIGHT_SPACESHIP
#define SPLIT_OLED_ENABLE

// Tapping configuration (home‑row mods feel similar to ZMK)
#undef QUICK_TAP_TERM
#define QUICK_TAP_TERM 150
#undef TAPPING_TERM
#define TAPPING_TERM 225
#define TAPPING_TERM_PER_KEY

#define CHORDAL_HOLD

// RGB Matrix configuration (Corne rev1-style: 54 LEDs total, 27 per side)
#ifdef RGB_MATRIX_ENABLE
    #ifdef RGB_MATRIX_LED_COUNT
    #    undef RGB_MATRIX_LED_COUNT
    #endif
    #define RGB_MATRIX_LED_COUNT 54            // Total LEDs (27 per side)
    #define RGB_MATRIX_KEYPRESSES              // enable reactive effects
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS     // enable heatmap/digital rain
    #define RGB_MATRIX_SLEEP                   // turn off effects when suspended
    // Increase maximum brightness for full-vivid startup.
    // If you see instability (USB brownouts), reduce back toward 120–140.
    #ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #endif
    // Clamp overall brightness further to reduce USB power draw
    // while preserving hues/effects.
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 100

    // Enable full set of effects
    #define ENABLE_RGB_MATRIX_ALPHAS_MODS
    #define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    #define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_BREATHING
    #define ENABLE_RGB_MATRIX_BAND_SAT
    #define ENABLE_RGB_MATRIX_BAND_VAL
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    #define ENABLE_RGB_MATRIX_CYCLE_ALL
    #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define ENABLE_RGB_MATRIX_DUAL_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #define ENABLE_RGB_MATRIX_FLOWER_BLOOMING
    #define ENABLE_RGB_MATRIX_RAINDROPS
    #define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #define ENABLE_RGB_MATRIX_HUE_BREATHING
    #define ENABLE_RGB_MATRIX_HUE_PENDULUM
    #define ENABLE_RGB_MATRIX_HUE_WAVE
    #define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    #define ENABLE_RGB_MATRIX_PIXEL_FLOW
    #define ENABLE_RGB_MATRIX_PIXEL_RAIN
    #define ENABLE_RGB_MATRIX_STARLIGHT
    #define ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH
    #define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE
    #define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT
    #define ENABLE_RGB_MATRIX_RIVERFLOW
    #define ENABLE_RGB_MATRIX_TYPING_HEATMAP
    #define ENABLE_RGB_MATRIX_DIGITAL_RAIN
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    #define ENABLE_RGB_MATRIX_SPLASH
    #define ENABLE_RGB_MATRIX_MULTISPLASH
    #define ENABLE_RGB_MATRIX_SOLID_SPLASH
    #define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
    
    // Default mode (overridden per layer at runtime)
    #undef RGB_MATRIX_DEFAULT_MODE
    #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_RAINBOW_PINWHEELS
    #define RGB_MATRIX_DEFAULT_HUE 175
    #define RGB_MATRIX_DEFAULT_SAT 255
    // Raise default value (brightness) for richer colors on rainbow effects
    #ifdef RGB_MATRIX_DEFAULT_VAL
    #    undef RGB_MATRIX_DEFAULT_VAL
    #endif
    // Leave default at full; runtime clamp above limits actual output.
    #define RGB_MATRIX_DEFAULT_VAL 255
    
    // Step sizes for RGB adjustments
    #define RGB_MATRIX_HUE_STEP 8
    #define RGB_MATRIX_SAT_STEP 8
    #define RGB_MATRIX_VAL_STEP 8
    #define RGB_MATRIX_SPD_STEP 10
    
    // Performance: throttle LED processing & flush rate
    #define RGB_MATRIX_LED_PROCESS_LIMIT (RGB_MATRIX_LED_COUNT + 4) / 5
    #define RGB_MATRIX_LED_FLUSH_LIMIT 16
#endif

// Encoder support (if using encoder-enabled Corne)
#ifdef ENCODER_ENABLE
    #define ENCODER_DIRECTION_FLIP
#endif

// Split sync optimizations for Corne
#define SPLIT_LAYER_STATE_ENABLE               // Sync layer state between halves
#define SPLIT_LED_STATE_ENABLE                 // Sync LED state between halves
#define SPLIT_MODS_ENABLE                      // Sync modifiers between halves

// Trim debug/print code from firmware
#ifndef NO_DEBUG
#define NO_DEBUG
#endif
#ifndef NO_PRINT
#define NO_PRINT
#endif
