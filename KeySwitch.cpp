#include "Arduino.h"
#include "KeySwitch.h"

// ---- Constructors ----------------------------------------------------------
KeySwitch::KeySwitch(uint8_t pin, bool invert) {
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
    // Enable DDR
    *_pin_ddr&=~_pin_mask;

    // Enable Pullup resistor
    if (_invert)
        *_pin_out_reg |= _pin_mask;

    // Read pin state
    _state = (*_pin_inp_reg&_pin_mask);
    _state = _invert ? !_state : _state;
    _lastState = _state;
    _enabled = true;
}

// ---- Update ----------------------------------------------------------------
void KeySwitch::update(void) {
    if (!_enabled) return;

    // Read pin state
    bool _rd = (*_pin_inp_reg&_pin_mask);
    _rd = _invert ? !_rd : _rd;

    if (_rd != _lastState) {
        _lastTimeout = (uint16_t)millis();
    }

    if ((uint16_t)(millis() - _lastTimeout) >= 25 ) {

        // If the state has changed handle onKeyDown && onKeyUp
        if (_rd != _state) {
            _state = _rd;

            if (_state == true && _cb_keydown != NULL) {
                _cb_keydown();
            }
            if (_state == false && _cb_keyup != NULL) {
                _held = false;
                _cb_keyup();
            }
        }
        // Otherwise, every interval handle onPressed
        if (_state == true && _cb_pressed != NULL) {
                _cb_pressed();
        }

        if ((uint16_t)(millis() - _lastTimeout) >= _hold_time) {
            if (_rd != _held) {
                _held = _rd;

                if (_held == true && _cb_keyheld != NULL)
                    _cb_keyheld();
            }
        }
    }

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
