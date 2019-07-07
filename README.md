# Develop MSP430G2553 KIT

## Prerequesite

#### Setting up the environent

- Download and install [Code Composer Studio Toolchain](http://processors.wiki.ti.com/index.php/Download_CCS)

- Pull project on [the repository](https://github.com/Truongtx91/msp430)

- Import project into workspace: File->Import->Code Composer Studio

## Project Structure  

1. App - which define  interface for user application
	- Auto Car
		1. detect obstacles and measure the distance, then display value on the 7-Segment LED.
		2. Drive by the Application on smartphone via the bluetooth such as turn left, right, up, down ...
		3. Change motor speed by H-Bridge circuit.
	- LED wifi
		1. Display data on the led matrix
		2. Control display by the WEB UI via ESP8266
		3. Remote Control via LoRa.	
	- PID Algorithm
2. Bootloader - which is small boot program use to trigger and flash binary data into the microcontroller
3. Driver - has role enable peripheral module of MSP430 such as SRAM, Flash, GPIO, 
	TIMER, Watchdog, PWM, UART, I2C, SPI.
4. HAL(Hardware Abstract Layer) - provide interface to connect between the other module and MSP430
5. Utils - some useful general functions such as CRC, Ring Buffer.
