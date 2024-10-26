# Low Cost ISO bus capability DEV Board  & Firmware Support

## Description

In this project, I attempted to design hardware that could be used with the ISO 11783 (ISOBUS) protocol, a CAN-based communication protocol used in agricultural machinery, or a subset of it. I aimed to integrate this design into the Nuvoton ecosystem, with which I am familiar with.


!(https://github.com/ihm3/ISO_VT/board.PNG)

## Features

- Parallel RGB LCD supports for ISOBus Virtual Terminal with AEC-Q100 Qualified video engine [BT817-AQ](https://brtchip.com/product/bt817aq/) up to 1280 x 800 res.
- RS232 connector fits to PEAKcan
- Cortex -M23 based uC 64 MHz & OTA capability
- SD card Support to record iop files
- USB connection for debug
- AgOpen ISObus Stack Support
- WiFi Support with ESP-12 both RX-TX & SPI for faster download DDOP
- IO pins for users


!(https://github.com/ihm3/ISO_VT/board.PNG)


## Coming Soon

- Adding more detailed README.md file or PDF document to include safe power for Automobiles & Trucs with try not to refer ISOBus Hardware Layer
- Adding Clear interrup capability to AgOpen Stack
- Adding LVGL port for nuvoton