#---------------------------------------------------------
# Imagem processing using python
# By Luiz Eduardo da Silva
#---------------------------------------------------------
#!/usr/bin/env python
# coding: utf-8

import matplotlib.pyplot as plt
import matplotlib.image as img
import numpy as np
import argparse
import os

def readpgm (name):
    f = open(name,"r")

    assert f.readline() == 'P2\n'
    line = f.readline()
    while (line[0] == '#'):
           line = f.readline()
    (width, height) = [int(i) for i in line.split()]
    print (width, height)
    depth = int(f.readline())
    assert depth <= 255
    print (depth)
    
    img = []
    row = []
    j = 0
    for line in f:
       values = line.split()
       for val in values:
           row.append (int (val))
           j = j + 1
           if j >= width:
              img.append (row)
              j=0
              row = []

    f.close()
    return img

def savepgm(name, img, depth):
    # Dimensions
    heigth = len(img)
    width = len(img[0])

    # Open file to write in
    with open(name, 'w') as f:
        # Pgm header
        f.write("P2\n")
        f.write("# Image processing with python - ByDu\n")
        f.write("{} {}\n".format(width, heigth))
        f.write("{}\n".format(depth))  # Max gray level

        # write pixels to pgm file
        for line in img:
            for pixel in line:
                f.write("{} ".format(pixel))
            f.write("\n")


def imgalloc (nl, nc):
    img = []
    for i in range(nl):
        lin = []
        for j in range(nc):
            lin.append(0)
        img.append(lin)
    return img

# Read image
parser = argparse.ArgumentParser(description='Script description')

# Add Arguments
parser.add_argument('image_name', type=str, help='Image name in format .PGM')

# Analysing arguments in command line
args = parser.parse_args()
image_name = args.image_name

img = readpgm(image_name)
print(np.asarray(img))

# Negative image processing
nl = len(img)
nc = len(img[0])
imgS = imgalloc(nl, nc)
for i in range(nl):
    for j in range(nc):
        imgS[i][j] = 255 - img[i][j]

print(np.asarray(imgS))
savepgm("result.pgm", imgS, 255)

#os.system("{} {} &".format("eog", "result.pgm"))  # Linux command (uncomment this line for Linux)

# Windows system command (uncomment the next line to open the result image in Windows)
os.system("start result.pgm")


# Other form to calculate negative image in python
# imgT = np.asarray(img)
# imgT = 255- imgT
