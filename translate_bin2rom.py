import sys

cnt = 0

print("const uint8_t rom_contents[ROM_SIZE] = {");
print("/* {0:04x} */\t".format(cnt), end="")

with open(sys.argv[1], "rb") as file:
	while byte := file.read(1):  # Read one byte at a time
		v = int.from_bytes(byte, "little")
		print("0x{0:02x}, ".format(v), end="")
	
		cnt += 1
	
		if (cnt % 8 == 0):
			print()
			print("/* {0:04x} */\t".format(cnt), end="")

print()
print("};")
