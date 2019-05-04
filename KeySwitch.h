#ifndef __H_KEY_SWITCH__
#define __H_KEY_SWITCH__

class KeySwitch {
    public:
        KeySwitch(uint8_t pin, bool invert=false);

        void begin(void);
        void update(void);

        bool isKeyDown(void) const;
        bool isKeyUp(void) const;
        bool isPressed(void) const;
        bool isKeyHeld(void) const;

        void onKeyDown(void (*callback)());
        void onKeyUp(void (*callback)());
        void onPressed(void (*callback)());
        void onKeyHeld(void (*callback)(), unsigned long hold_time);

    private:
        uint8_t _pin_mask;
        volatile uint8_t *_pin_ddr;
        volatile uint8_t *_pin_out_reg;
        volatile uint8_t *_pin_inp_reg;
        void (*_cb_keydown)();
        void (*_cb_keyup)();
        void (*_cb_pressed)();
        void (*_cb_keyheld)();
        bool _invert;
        bool _state;
        bool _lastState;
        bool _held;
        bool _enabled;
        unsigned long _lastTimeout;
        unsigned long _hold_time;
};

#endif //__H_KEY_SWITCH__
