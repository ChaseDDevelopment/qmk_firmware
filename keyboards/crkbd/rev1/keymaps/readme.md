# Corne Keyboard - ChaseD Development Layout

Custom Corne (CRKBD) keymap adapted from Sofle RGB firmware, maintaining key features while optimized for the 42-key layout.

## Features

### 🎨 RGB Matrix Lighting
- **Layer-based RGB indication**: Different colors for each layer (Red/Cyan/Blue/Purple)
- **Per-key RGB lighting**: Outer columns and thumb clusters illuminate to show active layer
- **Customizable patterns**: Easily modify colors and patterns in the keymap

### 🖥️ OLED Displays
- **Luna Pet Animation**: Beloved pixel pet that responds to typing speed and modifier keys
  - Sits when idle
  - Walks with slow typing
  - Runs with fast typing  
  - Sneaks when Ctrl is held
  - Jumps when Space is pressed
- **Layer Status**: Real-time display of current layer and keyboard mode
- **WPM Counter**: Words per minute tracking with live display

### ⌨️ Layout Design

**Base Layer (QWERTY)**
```
Tab  Q  W  E  R  T        Y  U  I  O  P  Bspc
Esc  A  S  D  F  G        H  J  K  L  ;  '
Shift Z X  C  V  B        N  M  ,  .  /  Shift
       Gui Lower Enter    Space Raise Alt
```

**Lower Layer (Numbers & Navigation)**
- Numbers 1-0 on top row
- Function keys F1-F10 
- Arrow keys and navigation (Home/End/PgUp/PgDn)
- Bracket keys

**Raise Layer (Symbols & Functions)**
- Symbols (!@#$%^&*())
- Editing functions (Copy/Cut/Paste/Undo)
- Additional brackets and special characters
- Gaming mode toggle

**Gaming Layer**
- Standard QWERTY layout
- Optimized for gaming with Space on left thumb
- Gaming mode toggle to return to normal layers

## Key Adaptations from Sofle RGB

### Layout Changes
- **Condensed from 60 to 42 keys**: Removed number row, consolidated function keys
- **Smart layer design**: Numbers moved to Lower layer, symbols optimized in Raise layer
- **Thumb cluster optimization**: Efficient use of Corne's 3-key thumb clusters

### RGB System Migration
- **RGBLIGHT → RGB_MATRIX**: Converted from underglow strips to per-key matrix
- **Pattern adaptation**: Maintained color-coded layer system with new LED positions
- **Performance optimization**: Disabled unused animations to save memory

### OLED Optimization
- **Luna pet preserved**: Complete animation system adapted for Corne displays
- **Layout adjustments**: Optimized for Corne's OLED positioning and rotation

## Building & Flashing

### For Pro Micro footprint Corne (rev1-style) + RP2040-CE (Recommended)
```bash
# Compile the firmware using RP2040-CE conversion
qmk compile -kb crkbd/rev1 -km chaseddevelopment -e CONVERT_TO=rp2040_ce

# Flash to keyboard  
qmk flash -kb crkbd/rev1 -km chaseddevelopment -e CONVERT_TO=rp2040_ce
```

### Hardware Compatibility
- **rev1-style (Pro Micro footprint)**: 54 LEDs (27 per side). Use `-e CONVERT_TO=rp2040_ce` for RP2040-CE
- **rev4_0/rev4_1**: Newer RP2040 Corne v4 (46 LEDs - requires different LED configuration)

## Customization

### Changing RGB Colors
Edit the `rgb_matrix_layer_colors` array in keymap.c:
```c
const uint8_t PROGMEM rgb_matrix_layer_colors[][3] = {
    [_QWERTY] = {255, 0, 0},      // Red
    [_LOWER]  = {0, 255, 255},    // Cyan  
    [_RAISE]  = {0, 0, 255},      // Blue
    [_GAMING] = {255, 0, 255}     // Purple
};
```

### Adding Encoder Support
If using an encoder-equipped Corne:
1. Set `ENCODER_ENABLE = yes` in rules.mk
2. Uncomment encoder configuration in config.h
3. Add encoder mapping to keymap.c

### OLED Customization
- Modify `print_status_narrow()` for custom status display
- Adjust Luna animation speed with `ANIM_FRAME_DURATION`
- Add custom OLED graphics in the animation functions

## Layer Reference

| Layer | Purpose | Access |
|-------|---------|---------|
| QWERTY | Base typing layer | Default |
| LOWER | Numbers, F-keys, navigation | Hold Lower key |
| RAISE | Symbols, editing, special functions | Hold Raise key |
| GAMING | Gaming-optimized layout | Toggle from Raise layer |

## Troubleshooting

**Compile Issues:**
- Ensure QMK is updated: `qmk doctor`
- Check feature compatibility: Some features may need adjustment based on Corne variant

**RGB Not Working:**
- Verify RGB_MATRIX_ENABLE in rules.mk  
- Check that RGBLIGHT_ENABLE is set to 'no'
- Confirm RGB Matrix LED count matches your Corne variant:
  - rev1-style: 54 LEDs (27 per side)
  - rev4_0/rev4_1: 46 LEDs (23 per side)
- Use correct build target and/or conversion flag for your hardware

**OLED Issues:**
- Verify OLED_ENABLE in rules.mk
- Check OLED orientation with `OLED_ROTATION_270`
- Ensure sufficient firmware space (enable LTO_ENABLE)