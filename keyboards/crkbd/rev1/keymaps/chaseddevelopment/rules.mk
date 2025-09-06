MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
CONSOLE_ENABLE = no
RGB_MATRIX_ENABLE = yes      # Use RGB Matrix (54 LEDs on rev1-style boards)
RGBLIGHT_ENABLE = no         # Disable RGBLIGHT since we're using RGB_MATRIX
ENCODER_ENABLE = no          # Enable if using encoder-equipped Corne
LTO_ENABLE = yes            # Link Time Optimization for smaller firmware
OLED_ENABLE = yes           # Enable OLED displays
WPM_ENABLE = yes            # Enable WPM tracking for Luna animation
# Use shared userspace (OLED views/sprites)
USER_NAME := chaseddevelopment
TRI_LAYER_ENABLE = no       # Not using tri-layer in this keymap

# Enable VIA support (dynamic keymap over RAW HID)
VIA_ENABLE = yes

# Optimize for space - disable unused features
MAGIC_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no

# For RP2040-CE conversion, build with: -e CONVERT_TO=rp2040_ce
