# CS5780 Final Project
# Nixie Tube Sound Meter

## Rohan Vittal, Will Metz, Adam Losser, and Matt Shaw 

This project is a sound meter using Nixie tubes for the display and an STM32 Discovery board to handle input processing. A custom PCB is used to both power and drive inputs to the tubes. It uses 16 pins for BCD input, and a К155ИД1 IC to convert to decimal. 

See [BOM](./BOM.md) for a detailed list of components. 

[mic_test](./mic_test/) contains the code that runs on the STM32. To flash, connect the STM32 discovery to USB, and use the cmake command

```bash
cmake --build build --target mic_test
```

[fixedSN74141N](./fixedSN74141N/) is a converted KiCAD footprint for the TI SN74141N IC (Identical to the Soviet К155ИД1). 

[nixie_driver](./nixie_driver/) is the KiCAD project for our custom PCB. It takes a 170VDC and 5VDC input, as well as 16 GPIOs for BCD number input. Note that the high voltage and 5V side need to share a common grounding. 
