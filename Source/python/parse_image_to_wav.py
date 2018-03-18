import wave
import struct
import numpy as np
import sys
import math
from scipy.io import wavfile
import skimage
from skimage import data, color, io, filters, util
from skimage.transform import rescale
from matplotlib import pyplot as plt

# what can we do to make this more interesting?
#
# Traverse the picture in different ways
# what other metadata can we get from a picture?

##### TODO:
# try different traversal methods
# add filters in xcode
# try different decay functions
# write decay function into xcode (volume envelope)
# write report
# build web interface for impulse creator?



samplerate = 44100.0
maxlength = 8 # in seconds

file = sys.argv[1]
name = file[:-4]
fp = 'images/' + file


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
    im *= 0.866
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

def sectosam(sec):
    global samplerate
    return int(sec * samplerate)

def samtosec(sam):
    global samplerate
    return sam / samplerate

def checkForLength(im):
    x = im.shape
    total = x[0] * x[1] # currently the total samples
    length = samtosec(total)
    if length>maxlength:
        how_many_times = math.ceil(length/8)
        im = rescale(im, math.sqrt(1.0/float(how_many_times)), mode='constant')
    return im


def applyRationalEnvDecay(data, start):
    firstpos = sectosam(start)
    diff = 1 - start
    size = len(data)
    if(diff<1):
        for i in range(firstpos,size):
            data[i]/= diff + samtosec(i)
    else:
        for i in range(firstpos,size):
            data[i]/= diff + samtosec(i)
    return data

# squared rational decay

# linear decay


def applyExpEnvDecay(data, start):
    # this is going to be a matter of timeshifting
    # so that exp(0) is where you want decay to start
    size = len(data)
    firstpos = sectosam(start)
    for i in range(firstpos, size):
        data[i] *= math.exp(-samtosec(i))
    return data



r = loadAndSmoothImage(fp)
r = checkForLength(r)
# showImage(r)
r = normalizeImage(r)
neg, pos = checkPosNegBalance(r)
print("neg", neg,"pos", pos)
r = r.astype(np.float32)
r = r.flatten()
# probably the best combo so far
# r = applyRationalEnvDecay(r, 0.2)
# r = applyExpEnvDecay(r, 0)
r = applyExpEnvDecay(r, 0)
r = applyRationalEnvDecay(r, 0.2)


outpath = "/Users/iegan/Music/IR/"+name+"_exprat" + ".wav"
wavfile.write(outpath, int(samplerate), r)
