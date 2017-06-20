#!/usr/bin/python
from PIL import Image

width = 1000
height = 1000
imgSize = (width, height)
f = open('julia.data', 'rb')
juliaD = f.read()
f.close()
juliaD = juliaD.replace("\x01", "\x44\x66\xed")
juliaD = juliaD.replace("\x00", "\xf9\xfc\x10")

img = Image.frombytes('RGB', imgSize, juliaD, decoder_name='raw')
img.save('output.png')
