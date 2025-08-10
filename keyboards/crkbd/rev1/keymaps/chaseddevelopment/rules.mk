MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
CONSOLE_ENABLE = no
RGB_MATRIX_ENABLE = yes      # Use RGB Matrix for Corne r2g variant (54 LEDs)
RGBLIGHT_ENABLE = no         # Disable RGBLIGHT since we're using RGB_MATRIX
ENCODER_ENABLE = no          # Enable if using encoder-equipped Corne
LTO_ENABLE = yes            # Link Time Optimization for smaller firmware
OLED_ENABLE = yes           # Enable OLED displays
WPM_ENABLE = yes            # Enable WPM tracking for Luna animation
TRI_LAYER_ENABLE = no       # Not using tri-layer in this keymap

# Optimize for space - disable unused features
MAGIC_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no

# RP2040-converted Corne v3 specific settings
# Note: Uses r2g variant which maintains ATmega32u4 pinout with RP2040 chip