# rpi4-play
Testing programs on raspberry pi4 running Ubuntu

uart - is a simple I/O program that uses MMIO to UART and GPIO to perform I/O. This program access peripheral registers mapped to RAM using mmap() to map to /dev/mem

beacon-bluez - try to implement  Eddystone beacon based on rpi4-osdev project but using bluez C libraries instead of MMIO/GPIO calls as done in rpi4-osdev project
