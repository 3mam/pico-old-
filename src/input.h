#ifndef INPUT_H
#define INPUT_H
#include "macro.h"
#include <SDL2/SDL.h>
ui8 keys[512];
ui8 press[512];
void input_init(void);
ui8 key_press(i32 id);
#endif // INPUT_H
