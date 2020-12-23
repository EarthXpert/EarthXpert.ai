#  -----------------------------------------------------------------------------
#   Copyright (c) 2019.  All rights reserved.
#  -----------------------------------------------------------------------------

import argparse
import expyext as epy

def saveRationalModel(modelFile, ratModel):
  epy.saveRationalModel(modelFile, ratModel)

def loadRationalModel(modelFile):
  ret = epy.loadRationalModel(modelFile)
  return ret
