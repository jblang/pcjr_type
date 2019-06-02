## PCjr Keyboard Emulation for Arduino

This library emulates a PCjr keyboard using an Arduino.  This allows you to script control of your PCjr or paste text from another computer to the PCjr over a serial terminal.  Because it's just an infrared link, it's non-invasive and has no risk of damaging your PCjr.

The only hardware required is an infrared LED and a 100 ohm resistor.  The LED should be connected through the 100 ohm resistor to pin 3 on the Arduino, and to ground.  

The PCjr can receive about 100 keypresses per second over IR. Shifted characters are sent at half that speed because the shift key is pressed and released once for each character.  It would be possible to optimize by detecting consecutive shifted characters and only pressing the shift key once before for all the characters and releasing it after, but I haven't done that yet.

The library itself is in pcjr_type.h and the example sketch that receives characters over serial and sends them over IR is in pcjr_type.ino.  The sketch will run a self-test at startup that types all the printable characters.  This can be commented out if
desired.

## Library Documentation

The following is only for users wishing to write ther own sketches using the library. It is not necessary to understand this to use the provided sketch.

- `pcjrInit` must be called in your setup() function before using any other functions.
- `typeAscii` will send any ASCII character that can be represented by a keyboard sequence. Because only typable characters can be
sent, it means that binary data cannot be sent to the PCjr without first translating it to hex or uuencode.
- `typeString` will send a complete string of ASCII characters.
- `TYPE` will press and release the specified key.
- `PRESS` will press and hold the specified key. This can be used for modifier keys CTRL, SHIFT, ALT, and FN before sending the  shifted keys using `TYPE`.
- `RELEASE` will release the specified key.
- `KEY_*` macros are defined for each of the keys on the PCjr keyboard.  For letter and number keys, `KEY_LETTER()` and `KEY_NUMBER()` macros are provided instead of individual macros for each key.
- Look at the `typeAscii` function for an example of how to use TYPE, PRESS, RELEASE, and the KEY_ macros.

## License

Copyright 2019 J.B. Langston

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.