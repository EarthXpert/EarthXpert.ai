#  -----------------------------------------------------------------------------
#   Copyright (c) 2020.  All rights reserved.
#  -----------------------------------------------------------------------------

import argparse
import expyext as epy

def genFrameEpi():
  epy.generateFrameEpipolar()

'''
  The script frameepipolar generates epipolar images 
  from a pair of overlapping images acquired by frame camera.

  usage:  rationalepipolar.py 
'''

if __name__ == '__main__':
  genFrameEpi()
  