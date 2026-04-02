# chaseddevelopment keymap for crkbd/rev1

Authoritative Corne (crkbd rev1) keymap matching your ZMK semantics with QMK features.

Build (RP2040 converter, .uf2):

`qmk compile -kb crkbd/rev1 -km chaseddevelopment -e CONVERT_TO=rp2040_ce`

Flash: Put RP2040 into BOOTSEL (RPI-RP2), copy the generated `.uf2`. For splits: flash both halves.

Layer diagrams (aligned)

Base (QWERTY, HRMs)
```
[Tab ] [ Q  ] [ W  ] [ E  ] [ R  ] [ T  ]    [ Y  ] [ U  ] [ I  ] [ O  ] [ P  ] [Bspc]
[Esc ] [ A  ] [ S  ] [ D  ] [ F  ] [ G  ]    [ H  ] [ J  ] [ K  ] [ L  ] [ ;  ] [ '  ]
[LShf] [ Z  ] [ X  ] [ C  ] [ V  ] [ B  ]    [ N  ] [ M  ] [ ,  ] [ .  ] [ /  ] [RShf]
                 [LGUI] [LWR ] [ENT ]    [SPC ] [RSE ] [RAlt]
```
Home‑row mods: A=GUI, S=ALT, D=SHIFT, F=CTRL; J=CTRL, K=SHIFT, L=ALT, ;=GUI.

Lower (ZMK‑style numbers + arrows)
```
[Tab ] [ 1  ] [ 2  ] [ 3  ] [ 4  ] [ 5  ]    [ 6  ] [ 7  ] [ 8  ] [ 9  ] [ 0  ] [Bspc]
[____] [____] [____] [____] [____] [____]    [Left] [Down] [ Up ] [Rgt ] [____] [____]
[LShf] [____] [____] [____] [____] [____]    [____] [____] [____] [____] [____] [____]
                 [LGUI] [____] [ENT ]    [SPC ] [____] [RAlt]
```

Raise (symbols + Game toggle, ZMK layout)
```
[ BT ] [ !  ] [ @  ] [ #  ] [ $  ] [ %  ]    [ ^  ] [ &  ] [ *  ] [ (  ] [ )  ] [Bspc]
[Ctrl] [extP] [rgb0] [____] [____] [____]    [ =  ] [ -  ] [ [  ] [ ]  ] [ \  ] [ `  ]
[LShf] [RGBT] [BRI+] [BRI-] [____] [____]    [ _  ] [ +  ] [ {  ] [ }  ] [ |  ] [ ~  ]
                 [LGUI] [GAME] [ENT ]    [SPC ] [____] [RAlt]
```

Gaming (ZMK left active; right blank; right mid thumb exits)
```
[Esc ] [ Q  ] [ W  ] [ E  ] [ R  ] [ T  ]    [____] [____] [____] [____] [____] [____]
[Ctrl] [ A  ] [ S  ] [ D  ] [ F  ] [ G  ]    [____] [____] [____] [____] [____] [____]
[LShf] [ Z  ] [ X  ] [ C  ] [ V  ] [ B  ]    [____] [____] [____] [____] [____] [____]
                 [LAlt] [SPC ] [LCTL]    [____] [EXIT] [____]
```

Notes
- Toggle Gaming from Raise left thumb middle (GAME). Exit from Gaming right thumb middle.
- OLED turns off on idle. RGB stays on.
- Per‑layer RGB: Base swirl; Lower teal single‑color sweep; Raise magenta single‑color sweep; Gaming raindrops + WASD red.

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
