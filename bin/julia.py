#!/usr/bin/python

import cmath
import numpy as np
import scipy.misc as smp

def isJulia(x, y):
	z = cmath.rect(x, y)
	c = cmath.rect(-0.8, 0.156)
	i = 0
	while(i < 100):
		if(cmath.isinf(z)):
			return -1
		z = z*z + c
		i += 1
	try:
   		if(abs(z) > 1.7):
   			return 0
	except OverflowError:
		return -1
	return 1
		
if __name__ == '__main__':
	scale = 1.5 #2.5
	height = 1080
	width = 1920
	julia = np.zeros((height, width, 3), dtype=np.uint8)
	x = 0
	y = 0
	while (x < width):
		while (y < height):
			sx = scale * (width/2 - x) / (width/2)
			sy = scale * (height/2 - y) / (height/2)
			ij = isJulia(sx, sy)
			if(ij == 1):
				julia[y, x] = [68, 102, 237]
			elif(ij == 0):
				julia[y, x] = [249, 252, 16]
			elif (ij == -1):
				julia[y, x] = [176, 252, 181]
			y += 1
		x += 1
		y = 0

	img = smp.toimage(julia)
	smp.imsave('./output-1.5-1080x1920-100-1.7-0.6+0.156.png', img)
