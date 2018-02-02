#!/usr/bin/env python3
'''
python2 改写 python3
'''

from PIL import Image
import struct

def readImage(filename):
    f = open(filename,'rb')

    index = 0
    buf = f.read()
    f.close()

    magic, images, rows, columns = struct.unpack_from('>IIII',buf,index)
    tmp = struct.unpack_from('>IIII',buf,index)
    print("tmp:",tmp)
    index += struct.calcsize('>IIII')

    print("magic:",magic," images:",images," rows:",rows," cols:",columns)

    for i in range(images):
        image = Image.new('L',(columns,rows))

        for x in range(rows):
            for y in range(columns):
                image.putpixel((y,x),int(struct.unpack_from('>B',buf,index)[0]))
                index += struct.calcsize('>B')

        print("save ",str(i)," image")

        image.save('./'+str(i)+'.png')


if __name__ == '__main__':
    readImage('mnist/t10k-images-idx3-ubyte')
