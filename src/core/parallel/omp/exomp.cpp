/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "core/parallel/exomp.h"

namespace earthxpert {
int kNumPhysicalCores = omp_get_num_procs();
}
