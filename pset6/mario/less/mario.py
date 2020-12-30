#! /usr/bin/env python3

# then run chmod a+x in linux command line
# then using ./[file].py to execute the python file
from cs50 import get_int
while True:
    height = get_int("Height: ")
    if height > 0 and height <= 8:
        break

for i in range(height):
    print(" " * (height - i - 1), end ="")
    print("#" * (i + 1))

