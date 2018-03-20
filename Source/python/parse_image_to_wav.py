import struct
import numpy as np
import sys
import math
import hilbert
from scipy.io import wavfile
import skimage
from skimage import data, color, io, filters, util
from skimage.transform import rescale, resize
from matplotlib import pyplot as plt

samplerate = 44100.0
maxlength = 8 # in seconds

"""
what can we do to make this more interesting?

Traverse the picture in different ways
what other metadata can we get from a picture?

##### TODO:
add filters in xcode
write report
build web interface for impulse creator?

"""
def sectosam(sec):
    global samplerate
    return int(sec * samplerate)

def samtosec(sam):
    global samplerate
    return sam / samplerate

def showImage(im):
    io.imshow(im)
    plt.show()

maxp = int(math.log(sectosam(maxlength), 4))
max_square_side = math.sqrt(int(math.pow(4, maxp)))

file = sys.argv[1]
name = file[:-4]
fp = 'images/' + file

def loadAndSmoothImage(fp):
    im = io.imread(fp, as_grey=True)
    im = filters.gaussian(im)
    im = im.astype(np.float32)
    return im

def normalizeImage(im):
    peakIndexTuple = np.unravel_index(np.argmax(im), im.shape)
    xind = peakIndexTuple[0]
    yind = peakIndexTuple[1]
    max  = im [xind][yind]
    im = (im * 2 / max) - 1
    im *= 0.866
    im = im.astype(np.float32)
    return im

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

def checkForLength(im, hil=False):
    if(hil):
        if(imgIsSquare(im)==False):
            im = makeImgSquare(im)
        if im.shape[0]>max_square_side:
            ratio = max_square_side/im.shape[0]
            im = rescale(im, ratio, mode='constant')
    else:
        total = im.shape[0] * im.shape[1] # currently the total samples
        length = samtosec(total)
        if length>maxlength:
            how_many_times = math.ceil(length/maxlength)
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

def imgIsSquare(im):
    x = im.shape
    if (x[0] != x[1]):
        return False
    return True

def makeImgSquare(im):
    # height needs reduction
    if (im.shape[0]>im.shape[1]):
        ratio = im.shape[0] / im.shape[1]
        im = resize(im, (im.shape[0]/ratio, im.shape[1]), mode='constant')
    # or width needs reduction
    else:
        ratio = im.shape[1] / im.shape[0]
        im = resize(im, (im.shape[0], im.shape[1]/ratio), mode='constant')
    return im

def hilbertTraversal(im):
    # if not square image, let's make it square
    # just squish it, that way we keep some image
    coords = []
    if(imgIsSquare(im)==False):
        im = makeImgSquare(im)
    # then we need to find the closest power of 4
    num_pix = im.shape[0] * im.shape[1]
    p = int(math.log(num_pix, 4))
    hc = hilbert.HilbertCurve(p, 2)
    # should return flattened single array of coordinates
    hil_size = int(math.pow(4, p))
    for i in range(hil_size):
        coord = hc.coordinates_from_distance(i)
        val = im[coord[0]][coord[1]]
        coords.append(val)
    # flatten in order of coords
    return np.asarray(coords, dtype=np.float32)




orig = loadAndSmoothImage(fp)
r = orig
r = checkForLength(r, hil=True)
# showImage(r)
r = normalizeImage(r)
neg, pos = checkPosNegBalance(r)
print("neg", neg,"pos", pos)
# r = hilbertTraversal(r)
# showImage(r)
"""

#probably the best combo so far

r = applyExpEnvDecay(r, 0)

r = applyRationalEnvDecay(r, 0.2)
"""
r = r.flatten()
r = applyExpEnvDecay(r, 0)

r = applyRationalEnvDecay(r, 0.2)
if (len(r.shape)==1):
    # duplicate channel
    r = np.array([r, r])
outpath = "/Users/iegan/Desktop/vacancy/media/IR/"+name+"_test" + ".wav"
# make the write stereo!
wavfile.write(outpath, int(samplerate), r.T)
