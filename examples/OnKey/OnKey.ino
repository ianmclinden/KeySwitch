#include <KeySwitch.h>

/*
 * KeySwitch - JavaScript-like button and switch callback binding for Arduino
 * 2019 Ian McLinden <https://github.com/ianmclinden>
 *
 * Use KeySwitch.on{KeyDown,KeyUp} to bind callbacks
 * Updated - 4 May 2019
 * 
 */

#define SW0_PIN (PB0)
#define SW1_PIN (PB1)

/*
 * Create a couple keys and bind them to digital pins
 * 
 * KeySwitch(uint8_t pin, bool invert)
 *   pin:    Arduino pin number or alias, e.g. 11 or PB0
 *   invert: true  - If the switch is pulled to GND when pressed. 
 *                   Uses INPUT_PULLUP
 *           false - If the switch is pulled to VCC when pressed.
 *                   Uses INPUT
 */

KeySwitch sw0(SW0_PIN, true);
KeySwitch sw1(SW1_PIN, true);

/*
 * Create some void->void callback functions to bind to key changes
 */
void sw0keyDown(void) {
    Serial.println(F("Switch 0 Pressed"));
}
void sw0keyUp(void) {
    Serial.println(F("Switch 0 Released"));
}
void sw1keyDown(void) {
    Serial.println(F("Switch 1 Pressed"));
}
void sw1keyUp(void) {
    Serial.println(F("Switch 1 Released"));
}
//void keyIsPressed(void) {
//    Serial.println(F("A key is pressed"));
//}
//void keyIsHeld(void) {
//    Serial.println(F("A key was held"));
//}

void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    // Bind our callbacks
    // onKeyDown( void (*callback)() ) - Triggers as soon as
    // the switch or button is pulled to the active state
    sw0.onKeyDown( &sw0keyDown );
    sw1.onKeyDown( &sw1keyDown );

    // onKeyUp( void (*callback)() ) - Triggers as soon as
    // the switch or button is released from the active state
    sw1.onKeyUp( &sw1keyUp );
    sw0.onKeyUp( &sw0keyUp );

    // Two other callbacks are available:
    // onPressed( void (*callback)() ) - Happens continuously 
    // if switch or button is in the active state
    //sw0.onPressed( &keyIsPressed );
    //sw1.onPressed( &keyIsPressed );

    // onKeyHeld( void (*callback)(), unsigned long hold_time )
    // Happens after hold_time milliseconds of the switch or
    // button being held
    //sw0.keyIsHeld( &keyIsPressed, 5000UL );
    //sw1.keyIsHeld( &keyIsPressed, 5000UL );

    // Start the switch handlers
    sw0.begin();
    sw1.begin();
}

void loop() {
    // Update the switches.
    sw0.update();
    sw1.update();

    // Nothing here should delay() or switch handling may break
}
