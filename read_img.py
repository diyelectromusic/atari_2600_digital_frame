import sys
from PIL import Image

im = Image.open(sys.argv[1])
pix = im.load()

print("{")
print("// File ",sys.argv[1])
print("// Size ",im.size[0],"x",im.size[1])

# View ASCII version of image.
for y in range (im.size[1]):
	print("//  " +"{:03d}".format(y), " ", end="")
	for x in range (0, 64):
		if pix[x, y] > 0:
			print("1", end="")
		else:
			print("0", end="")
		
	print()

print()

addr = 0

# Left side of image.
cnt = 0
val = ""
for y in range (im.size[1]):
	for x in range (0, 48):
		if pix[x, y] > 0:
			val += "1"
		else:
			val += "0"
		
		cnt += 1

		if cnt == 8:
			cnt = 0
			print("0x{0:02x}, ".format(int(val, 2)), end="")
			val = ""
			addr += 1
			if (addr % 6 == 0):
				print()

# Right side of image.
cnt = 0
val = ""
for y in range (im.size[1]):
	for x in range (48, 64):
		if pix[x, y] > 0:
			val += "1"
		else:
			val += "0"
		
		cnt += 1

		if cnt == 8:
			cnt = 0
			print("0x{0:02x}, ".format(int(val, 2)), end="")
			val = ""
			addr += 1
			if (addr % 2 == 0):
				print()

print("},")
