#  -----------------------------------------------------------------------------
#   Copyright (c) 2019.  All rights reserved.
#  -----------------------------------------------------------------------------

import multiprocessing as mp
import multiprocessing.dummy as mt
#import pp	#  parallel python

from concurrent.futures import ThreadPoolExecutor
from concurrent.futures import ProcessPoolExecutor

from multiprocessing import Pool as ProcessPool
from multiprocessing.dummy import Pool as ThreadPool
