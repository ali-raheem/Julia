#!/usr/bin/python
from PIL import Image

width = 800
height = 800
imgSize = (width, height)
juliaD = open('julia.data', 'rb').read()
img = Image.frombytes('RGB', imgSize, juliaD, decoder_name='raw')
img.save('output.png')
