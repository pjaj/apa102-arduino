/* This example shows how to display a moving rainbow pattern on
   an APA102-based LED strip. */

/* By default, the APA102 library uses pinMode and digitalWrite
   to write to the LEDs, which works on all Arduino-compatible
   boards but might be slow.  If you have a board supported by
   the FastGPIO library and want faster LED updates, then install
   the FastGPIO library and uncomment the next two lines: */
//#include <FastGPIO.h>
//#define APA102_USE_FAST_GPIO

#include <APA102.h>

// Define which pins to use.
const uint8_t dataPin = 11;
const uint8_t clockPin = 12;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 144;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[ledCount];

// Set the brightness to use (the maximum is 31).
const uint8_t brightness = 1;
// Create red, green and blue rainbow arrays to be filled once during setup phase
uint8_t rr[ledCount], rg[ledCount], rb[ledCount];
// Set the number of complete rainbows to run along strip
uint8_t numbows = 1;

void setup()
{
  // Set numleds, the number of LEDs in each section of a single rainbow.
  // There are 6 sections to each rainbow.
  // ramp up from 0 to 255
  // hold at 255 for two sections
  // ramp down to 0
  // hold at 0 for two sections.
  // each of the 3 colours, r,g & b, perform these cycles, but out of step with eachother by 2 sections
  uint8_t numleds = ledCount / (6 * numbows);
  // Fill the rr,rrg &rb arays.
  for (uint32_t i = 0; i < ledCount; i++) {

    // Calculate the brightness level of a colour during the ramp up phase.
    // (i % numleds) is the index into the section, 11 scales it into the range 0-255 when further multiplied by numbows.
    uint32_t t = (i % numleds) * 11 * numbows;
    // Calculate which of the 6 sections we are in
    uint32_t j = (i / numleds) % 6;
    // q is the corresponding colour value during ramp down.
    uint32_t q = 255 - t;

    // Set the contents of the 3 colour arrays by section
    // Red starts at 255 for section (0), then ramps down to 0 over the next(1), it holds at 0 for two sections(2 & 3) then ramps up to 255 in section (4) finally holds at 255 in section (5)
    // Green ramps up from 0 to 255 over section (0), holds at 255 for 2 sections (1 & 2), ramps down to 0 in section (3) and holds at 0 for 2 sections (4 & 5)
    // Blue holds at 0 for 2 sections (0 & 1), ramps up over section (2), holds at 255 for 2 sections (3 & 4) then ramps down for section (5)
    switch (j) {
      case 0: rr[i] = 255; rg[i] = t; rb[i] = 0; break;
      case 1: rr[i] = q; rg[i] = 255; rb[i] = 0; break;
      case 2: rr[i] = 0; rg[i] = 255; rb[i] = t; break;
      case 3: rr[i] = 0; rg[i] = q; rb[i] = 255; break;
      case 4: rr[i] = t; rg[i] = 0; rb[i] = 255; break;
      case 5: rr[i] = 255; rg[i] = 0; rb[i] = q; break;
    }
  }
}

void Rainbow(uint32_t loops, uint16_t delayms) {
  rgb_color colors[ledCount];
  // Set the brightness
  uint8_t bpos = 1;

  // shift is how far down the LED strip to move the start of the rainbow, it increments once every pass.
  for (uint32_t shift = 0; shift < loops; shift++) {

    // Copy the alreadt calculated colours into the colour array for driving the LED strip.
    // The position to copy will shift by 1 every time round the loop, but wrapping round at the end of the count to fill the beginning of the array
    for (uint32_t i = 0; i < ledCount; i++) {
      colors[(i + shift) % ledCount] = rgb_color(rr[i], rg[i], rb[i]);
    }

    ledStrip.write(colors, ledCount, bpos);
    delay(delayms);
  }
}

void loop()
{ 
  Rainbow(144, 0);
}
