# Userspace build configuration for chaseddevelopment

# Ensure this userspace sources are compiled
SRC += $(USER_PATH)/oled/oled_views.c
SRC += $(USER_PATH)/oled/oled_sprites.c
SRC += $(USER_PATH)/oled/mod_icons.c
SRC += $(USER_PATH)/oled/spaceship_view.c

# Optional feature toggles (set via OPT_DEFS in keymap rules.mk or config.h defines)
# OPT_DEFS += -DCHASED_OLED_GAMEPAD_BADGE
# OPT_DEFS += -DCHASED_OLED_PET_ON_SLAVE
# OPT_DEFS += -DCHASED_OLED_RIGHT_SPACESHIP

