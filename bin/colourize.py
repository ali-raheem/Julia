#!/usr/bin/python
from PIL import Image

f = open('julia.data', 'rb')
juliaD = f.read()
f.close()

width = height = int(len(juliaD)**0.5)
imgSize = (width, height)


palette=["\x00\x00\x00", "\x9D\x9D\x9D", "\xFF\xFF\xFF", "\xBE\x26\x33", "\xE0\x6F\x8B", "\x49\x3C\x2B", "\xA4\x64\x22", "\xEB\x89\x31", "\xF7\xE2\x6B", "\x2F\x48\x4E", "\x44\x89\x1a", "\xA3\xce\x27", "\x1B\x26\x32", "\x00\x57\x84", "\x31\xa2\xf2", "\xB2\xDC\xEF"]

pltimg = ''

for i in juliaD:
    pltimg += palette[(ord(i)/16)%16]

img = Image.frombytes('RGB', imgSize, pltimg, decoder_name='raw')
img.save('output.png')
