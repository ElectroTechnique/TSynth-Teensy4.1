![](https://electrotechnique.github.io/etlogo.png)

# Teensy 4.1 based synthesizer using PJRC Audio Board and Audio Lib

Website: [electrotechnique.cc](https://electrotechnique.cc)

The pcb and front panel to build TSynth are available from [Tindie](https://www.tindie.com/products/electrotechnique/tsynth-teensy-based-diy-programmable-poly-synth/) with SMD 4067 multiplexers, 6N137 opto-isolator, capacitors and resistors fitted. The entire cost of parts to build TSynth will be around $99 if you buy components from the cheaper suppliers and the build time around two hours to solder. Plans for a 3D printed/laser cut enclosure are also available.
Questions: info@electrotechnique.cc

# Preset Patches
Format your SD card using [the SD Association formatter](https://www.sdcard.org/downloads/formatter/). Copy all the presets straight on to the card with no other files or folders.

# Instructions

The source code **requires** at least Teensyduino 1.54 from [PJRC](https://pjrc.com) to compile. You also need CircularBuffer from Agileware and Adafruit_GFX, which are available in the Arduino Library Manager. **NOTE** if using Teensyduino 1.55, you'll need to [remove a line from TeensyThreads.cpp](https://forum.pjrc.com/threads/68192-Teensyduino-1-55-Released)


See this thread if you get an error concerning utils/debug.h:  [https://forum.pjrc.com/threads/68192-Teensyduino-1-55-Released?highlight=utils%2Fdebug.h}(https://forum.pjrc.com/threads/68192-Teensyduino-1-55-Released?highlight=utils%2Fdebug.h)
