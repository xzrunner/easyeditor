# coding: utf-8

import sys, os
from PIL import Image

print(sys.argv)

argv = sys.argv
i = 1
src_filename = argv[i]; i+=1
dst_filename = argv[i]; i+=1
x1 = int(argv[i]); i+=1
y1 = int(argv[i]); i+=1
x2 = int(argv[i]); i+=1
y2 = int(argv[i]); i+=1
x3 = int(argv[i]); i+=1
y3 = int(argv[i]); i+=1
x4 = int(argv[i]); i+=1
y4 = int(argv[i]); i+=1

# print("src_filename:", src_filename)
# print("dst_filename:", dst_filename)
# print("x1:", x1)
# print("y1:", y1)
# print("x2:", x2)
# print("y2:", y2)
# print("x3:", x3)
# print("y3:", y3)
# print("x4:", x4)
# print("y4:", y4)

im = Image.open("misc1.png")
# im.show()

xx1 = min(x1, x2, x3, x4)
xx2 = max(x1, x2, x3, x4)
# width = xx2 - xx1

yy1 = min(y1, y2, y3, y4)
yy2 = max(y1, y2, y3, y4)
# height = yy2 - yy1

im_dst = im.crop((xx1, yy1, xx2, yy2))

# im_dst = Image.new("RGBA", (width, height))

im_dst.save(dst_filename, "PNG")
