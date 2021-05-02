#  -----------------------------------------------------------------------------
#   Copyright (c) 2020.  All rights reserved.
#  -----------------------------------------------------------------------------

import argparse
from expyext import expy as epy

def genFrameEpi():
  epy.generateFrameEpipolar()


if __name__ == '__main__':
  """The script frameepipolar generates epipolar images 
    from a pair of overlapping images acquired by frame camera.

    usage:  rationalepipolar.py 
  """

  genFrameEpi()
  