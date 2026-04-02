// Copyright 2024 Santosh Kumar (@santosh)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// OLED: explicit panel size and custom font glyphs
#ifndef OLED_DISPLAY_128X32
#    define OLED_DISPLAY_128X32
#endif

#ifndef OLED_FONT_H
#    define OLED_FONT_H "users/chaseddevelopment/oled/fonts/glcdfont.c"
#endif

// Increase VIA dynamic layers to allow future expansion
#ifndef DYNAMIC_KEYMAP_LAYER_COUNT
#    define DYNAMIC_KEYMAP_LAYER_COUNT 8
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

// RGB Matrix configuration (Sofle: 72 LEDs total, 36 per side)
#ifdef RGB_MATRIX_ENABLE
    #ifdef RGB_MATRIX_LED_COUNT
    #    undef RGB_MATRIX_LED_COUNT
    #endif
    #define RGB_MATRIX_LED_COUNT 72            // Total LEDs (36 per side)
    #define RGB_MATRIX_KEYPRESSES              // enable reactive effects
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS     // enable heatmap/digital rain
    #define RGB_MATRIX_SLEEP                   // turn off effects when suspended
    // Reduced maximum brightness to prevent USB brownouts with more LEDs
    // Sofle has 72 LEDs vs Corne's 54, so we use 70 instead of 100
    #ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #endif
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
    // Reduced default brightness to prevent brownouts
    #ifdef RGB_MATRIX_DEFAULT_VAL
    #    undef RGB_MATRIX_DEFAULT_VAL
    #endif
    #define RGB_MATRIX_DEFAULT_VAL 230
    
    // Step sizes for RGB adjustments
    #define RGB_MATRIX_HUE_STEP 8
    #define RGB_MATRIX_SAT_STEP 8
    #define RGB_MATRIX_VAL_STEP 8
    #define RGB_MATRIX_SPD_STEP 10
    
    // Performance: throttle LED processing & flush rate
    #define RGB_MATRIX_LED_PROCESS_LIMIT (RGB_MATRIX_LED_COUNT + 4) / 5
    #define RGB_MATRIX_LED_FLUSH_LIMIT 16
    
    // Split RGB Matrix configuration (36 LEDs per half)
    #define RGB_MATRIX_SPLIT { 36, 36 }
    // Sync keypresses between halves for reactive effects
    #define SPLIT_TRANSPORT_MIRROR
#endif

// Encoder support (if using encoder-enabled Sofle)
#ifdef ENCODER_ENABLE
    #define ENCODER_DIRECTION_FLIP
#endif

// Split sync optimizations for Sofle
#define SPLIT_LAYER_STATE_ENABLE               // Sync layer state between halves
#define SPLIT_LED_STATE_ENABLE                 // Sync LED state between halves
#define SPLIT_MODS_ENABLE                      // Sync modifiers between halves

