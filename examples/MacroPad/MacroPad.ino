#include <KeySwitch.h>
#include <TrinketHidCombo.h>

/*
 * KeySwitch - JavaScript-like button and switch callback binding for Arduino
 * 2019 Ian McLinden <https://github.com/ianmclinden>
 *
 * Basic 2 key macro keypad that copies and pastes. 
 * 
 * Requires TrinketHidCombo from Adafruit:
 * https://github.com/adafruit/Adafruit-Trinket-USB
 * 
 * Updated - 4 May 2019
 * 
 */

#define SW0_PIN (PB0)
#define SW1_PIN (PB1)

// If we're on OSX we want to use Cmd+C, Cmd+V
// Otherwise we want to use Ctrl+C,Ctrl+V (comment out the define)
#define __APPLE__

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

void setup() {
    // Begin HID
    TrinketHidCombo.begin();
    
    // Bind our callbacks
    // onKeyDown( void (*callback)() ) - Triggers as soon as
    // the switch or button is pulled to the active state
    sw0.onKeyDown([]{
        #ifdef __APPLE__
            TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_GUI, KEYCODE_C);
        #else
            TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_CONTROL, KEYCODE_C);
        #endif
    });
    sw1.onKeyDown([]{
        #ifdef __APPLE__
            TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_GUI, KEYCODE_V);
        #else
            TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_CONTROL, KEYCODE_V);
        #endif
    });

    // onKeyUp( void (*callback)() ) - Triggers as soon as
    // the switch or button is released from the active state
    sw1.onKeyUp([]{
        // Release the key
        TrinketHidCombo.pressKey(0, 0);
    });
    sw0.onKeyUp([]{
        // Release the key
        TrinketHidCombo.pressKey(0, 0);
    });

    // Start the switch handlers
    sw0.begin();
    sw1.begin();
}

void loop() {
    // poll() or print() Must be called every 10ms or USB may fail
    TrinketHidCombo.poll();
    
    // Update the switches.
    sw0.update();
    sw1.update();

    // Nothing here should delay() or switch handling may break
}
