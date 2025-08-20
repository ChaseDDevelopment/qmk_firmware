# chaseddevelopment userspace

This userspace provides a reusable OLED UI for 128x32 (and 128x64) displays.

- Master: cycles between a 3-line layer indicator block and 2-line mod icons (compact spacing, no overlap).
- Slave: Luna pet at the top; WPM number on the bottom line with a single-line bar.
- Fonts are embedded locally, no dependency on external repos.

## Files
- `users/chaseddevelopment/oled/oled.h`: public API and small wrappers
- `users/chaseddevelopment/oled/oled_views.c`: master/slave rendering logic
- `users/chaseddevelopment/oled/oled_sprites.c`: layer/mod glyphs, Luna sprites, WPM bar, badges
- `users/chaseddevelopment/oled/fonts/glcdfont.c`: classic 5x7 font bytes
- `users/chaseddevelopment/rules.mk`: userspace build rules

## Integrate in a keymap
1) `keymaps/<board>/rules.mk`
```
OLED_ENABLE = yes
WPM_ENABLE = yes
USER_NAME := chaseddevelopment
```
- If your board is split, ensure your split transport is configured. The module respects shared WPM when `SPLIT_WPM_ENABLE` is defined.

2) `keymaps/<board>/config.h`
```
// Panel size
#define OLED_DISPLAY_128X32   // or: #define OLED_DISPLAY_128X64

// Embedded font
#define OLED_FONT_H "users/chaseddevelopment/oled/fonts/glcdfont.c"

// If split and you want WPM mirrored to slave
#define SPLIT_WPM_ENABLE

// Optional (defaults shown):
// #define CHASED_OLED_PET_ON_SLAVE 1              // show Luna on slave
// #define CHASED_OLED_VIEW_SWAP_INTERVAL_MS 2500  // master view cycle
// #define CHASED_OLED_GAMEPAD_BADGE               // enable gaming badge
```

3) `keymaps/<board>/keymap.c`
```
#include "users/chaseddevelopment/oled/oled.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return chased_oled_init(rotation);
}

bool oled_task_user(void) {
  return oled_task_user_wrapped();
}
```

4) Optional Luna pet hooks in `process_record_user`:
```
case KC_LCTL:
case KC_RCTL:
  chased_oled_on_ctrl(record->event.pressed);
  break;
case KC_SPC:
  chased_oled_on_space(record->event.pressed);
  break;
```

5) Optional gaming badge integration
- Enable with `#define CHASED_OLED_GAMEPAD_BADGE`.
- Override in your keymap if needed:
```
bool chased_is_game_layer_active(void) {
  return layer_state_is(_GAMING);  // or your custom condition
}
```

## Notes
- 128x32 writes are fully guarded (no page > 3).
- After delegating to userspace, you can remove old OLED helpers/assets from your keymap to save flash.
- Extend `glcdfont.c` if you want custom glyphs.



