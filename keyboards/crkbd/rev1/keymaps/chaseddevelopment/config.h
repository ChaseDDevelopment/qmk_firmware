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

// Split keyboard configuration
#define SPLIT_WPM_ENABLE
#define MASTER_LEFT

// OLED configuration
#define CUSTOM_LAYER_READ
#define OLED_TIMEOUT 30000

// Tapping configuration
#define QUICK_TAP_TERM 0
#ifdef TAPPING_TERM
    #undef TAPPING_TERM
    #define TAPPING_TERM 200
#endif

// RGB Matrix configuration (Corne r2g variant: 54 LEDs total, 27 per side)
#ifdef RGB_MATRIX_ENABLE
    #define RGB_MATRIX_LED_COUNT 54            // Total LEDs (27 per side for r2g variant)
    #define RGB_MATRIX_KEYPRESSES              // reacts to keypresses
    #define RGB_MATRIX_SLEEP                   // turn off effects when suspended
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120  // limits maximum brightness (was 150 in Sofle)
    
    // Enable rainbow and gradient animations
    #define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    #define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_CYCLE_ALL
    #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #define ENABLE_RGB_MATRIX_RAINDROPS
    
    // Default mode - Rainbow gradient
    #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_GRADIENT_LEFT_RIGHT
    
    // Step sizes for RGB adjustments
    #define RGB_MATRIX_HUE_STEP 8
    #define RGB_MATRIX_SAT_STEP 8
    #define RGB_MATRIX_VAL_STEP 8
    #define RGB_MATRIX_SPD_STEP 10
    
    // Disable some animations to save space
    #undef ENABLE_RGB_MATRIX_ALPHAS_MODS
    #undef ENABLE_RGB_MATRIX_BREATHING
    #undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    #undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #undef ENABLE_RGB_MATRIX_DUAL_BEACON
    #undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
    #undef ENABLE_RGB_MATRIX_DIGITAL_RAIN
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    #undef ENABLE_RGB_MATRIX_SPLASH
    #undef ENABLE_RGB_MATRIX_MULTISPLASH
    #undef ENABLE_RGB_MATRIX_SOLID_SPLASH
    #undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif

// Encoder support (if using encoder-enabled Corne)
#ifdef ENCODER_ENABLE
    #define ENCODER_DIRECTION_FLIP
#endif

// RP2040-converted Corne v3 optimizations (r2g variant)
#define SPLIT_LAYER_STATE_ENABLE               // Sync layer state between halves
#define SPLIT_LED_STATE_ENABLE                 // Sync LED state between halves
#define SPLIT_MODS_ENABLE                      // Sync modifiers between halves