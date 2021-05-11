// This testing program is base on:
// Adam Greenwood-Byrne (isometimes) with his project rpi4-osdev.

enum {
    PERIPHERAL_BASE = 0xFE000000,
    GPFSEL0         = 0x200000,
    GPSET0          = 0x20001C,
    GPCLR0          = 0x200028,
    GPPUPPDN0       = 0x2000E4,
    ARM_UART0_BASE	= 0x201000,
    ARM_UART0_DR	= ARM_UART0_BASE + 0x00,
    ARM_UART0_FR    = ARM_UART0_BASE + 0x18,
    ARM_UART0_IBRD  = ARM_UART0_BASE + 0x24,
    ARM_UART0_FBRD  = ARM_UART0_BASE + 0x28,
    ARM_UART0_LCRH  = ARM_UART0_BASE + 0x2C,
    ARM_UART0_CR    = ARM_UART0_BASE + 0x30,
    ARM_UART0_IFLS  = ARM_UART0_BASE + 0x34,
    ARM_UART0_IMSC  = ARM_UART0_BASE + 0x38,
    ARM_UART0_RIS   = ARM_UART0_BASE + 0x3C,
    ARM_UART0_MIS   = ARM_UART0_BASE + 0x40,
    ARM_UART0_ICR   = ARM_UART0_BASE + 0x44,
};

enum {
    GPIO_MAX_PIN       = 53,
    GPIO_FUNCTION_OUT  = 1,
    GPIO_FUNCTION_ALT5 = 2,
    GPIO_FUNCTION_ALT3 = 7
};

enum {
    Pull_None = 0,
    Pull_Down = 1, // Are down and up the right way around?
    Pull_Up = 2
};

int msleep(long msec);
void setup_io();

void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

void gpio_useAsAlt3(unsigned int pin_number);