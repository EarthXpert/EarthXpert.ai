#  -----------------------------------------------------------------------------
#   Copyright (c) 2019.  All rights reserved.
#  -----------------------------------------------------------------------------

import argparse
import yaml
from expyext import expy as epy

def saveRationalModel(modelFile, ratModel):
  epy.saveRationalModel(modelFile, ratModel)

def loadRationalModel(modelFile):
  ret = epy.loadRationalModel(modelFile)
  return ret

def loadRpcFile(rpcFile)
  stream = open(rpcFile, 'r')
  rpc = yaml.load(stream)
  for key, value in rpc.items():
    print (key + " : " + str(value))
