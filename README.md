# ECG_Kit
 Simple implementation of FIR Equiripple and ADC for an ECG device

Device used: ATSAMD10 - Xplained Mini
Programmed via SERCOM2, Virtual COM port Onboard Debugger
IDE: Atmel Studio 7.0, HAL Wizard

To Use download the Atmel Studio 7.0
Build Project
Plug in device to USB
Run without Debugging

PA03 is Analog to Digital Converter, Channel 1
PA02 is Digital to Analog Converter, writes the output of the FIR filter

FIR is set for 100 Hz sampling, filtering 45 Hz, very sharply, -80 dB, 64 points

USART is enabled for both TX and RX interrupt service routines, but the RX_interrupt is currently not coded

FIR Coefficients generated using MatLAB filter design tools.
ADC is Timer Driven at 1uS (1 Megasamples per second)
Actual sample rate unknown, but plausible at 100 Hz because the FIR is working :)
