# CS5780 Final Project
# Nixie Tube Sound Meter

## Rohan Vittal, Will Metz, Adam Losser, and Matt Shaw 

A nixie tube driver that displays real-time microphone loudness as a percentage (0–100) across four ZM1000 nixie tubes. Audio is sampled via an onboard microphone through the STM32's ADC, processed into a loudness percentage, and transmitted over UART to a PC while simultaneously driving the nixie display.

## System Overview:
The system consists of three physical boards:

- STM32F072 Discovery — microcontroller, ADC microphone input, UART output, GPIO drive
- Custom PCB — contains K155ID1 (К155ИД1) BCD driver ICs, HV power distribution, and connectors
- [HV Power Board](https://ebay.us/m/Hubveg) — MAX1771 boost converter (160–210V output), 5V logic input
See [BOM](./BOM.md) for a detailed list of components.

The STM32 extracts digits from the loudness percentage and writes 4-bit BCD values to GPIO pins. Each set of 4 GPIO pins drives one K155ID1 BCD decoder IC, which selects the corresponding cathode on each ZM1000 tube. The anode of each tube is supplied with ~170V.

## Software: 
[mic_test](./mic_test/) contains the code that runs on the STM32. To flash, connect the STM32 discovery to USB, and use the cmake command

```bash
cmake --build build --target mic_test
```
### Software Structure
| File | Description |
|---|---|
| `main.c` | Main loop — ADC sampling, smoothing, UART transmit, nixie write |
| `nixie_drivers.c` | Digit extraction, BCD remapping, GPIO write |
| `adc_mic.c` | ADC initialisation and peak-to-peak measurement |
| `uart.c` | UART3 initialisation and byte transmit |
| `board.c` | SysTick, delay, LED bar (unused) |
| `stm32f0xx_it.c` | SysTick interrupt handler |

### GPIO Pin mappings: 
| Tube | MSB | — | — | LSB |
|---|---|---|---|---|
| Tube 4 (hundreds) | PC9 | PC8 | PC7 | PC6 |
| Tube 3 (tens) | PA11 | PA10 | PA9 | PA8 |
| Tube 2 (ones) | PB11 | PB10 | PB9 | PB8 |
| Tube 1 (tenths) | PB7 | PB6 | PB5 | PB4 |

### BCD Remapping

Due to physical wiring errors on the PCB, a lookup table in firmware remaps intended digits to the correct GPIO pattern:

```c
static const uint8_t digit_map[10] = {
    0b1101, // 0
    0b0001, // 1
    0b1001, // 2
    0b0000, // 3
    0b1110, // 4
    0b0110, // 5
    0b1010, // 6
    0b0010, // 7
    0b1000, // 8
    0b0100  // 9
};
```

## Hardware Notes

- **HV polarity** — the eBay HV power board has reversed polarity output terminals. Verify with a multimeter before connecting to the PCB.
- **HV and logic ground** — must be tied to a common ground, either through the screw terminal or using a breadboard. Separate grounds will prevent cathode current return and the tube will not display any digit.
- **Primer pin** — the ZM1000 primer pin is left floating. A 10MΩ resistor to the anode is recommended per the datasheet for strobed applications but is not required for basic DC operation.
- **USB power** — the micro-USB connector footprint on the PCB was incorrect. It is more reliable to attach 5V power directly by soldering a wire or header on in lieu of the microUSB. 

[fixedSN74141N](./fixedSN74141N/) is a converted KiCAD footprint for the TI SN74141N IC (Identical to the Soviet К155ИД1). 

[nixie_driver](./nixie_driver/) is the KiCAD project for our custom PCB. It takes a 170VDC and 5VDC input, as well as 16 GPIOs for BCD number input. Note that the high voltage and 5V side need to share a common grounding. 
