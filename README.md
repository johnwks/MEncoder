# MEncoder

Arduino library to work with the AS5600 magnetic hall sensor PCB to function as a rotary encoder or switch.

## Installation

Create a new directory (name it MEncoder) in your Arduino library path (usually Documents\Arduino\libraries in Windows) and copy the .cpp and .h files there.

## Usage

Include MEncoder.h at the top of your Arduino sketch.

```
#include "MEncoder.h"
```

Create object instances for each encoder.

```
MEncoder encoder(PIN_ENCODER1, 12, 1023);
```

PIN_ENCODER is the Arduino pin which is connected to the analog output of the AS5600 PCB.

The value 12 is for the number of positions the encoder supports. The included STEP files are for a 12 position encoder.

The value 1023 is the maximum value of the analog pin output. This is the default for the AS5600 in analog mode. It goes from 0 to 1023 (i.e. 10 bits).

Initialize the encoder in ```setup()```.

```
void setup()
{
    encoder.init();
}
```

This will have the effect of setting the current encoder position as position 0. This works well as an incremental rotary encoder as all it needs to do is to detect clockwise or counter-clockwise rotation. See below on how to make it function like a rotary switch.

In the ```loop()``` function, poll the encoder to determine if there has been a clockwise or counter-clockwise rotation.

```
void loop()
{
  ..
  encoder.process();
  Joystick.setButton(BUTTON_NUM, 0);
  Joystick.setButton(BUTTON_NUM + 1, 0);
  if (encoder.state == ME_CW) {
    Joystick.setButton(BUTTON_NUM, 1);
  }
  if (encoder.state == ME_CCW) {
    Joystick.setButton(BUTTON_NUM + 1, 1);
  }
  encoder.resetState();
  ..

}
```

In this example, a clockwise rotation will trigger BUTTON_NUM and a counter-clockwise rotation will trigger BUTTON_NUM + 1.

In order to make the encoder function like a rotary switch, we need to find out the analog value at the intended position 0.

```
  base = encoder.readVal();
```

Once we have the base value, we initialize the encoder with this value.

```
  encoder.init(base);
```

This will set ```base``` as position 0 as opposed to whatever the current analog reading.


