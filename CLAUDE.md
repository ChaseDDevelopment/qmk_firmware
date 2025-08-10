# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## QMK Firmware Repository

This is the QMK (Quantum Mechanical Keyboard) Firmware repository for custom keyboard development. The project contains firmware for hundreds of keyboards with a focus on split keyboards, custom layouts, and advanced features like RGB lighting, OLED displays, and encoders.

## Development Commands

### Building Firmware
```bash
# Compile firmware for a specific keyboard and keymap
qmk compile -kb <keyboard> -km <keymap>

# Example: Compile custom Sofle RGB keymap
qmk compile -kb sofle -km chaseddevelopment

# Example: Compile custom Corne keymap (RP2040-converted v3)
qmk compile -kb crkbd/r2g -km chaseddevelopment
```

### Flashing Firmware
```bash
# Flash firmware to connected keyboard
qmk flash -kb <keyboard> -km <keymap>

# Example: Flash Corne with custom keymap (RP2040-converted v3)
qmk flash -kb crkbd/r2g -km chaseddevelopment
```

### Development Tools
```bash
# Check QMK configuration
qmk doctor

# Generate compilation database for IDE support
qmk generate-compilation-database -kb <keyboard> -km <keymap>

# Lint keymap code
qmk lint -km <keymap>

# Format C code
qmk cformat
```

## Architecture Overview

### Directory Structure
```
keyboards/
├── sofle/              # Sofle keyboard definitions
│   ├── keymaps/        # Custom keymaps
│   │   └── chaseddevelopment/  # Custom Sofle RGB setup
│   ├── info.json       # Keyboard hardware definition
│   └── readme.md
├── crkbd/              # Corne keyboard definitions  
│   ├── keymaps/        # Custom keymaps
│   │   └── default/    # Default Corne layout
│   ├── info.json       # Keyboard hardware definition
│   └── readme.md
quantum/                # Core QMK functionality
├── rgb_matrix/         # RGB matrix lighting system
├── rgblight/          # RGB underglow system
├── oled/              # OLED display drivers
└── split_common/      # Split keyboard communication
```

### Key Components

**Keyboard Definition Files:**
- `info.json`: Hardware layout, pin assignments, feature capabilities
- `config.h`: Hardware-specific configuration defaults
- `keyboard.c`: Hardware initialization and custom functions

**Keymap Files:**
- `keymap.c`: Layer definitions, key assignments, custom logic
- `config.h`: User-specific configuration overrides
- `rules.mk`: Feature enablement (RGB, OLED, encoders, etc.)

**Split Keyboard Communication:**
- Serial communication between halves via TRRS cable
- Synchronized RGB lighting, OLED displays, and layer states
- Master/slave configuration with `MASTER_LEFT` or `MASTER_RIGHT`

### RGB Lighting Systems

**RGB Underglow (RGBLIGHT):**
- Used in Sofle RGB and many other keyboards
- Controls LED strips around keyboard perimeter
- Layer-based lighting with predefined patterns
- Configuration: `RGBLIGHT_LED_COUNT`, `RGBLED_SPLIT`

**RGB Matrix (RGB_MATRIX):**
- Per-key RGB lighting (Corne, Planck, etc.)
- Individual LED control with advanced animations
- Reactive typing effects and custom patterns
- Configuration: `RGB_MATRIX_LED_COUNT`, matrix layout definition

### OLED Display Integration
- 128x32 or 128x64 pixel displays on split keyboards
- Real-time information: layers, WPM, caps lock status
- Custom graphics: logos, animations (like Luna pet)
- Display rotation: `OLED_ROTATION_270` for side-mounted displays

### Layer Management
- Up to 32 layers with `layer_on()`, `layer_off()`, `layer_state_cmp()`
- Momentary layers with `MO(n)` or toggle with `TG(n)`
- Custom keycodes for advanced layer switching logic
- Tri-layer functionality for accessing adjustment layers

## Custom Keymap Development

### Essential Configuration Options
```c
// Split keyboard settings
#define MASTER_LEFT
#define SPLIT_WPM_ENABLE

// Timing configuration  
#define TAPPING_TERM 200
#define QUICK_TAP_TERM 0

// RGB configuration
#define RGBLIGHT_LAYERS           // Layer-based RGB
#define RGBLIGHT_SLEEP           // Turn off when sleeping
#define RGB_MATRIX_KEYPRESSES    // React to key presses

// OLED configuration
#define OLED_TIMEOUT 30000       // 30 second timeout
```

### Feature Integration
- **Encoders**: Rotary encoder support with `encoder_update_user()`
- **WPM Tracking**: Words per minute calculation with `get_current_wpm()`
- **Layer Callbacks**: `layer_state_set_user()` for RGB/OLED updates
- **Custom Keycodes**: `process_record_user()` for advanced functionality

### Testing and Debugging
```bash
# Enable console output for debugging
qmk compile -kb <keyboard> -km <keymap> -e CONSOLE_ENABLE=yes

# Use hid_listen tool to view debug output
# Available on most platforms for real-time firmware debugging
```

## Common Development Patterns

### RGB Layer Indication
Define layer-specific RGB patterns to provide visual feedback about active layers, using consistent color coding across the entire system.

### OLED Status Display
Implement master/slave specific OLED content - typically status information on master side, decorative content on slave side.

### Split Keyboard Synchronization
Ensure RGB lighting, OLED content, and custom variables are properly synchronized between keyboard halves using QMK's split transport system.

### Memory Optimization
Use `LTO_ENABLE = yes` and disable unused features to fit complex firmware within flash memory constraints, especially important for AVR-based keyboards.