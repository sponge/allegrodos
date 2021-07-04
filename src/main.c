#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#include "allegro/platform/aldjgpp.h"
#include "allegro.h"

const char *argv0;

BITMAP * bmp(const char *name, PALETTE *pal) {
  char fname[256];
  replace_filename(fname, argv0, name, sizeof(fname));
  BITMAP *ret = load_bitmap(fname, (RGB*)pal);

  if (!ret) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("Error loading %s!\n", fname);
    abort();
  }

  return ret;
}

volatile int ticks = 0;
void ticker(void)
{
    ticks++;
}
END_OF_FUNCTION(ticker)

bool looping = true;
int selectedIdx = 0;
char lastKey[KEY_MAX];

int main(int argc, char **argv) {
  argv0 = argv[0];
  if (allegro_init() != 0) return -1;

  install_keyboard();
  install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
  install_timer();
  LOCK_FUNCTION(ticker);
  LOCK_VARIABLE(ticks);
  install_int_ex(ticker, BPS_TO_TIMER(60));

  if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("Unable to set 320x200\n%s\n", allegro_error);
    return -1;
  }

  PALETTE pal;
  BITMAP *bg = bmp("bg.bmp", &pal);
  set_palette(pal);

  BITMAP *selected = bmp("selected.bmp", NULL);

  const int maxItems = 6;

  do {
    vsync();
    rectfill(screen, 0, 0, 320, 200, 16);

    masked_blit(bg, screen, 0, 0, 0, 0, bg->w, bg->h);
    masked_blit(selected, screen, 0, 0, 84, selectedIdx * 17 + 57, selected->w, selected->w);

    if (keypressed()) {      
      if (!lastKey[KEY_UP] && key[KEY_UP]) {
        selectedIdx--;
        if (selectedIdx < 0) {
          selectedIdx = maxItems - 1;
        }
        
      } else if (!lastKey[KEY_DOWN] && key[KEY_DOWN]) {
        selectedIdx = (selectedIdx + 1) % maxItems;

      } else if (key[KEY_ENTER]) {
        return selectedIdx + 1;
        
      } else if (key[KEY_ESC]) {
        return 0;
        
      }
    }
    
    for (int i = 0; i < KEY_MAX; i++) {
      lastKey[i] = key[i];
    }
  } while (looping);

  return 0;
}