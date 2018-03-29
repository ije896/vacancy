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
- Enter any size or color of photo. A max length parameter determines the size the photo will be resized to 
- Outputs stereo audio derived from image intensities
- Implemented Raster Scanning (1 pass) for image traversal
- Implemented Hilbert traversal (credit to https://github.com/galtay/hilbert_curve for algorithm)

## Dependencies
- Python:
  - Numpy
  - SciPy
  - MatPlotLib
  - SciKit-Image
- JUCE (v5.2.1):
  - STK Wrapper Module (sourced from https://github.com/adamski/stk_wrapper)
(you don't need to, and you shouldn't download that module ^^. I had to modify STK for the needs of this plugin, so use the included folder. See installation instructions for more details)
  
## Installation and Usage
The most recent builds of the plugin are available as a VST or an AU in the *Builds/* directory.

If you wish to edit or build Vacancy, download this repo. First, open *vacancy.jucer* and check that the *stk_wrapper* folder path is added in the 'Module' settings pane. Click save and export for your IDE of choice. Then select your build target, and *TA DA*, you have a working build.

If you wish to use the image parser, place whatever photos you want to parse into the directory *media/images/*, where you will find some examples. Then run `python3 parse_image_to_wav.py theNameOfYourImage.jpg`. An audio file of the same name should appear in the directory *media/IR/*
  
 ## Future goals
- UI Improvement
- Additional parameters such as:
  - Pre-delay
  - Time stretch
- Better plugin memory management
- Find way to wrap python in C++ code to unify Vacancy and the image parser
