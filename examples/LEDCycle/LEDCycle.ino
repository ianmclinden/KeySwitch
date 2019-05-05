#include <Adafruit_NeoPixel.h>
#include <KeySwitch.h>

/*
 * KeySwitch - JavaScript-like button and switch callback binding for Arduino
 * 2019 Ian McLinden <https://github.com/ianmclinden>
 * 
 * Cycle NeoPixels with button presses
 * Updated - 4 May 2019
 * 
 * NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
 * 
 * Modification of the Adafruit NeoPixel buttoncyler code. The original code uses
 * delay() to time animations. This means that while animations are running the 
 * KeySwitch will not be processing inputs.
 * 
 * From the Adafruit example:
 * 
 * This is a demonstration on how to use an input device to trigger changes on your neo pixels.
 * You should wire a momentary push button to connect from ground to a digital IO pin.  When you
 * press the button it will change to a new pixel animation.  Note that you need to press the
 * button once to start the first animation!
 */

#define LED_PIN (PB2)
#define SW0_PIN (PB0)
#define NUM_PIXELS (24)

// Create a NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

/*
 * Create a key and bind it to a digital pin
 * 
 * KeySwitch(uint8_t pin, bool invert)
 *   pin:    Arduino pin number or alias, e.g. 11 or PB0
 *   invert: true  - If the switch is pulled to GND when pressed. 
 *                   Uses INPUT_PULLUP
 *           false - If the switch is pulled to VCC when pressed.
 *                   Uses INPUT
 */
KeySwitch sw0(SW0_PIN, true);

// Track the current show
int showType = 0;

// Callback for running the show
void startShow(int i);

void setup() {

    // Bind our callback
    // onKeyDown( void (*callback)() ) - Triggers as soon as
    // the switch or button is pulled to the active state
    sw0.onKeyDown([]{
        // Increment the showtype (0-9)
        showType = (showType+1) % 10;
        // Run the show
        startShow(showType);
    });

    // Start the switch handler
    sw0.begin();

    // Star the NeoPixel strip and initialize all pixels to 'off'
    strip.begin();
    strip.show(); 
}

void loop() {
    // Update the switch
    sw0.update();
}

void startShow(int i) {
    switch(i){
        case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
            break;
        case 1: colorWipe(strip.Color(255, 0, 0), 50);  // Red
            break;
        case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
            break;
        case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
            break;
        case 4: theaterChase(strip.Color(127, 127, 127), 50); // White
            break;
        case 5: theaterChase(strip.Color(127,   0,   0), 50); // Red
        break;
        case 6: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
            break;
        case 7: rainbow(20);
            break;
        case 8: rainbowCycle(20);
            break;
        case 9: theaterChaseRainbow(50);
            break;
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;
    
    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i+j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
    uint16_t i, j;
    
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, c);    //turn every third pixel on
            }
            strip.show();
            
            delay(wait);
            
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
            }
            strip.show();
            
            delay(wait);
            
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
        if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}
