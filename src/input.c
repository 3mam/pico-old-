#include "input.h"

void input_init(void) {
  memset(keys, 0, 512);
  memset(press, 1, 512);
}

ui8 key_press(i32 id) {
  /*
if (keys[id] && press[id]) {
          press[id] = 0;
          return 1;
  } else if (!keys[id] && !press[id]) {
          press[id] = 1;
  }
  return 0;
*/
  return keys[id];
}
