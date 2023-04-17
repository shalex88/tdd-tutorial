#include <iostream>

/* TODO:
 * - The LED driver controls 16 two-state LEDs
 * - The driver can turn on or off any individual LED without affecting the others
 * - The driver can turn all LEDs on or off with a single interface call
 * - The user of the driver can query the state of any LED
 * - At power-on, the hardware default is for LEDs to be latched on. They must be turned off by the software
 * - LEDs are memory-mapped to a 16-bit word (at an address to be determined)
 * - A 1 in a bit position lights the corresponding LED; 0 turns it off
 * - The least significant bit corresponds to LED 1; the most significant bit corresponds to LED 16
 */

 int main() {
  std::cout << "TDD tutorial" << std::endl;

  return 0;
}
