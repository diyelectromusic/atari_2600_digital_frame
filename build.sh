echo "Assembling..."
dasm showpic.asm -f3 -oshowpic.bin -lshowpic.lst
echo "Creating ROM..."
python3 translate_bin2rom.py showpic.bin > pico_rom_contents.h
echo "Creating images..."
echo "const uint8_t rom_images[IMG_NUM][IMG_SIZE] = {" > pico_rom_images.h
python3 read_img.py "img/for_display/1-convert.png" >> pico_rom_images.h
python3 read_img.py "img/for_display/2-convert.png" >> pico_rom_images.h
python3 read_img.py "img/for_display/3-convert.png" >> pico_rom_images.h
python3 read_img.py "img/for_display/hackaday.png" >> pico_rom_images.h
echo "};" >> pico_rom_images.h
echo "Done."



