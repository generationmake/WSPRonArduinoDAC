# WSPRonArduinoDAC
[![Compile Sketch status](https://github.com/generationmake/WSPRonArduinoDAC/workflows/Compile%20Sketch/badge.svg)](https://github.com/generationmake/WSPRonArduinoDAC/actions?workflow=Compile+Sketch)

generating WSPR signals with the DAC on Arduino SAMD boards

## used hardware

any Arduino board with an Atmel SAMD micocontroller

## used libraries

  * JTEncode https://github.com/etherkit/JTEncode
  * SAMDTimerInterrupt https://github.com/khoih-prog/SAMD_TimerInterrupt (version 1.5 or newer)

## pin usage

only the DAC pin of the Arduino is used, typically pin A0

