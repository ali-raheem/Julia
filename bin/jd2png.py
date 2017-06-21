#!/usr/bin/python
from PIL import Image

f = open('julia.data', 'rb')
juliaD = f.read()
f.close()

width = height = int(len(juliaD)**0.5)
imgSize = (width, height)

juliaD = juliaD.replace("\x01", "\x44\x66\xed")
juliaD = juliaD.replace("\x00", "\xf9\xfc\x10")

img = Image.frombytes('RGB', imgSize, juliaD, decoder_name='raw')
img.save('output.png')
