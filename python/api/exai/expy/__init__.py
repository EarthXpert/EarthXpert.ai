#  -----------------------------------------------------------------------------
#   Copyright (c) 2019.  All rights reserved.
#  -----------------------------------------------------------------------------

import frameepipolar as frmepi
import rationalepipolar as ratepi

class ExApi():
  """
  
  Attributes:
    ver: 
  """
  
  def __init__(self):
    self.ver = ''
    
  def genFrameEpi():
    frmepi.genFrameEpi()
    
  def genRationalEpi():
    ratepi.genRationalEpi()
