/*
* PicoROM
* Simulate a ROM chip (e.g. 28c256) with a Raspberry Pi Pico.
* Nick Bild (nick.bild@gmail.com)
* August 2021
*
* Updates: Kevin Dec 2025
*
*/

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "pin_definitions.h"
#include <stdlib.h>

#define ROM_SIZE 0x1000
#define IMG_NUM  4
#define IMG_SIZE 672  // 64*84/8
extern const uint8_t rom_contents[ROM_SIZE];
extern const uint8_t rom_images[IMG_NUM][IMG_SIZE];

#include "pico_rom_contents.h"
#include "pico_rom_images.h"

void setup_gpio();
int get_requested_address();
void put_data_on_bus(int);

uint16_t img_pos;
uint8_t  img_idx;
uint16_t last_address;
uint16_t frame_counter = 0;
uint8_t  rom_img;
uint8_t  first_read;

int main() {
    // Set system clock speed.
    // 400 MHz
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_pll(1600000000, 4, 1);
    
    // GPIO setup.
    setup_gpio();

    img_pos = 0;
    img_idx = 0;
	last_address = 0;
	frame_counter = 0;
    rom_img = rom_images[0][0];
    first_read = 1;
    
    // Continually check address lines and
    // put associated data on bus.
    while (true) {
        put_data_on_bus(get_requested_address());
    }
}

void setup_gpio() {
    // Address pins.
    gpio_init(A0);
    gpio_set_dir(A0, GPIO_IN);
    gpio_init(A1);
    gpio_set_dir(A1, GPIO_IN);
    gpio_init(A2);
    gpio_set_dir(A2, GPIO_IN);
    gpio_init(A3);
    gpio_set_dir(A3, GPIO_IN);
    gpio_init(A4);
    gpio_set_dir(A4, GPIO_IN);
    gpio_init(A5);
    gpio_set_dir(A5, GPIO_IN);
    gpio_init(A6);
    gpio_set_dir(A6, GPIO_IN);
    gpio_init(A7);
    gpio_set_dir(A7, GPIO_IN);
    gpio_init(A8);
    gpio_set_dir(A8, GPIO_IN);
    gpio_init(A9);
    gpio_set_dir(A9, GPIO_IN);
    gpio_init(A10);
    gpio_set_dir(A10, GPIO_IN);
    gpio_init(A11);
    gpio_set_dir(A11, GPIO_IN);
    gpio_init(A12);
    gpio_set_dir(A12, GPIO_IN);
    gpio_init(A13);
    gpio_set_dir(A13, GPIO_IN);
    gpio_init(A14);
    gpio_set_dir(A14, GPIO_IN);

    // Data pins.
    gpio_init(D0);
    gpio_set_dir(D0, GPIO_OUT);
    gpio_init(D1);
    gpio_set_dir(D1, GPIO_OUT);
    gpio_init(D2);
    gpio_set_dir(D2, GPIO_OUT);
    gpio_init(D3);
    gpio_set_dir(D3, GPIO_OUT);
    gpio_init(D4);
    gpio_set_dir(D4, GPIO_OUT);
    gpio_init(D5);
    gpio_set_dir(D5, GPIO_OUT);
    gpio_init(D6);
    gpio_set_dir(D6, GPIO_OUT);
    gpio_init(D7);
    gpio_set_dir(D7, GPIO_OUT);
}

int get_requested_address() {
    // Return only first 15 bits.
    return gpio_get_all() & 32767;
}

void put_data_on_bus(int address) {
    // Special handing of image data.
    if (address == 0xF00)
    {
        gpio_put_masked(8355840, rom_img << 15);
    }
    else if ((last_address == 0xF00) && (address != 0xF00))
    {
        // On last read update image pointer and store for next time
        if (first_read) {
            // There seems to be a spurious read on power up
            // so just this once, don't update the index...
            first_read = 0;
        } else {
            img_pos++;
        }

        // If a full frame has been drawn...
        if (img_pos >= IMG_SIZE) {
            frame_counter++;

            // Every X number of frames, show the next image.
            if (frame_counter > 500) {
                frame_counter = 0;
                img_idx++;
                if (img_idx >= IMG_NUM) img_idx = 0;
            }
            img_pos = 0;
        }
        rom_img = rom_images[img_idx][img_pos];
    }
    else
    {
        // gpio mask = 8355840; // i.e.: 11111111000000000000000
        // Shift data 15 bits to put it in correct position to match data pin defintion.
        gpio_put_masked(8355840, rom_contents[address] << 15);
    }

    last_address = address;
}

