#include "Arduino.h"
#include "KeySwitch.h"

// ---- Constructors ----------------------------------------------------------
KeySwitch::KeySwitch(uint8_t pin, bool invert) {

    /*
     * Pin Mask: Used to select a pin inside a register, e.g. (1<<5)
     * Port DDR: Used to select pin direction - 0:IN, 1:OUT
     * Port Output Register: Used to output to the register, or
     *     if DDR is set to 0:IN, used to set the pullup resistor,
     *     0:NO_PULLUP, 1:PULLUP
     * Port Input Register: Used to read from the register
     */

    _pin_mask  = digitalPinToBitMask(pin);
    _pin_ddr = portModeRegister(digitalPinToPort(pin));
    _pin_out_reg = portOutputRegister(digitalPinToPort(pin));
    _pin_inp_reg = portInputRegister(digitalPinToPort(pin));

    _cb_keydown = NULL;
    _cb_keyup = NULL;
    _cb_pressed = NULL;
    _cb_keyheld = NULL;
    _invert = invert;
    _held = false;
    _lastState = false;
    _enabled = false;
    _lastTimeout = 0;
    _hold_time = 0;
}

// ---- Begin -----------------------------------------------------------------
void KeySwitch::begin(void) {
    // Enable pin output via DDR
    *_pin_ddr&=~_pin_mask;

    // Enable Pullup resistor if necessary
    if (_invert) {
        *_pin_out_reg |= _pin_mask;
    }

    // Read pin state
    _state = (*_pin_inp_reg&_pin_mask);
    _state = _invert ? !_state : _state;
    _lastState = _state;

    // Enable so that callbacks are available
    _enabled = true;
}

// ---- Update ----------------------------------------------------------------
void KeySwitch::update(void) {
    // Don't bother processing if update() hasn't been called
    if (!_enabled) {
        return;
    }

    // Read pin state, invert if necessary
    bool _rd = (*_pin_inp_reg&_pin_mask);
    _rd = _invert ? !_rd : _rd;

	// If the state has changed, reset the timer to start debouncing
    if (_rd != _lastState) {
        _lastTimeout = (uint16_t)millis();
    }

    // If now is 20UL ms > the debounce timer, start processing
    if ((uint16_t)(millis() - _lastTimeout) >= 25 ) {

        // If the state has changed handle onKeyDown && onKeyUp
        if (_rd != _state) {
            _state = _rd;

            // If the switch is enabled, this toggle was to on
            if (_state == true && _cb_keydown != NULL) {
                _cb_keydown();
            }

            // If the switch is not enabled, this toggle was to off
            if (_state == false && _cb_keyup != NULL) {
                _held = false;
                _cb_keyup();
            }
        }

        // Otherwise, every interval handle onPressed
        if (_state == true && _cb_pressed != NULL) {
            _cb_pressed();
        }

        // Also (i.e. not toggled) check if held has been down for > hold_time
        if ((uint16_t)(millis() - _lastTimeout) >= _hold_time) {

            // If the held state has changed, handle onHeld
            if (_rd != _held) {
                _held = _rd;

                // If the switch has been on, this toggle was to held
                if (_held == true && _cb_keyheld != NULL) {
                    _cb_keyheld();
                }
            }
        }
    }

    // Save the state for next update
    _lastState = _rd;
}

// ---- Accessors -------------------------------------------------------------
bool KeySwitch::isKeyDown(void) const {
    return (bool) _state;
}
bool KeySwitch::isKeyUp(void) const {
    return (bool) !_state;
}
bool KeySwitch::isPressed(void) const {
    return (bool) _state;
}
bool KeySwitch::isKeyHeld(void) const {
    return (bool) (_held);
}

// ---- Press Handlers --------------------------------------------------------
void KeySwitch::onKeyDown(void (*callback)()) {
    _cb_keydown = callback;
}
void KeySwitch::onKeyUp(void (*callback)()) {
    _cb_keyup = callback;
}
void KeySwitch::onPressed(void (*callback)()){
    _cb_pressed = callback;
}
void KeySwitch::onKeyHeld(void (*callback)(), uint16_t hold_time) {
    _cb_keyheld = callback;
    _hold_time = hold_time;
}
