#  -----------------------------------------------------------------------------
#   Copyright (c) 2020.  All rights reserved.
#  -----------------------------------------------------------------------------

import pytest, unittest
import numpy as np
#from expyext import utils as epu

def test_eigen():
  a = np.array([1.0, 2], [3, 4], [5, 6])
  b = np.ones((2, 1))
  assert np.all(epu.matrixMultiple(a, b) == np.array([[3.0], [7], [11]]))

#def test_numpy():
#  assert epu.normL2(np.array(range(6), dtype="float64")) == 55
  
  
if __name__ == "__main__":
  unittest.main()
  