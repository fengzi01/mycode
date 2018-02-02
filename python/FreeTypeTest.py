#!/usr/bin/env python3
import freetype
import numpy as np
from PIL import Image

face = freetype.Face("/Library/Fonts/Courier New.ttf")
#face.set_char_size( 48*48 )
face.set_pixel_sizes(1280,1280)

for i in range(10):
    face.load_char(str(i))
    bitmap = face.glyph.bitmap
#    print(bitmap.buffer)
    print(bitmap.rows,",",bitmap.width)
    c = np.array(bitmap.buffer).reshape(bitmap.rows,bitmap.width)

    c = c.T
    c = np.ones(c.shape,dtype=int)*255 - c

    image = Image.new('L',c.shape)
    for x in range(c.shape[0]):
        for y in range(c.shape[1]):
            image.putpixel((x,y),int(c[x][y]))

    print("save "+str(i)+" png")
    image.save("./freetype"+str(i)+".png")

#print(c)

ch = 'a'
face.load_char(ch)
bitmap = face.glyph.bitmap
#print(bitmap.buffer)
print(bitmap.rows,",",bitmap.width)
c = np.array(bitmap.buffer).reshape(bitmap.rows,bitmap.width)

c = c.T

image = Image.new('L',c.shape)
for x in range(c.shape[0]):
    for y in range(c.shape[1]):
        image.putpixel((x,y),int(c[x][y]))

print("save "+str(ch)+" png")
image.save("./freetype"+str(ch)+".png")
