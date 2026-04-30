# Nixie Tube Driver — Bill of Materials



## Active Components

|Ref  |Description                |Value / Part      |Qty|Package  |Notes           |
|-----|---------------------------|------------------|---|---------|----------------|
|U1   |HV DC-DC Boost Module      |MAX1771         |1  |Module   |5V in → 170V out|
|U2–U5|BCD-to-Decimal Nixie Driver|SN74141 or K155ID1|4  |DIP-16   |One per tube    |
|MCU  |Microcontroller Board      |STM32 Discovery   |1  |Dev board|Already owned   |

-----

## Passive Components

|Ref  |Description                    |Value     |Qty|Package                  |Notes                                                                      |
|-----|-------------------------------|----------|---|-------------------------|---------------------------------------------------------------------------|
|R1–R4|Anode current-limiting resistor|12kΩ, 1W  |4  |Through-hole or 2512 SMD |1W rating for power dissipation at 170V. One per tube anode.|
|C1–C4|VCC decoupling cap per SN74141 |100nF, 50V|4  |0805 SMD or through-hole |Place as close to each IC VCC pin as possible                              |
|C7   |HV output decoupling           |10µF, 250V|1  |Electrolytic, rated >200V|On 170V rail, near anode resistors                                         |

-----

## Connectors 

|Ref      |Description        |Part                     |Qty|Notes                                       |
|---------|-------------------|-------------------------|---|--------------------------------------------|
|J1       |USB input connector|USB  Micro-B    |1  |5V power input to PCB                       |
|J2       |STM32 GPIO header  |2.54mm pin header        |1  |16-pin minimum (4 pins × 4 tubes)           |
|J3       |HV test point      |2-pin header or TP pad   |1  |For bench verification of 170V rail         |
|J4       |HV input screw terminal | 1x2 3.5mm screw terminal | 1 | |
|J5-J23 | Pin sockets| 1x5 and 1x2 2.54mm pin sockets| 8 ea. | Sockets for nixie tubes| 
