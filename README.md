#develop MSP430G2553

1. Setting up the toolchain

Download and install CCS Toolchain at [here](http://processors.wiki.ti.com/index.php/Download_CCS)

2. Project Structure  

app - the application layer - contain  the specific user application
bootloader - is small code to inject binary down the microcontroller
driver - contain full peripheral of MSP430: SRAM, Flash, GPIO, TIMER, Watchdog, UART, I2C, SPI 
hal - Hardware Abstract layer provide interface to adaper the other module with MSP430
utils - some useful functions such as crc, ring buffer.

3. Build

Import project into CSS toolchain
Click Run to build it.