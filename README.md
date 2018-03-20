# Vacancy - Convolution Reverb
###### Research for UCSB course **MAT240B**; Winter 2018
This repository contains the work for a convolution reverb plugin, as well as a python script that will transform an image into an Impulse Response to be used with Vacancy.

## Motivation
Construct an open source plugin for personal education, and hopefully for usable distribution.

## Vacancy Features
- Real-time convolution
- User-defined impulse response
- Interactive filter and ADSR envelope design
- Time reversal of IR

## Image Parser Features
- Enter any size or color of photo. A max length parameter determines the size the photo will be cropped to
- Outputs stereo audio derived from image intensities
- Implemented Raster Scanning (1 pass) for image traversal
- Implemented Hilbert traversal (credit to https://github.com/galtay/hilbert_curve for algorithm)

## Dependencies
###### (Using JUCE 5.2.1)
- Python:
  - Numpy
  - SciPy
  - MatPlotLib
  - SciKit-Image
- JUCE:
  - STK Wrapper Module
(see installation section for more details)
  
## Installation and Usage
If you wish to edit or build Vacancy, download this repo and move the *stk_wrapper* folder into your JUCE modules folder. Then, open *vacancy.projucer* and add the *stk_wrapper* folder path to the project. Click save and export for your IDE of choice. Then select your build target, and *TA DA*, you have a build.

If you wish to use the image parser, place whatever photos you want to parse into 'media/images/', where you will find some examples. Then run `python3 parse_image_to_wav.py theNameOfYourImage.jpg`. An audio file of the same name should appear in 
  media/IR/
  
 ## Future goals
- UI Improvement
- Additional parameters such as:
  - Pre-delay
  - Time stretch
- Better plugin memory management
- Find way to wrap python in C++ code to unify Vacancy and the image parser
- Add current builds to the repo
