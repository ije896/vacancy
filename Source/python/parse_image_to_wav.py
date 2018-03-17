import wave
import struct
import numpy as np
from scipy.io import wavfile
from skimage import data, color, io, filters
from matplotlib import pyplot as plt

samplerate = 44100

fp = 'images/droplet.jpg'

def loadAndSmoothImage(fp):
    im = io.imread(fp, as_grey=True)
    im = filters.gaussian(im)
    return im


def normalizeImage(im):
    peakIndexTuple = np.unravel_index(np.argmax(im), im.shape)
    xind = peakIndexTuple[0]
    yind = peakIndexTuple[1]
    max  = im [xind][yind]
    im = (im * 2 / max) - 1
    im *= 0.707
    return im

def showImage(im):
    io.imshow(im)
    plt.show()

def checkPosNegBalance(im):
    neg = 0
    x = im.shape
    total = x[0] * x[1]
    for i in range(x[0]):
        for j in range(x[1]):
            if im[i][j]<0:
                neg += 1
    negperc = neg / total
    posperc = 1- negperc
    return negperc, posperc



r = loadAndSmoothImage(fp)
r = normalizeImage(r)
neg, pos = checkPosNegBalance(r)
print("neg", neg,"pos", pos)

outpath = "/Users/iegan/Music/IR/test.wav"
wavfile.write(outpath, samplerate, r)

# showImage(r)
