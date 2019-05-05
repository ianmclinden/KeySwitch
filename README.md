# KeySwitch
JavaScript-like button and switch callback binding for Arduino

# Installation <sup>[1](#references)</sup>
From the [KeySwitch](https://github.com/ianmclinden/KeySwitch) repo, select `Clone or download > Download ZIP`, or use the [direct download link](https://github.com/ianmclinden/KeySwitch/archive/master.zip).

In the Arduino IDE, navigate to `Sketch > Include Library > Add .ZIP Library`. At the top of the drop down list, select the option to `Add .ZIP Library`.  

Return to the `Sketch > Include Library` menu. You should now see the library at the bottom of the drop-down menu. It is ready to be used in your sketch. The zip file will have been expanded in the libraries folder in your Arduino sketches directory.


# Example Code
Two code examples are provided: 

### [OnKey](https://github.com/ianmclinden/KeySwitch/blob/master/examples/OnKey/OnKey.ino)
Demonstrates setting simple button bindings using pre-defined void->void functions.

### [Lambda](https://github.com/ianmclinden/KeySwitch/blob/master/examples/Lambda/Lambda.ino)
Demonstrates use of C++ lamabdas for cleaner callback handling.

### [MacroPad](https://github.com/ianmclinden/KeySwitch/blob/master/examples/MacroPad/MacroPad.ino)
Create a 2-Key macro keypad that copies and pastes.
Note: Given example uses the TrinketKeyboard library for the ATtiny85 (Digispark, etc), but other keyboard libraries can be used as well.

### [LEDCycle](https://github.com/ianmclinden/KeySwitch/blob/master/examples/LEDCycle/LEDCycle.ino)
Re-implement of [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) butoncycler example.


# License and Author
2019 Ian McLinden under the [GNU General Public License v3.0](/LICENSE).
Use at your own risk, author retains no liability for misuse.

---
# References
*1) Installation instructions provided in part from the official [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries) guide.*
*2) Library format based in part on the [Arduino Libraries](https://www.arduino.cc/en/hacking/libraries) documentation, and on the [Arduino Test Library Implementation](https://www.arduino.cc/en/uploads/Hacking/Test.zip)*
