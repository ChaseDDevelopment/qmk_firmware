#include "oled.h"

#ifdef CHASED_OLED_RIGHT_SPACESHIP

// Full-screen spaceship-as-WPM visualization for 128x32 panels.
// Builds a 128x32 frame (4 pages x 128 bytes) with a simple starfield,
// a scrolling WPM trace, and a small ship sprite on the right whose
// vertical position follows the current WPM.

#if defined(OLED_DISPLAY_128X32)
#    define PANEL_PAGES 4
#else
#    define PANEL_PAGES 8
#endif

#define PANEL_WIDTH 128
#define FRAME_MS    200

static uint32_t frame_timer = 0;
static uint8_t  wpm_hist[PANEL_WIDTH] = {0};
static uint8_t  wpm_hist_idx          = 0; // points to newest column

// Simple PRNG for starfield
static uint32_t rng_state = 0xA5A5F00DUL;
static inline uint32_t rnd(void) {
    // xorshift32
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

static uint8_t scale_wpm_to_32(uint8_t wpm) {
    uint16_t v = wpm;
    if (v > 180) v = 180;
    // Approx scale: 0..31
    return (uint8_t)((v * 31) / 180);
}

static void draw_starfield(uint8_t *frame) {
    // Sprinkle a few stars per frame to give subtle twinkle
    for (int s = 0; s < 20; s++) {
        uint16_t r = (uint16_t)rnd();
        uint8_t  x = (uint8_t)(r & 0x7F);           // 0..127
        uint8_t  y = (uint8_t)((r >> 7) & 0x1F);    // 0..31
        uint8_t  p = (uint8_t)(y >> 3);             // page 0..3
        uint8_t  b = (uint8_t)(1u << (y & 7));      // bit within page
        frame[p * PANEL_WIDTH + x] |= b;
    }
}

static void draw_wpm_trace(uint8_t *frame) {
    // Draws a single pixel per column at the recorded height
    for (uint8_t i = 0; i < PANEL_WIDTH; i++) {
        uint8_t col = (uint8_t)((wpm_hist_idx - i) & (PANEL_WIDTH - 1));
        uint8_t h   = wpm_hist[col]; // 0..31
        uint8_t p   = (uint8_t)(h >> 3);
        uint8_t b   = (uint8_t)(1u << (h & 7));
        frame[p * PANEL_WIDTH + (PANEL_WIDTH - 1 - i)] |= b;
    }
}

static void draw_ship(uint8_t *frame, uint8_t y_mid) {
    // Simple 6x9-ish ship (drawn across pages as needed) at the far right.
    // y_mid is 0..31, we clamp to keep ship on-screen.
    int8_t y = (int8_t)y_mid - 4; // top of ship
    if (y < 0) y = 0;
    if (y > 31 - 9) y = 31 - 9;

    // 6 columns of ship mask, top-to-bottom (bits set = pixels on this column)
    // Coarse ASCII rocket shape; tweak for style as needed.
    static const uint16_t colmask[6] = {
        0b000110000, //   **
        0b001111000, //  ****
        0b011111100, // ******
        0b001111000, //  ****
        0b000110000, //   **
        0b000010000  //    * (nose)
    };

    for (uint8_t c = 0; c < 6; c++) {
        int x = PANEL_WIDTH - 1 - c;
        uint16_t mask = colmask[c];
        for (uint8_t dy = 0; dy < 9; dy++) {
            if (mask & (1u << dy)) {
                uint8_t yy = (uint8_t)(y + dy);
                uint8_t p  = (uint8_t)(yy >> 3);
                uint8_t b  = (uint8_t)(1u << (yy & 7));
                frame[p * PANEL_WIDTH + x] |= b;
            }
        }
    }

    // Thruster flicker below ship center based on timer
    if ((timer_read32() >> 6) & 1u) {
        int tx = PANEL_WIDTH - 3;
        int ty = y + 9;
        if (ty < 32) {
            uint8_t p = (uint8_t)(ty >> 3);
            uint8_t b = (uint8_t)(1u << (ty & 7));
            frame[p * PANEL_WIDTH + tx] |= b;
        }
    }
}

void chased_render_spaceship_wpm_fullscreen(void) {
    // Update history at fixed cadence
    if (timer_elapsed32(frame_timer) > FRAME_MS) {
        frame_timer = timer_read32();
        // Scroll history
        wpm_hist_idx = (uint8_t)((wpm_hist_idx + 1) & (PANEL_WIDTH - 1));
        wpm_hist[wpm_hist_idx] = scale_wpm_to_32((uint8_t)get_current_wpm());
    }

    // Build frame buffer
    uint8_t frame[PANEL_PAGES * PANEL_WIDTH] = {0};
    draw_starfield(frame);
    draw_wpm_trace(frame);

    // Ship follows most recent WPM
    draw_ship(frame, wpm_hist[wpm_hist_idx]);

    // Flush to OLED (paged)
    for (uint8_t p = 0; p < PANEL_PAGES; p++) {
        oled_set_cursor(0, p);
        oled_write_raw((const char *)(frame + p * PANEL_WIDTH), PANEL_WIDTH);
    }
}

#endif // CHASED_OLED_RIGHT_SPACESHIP

