/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <gsl/gsl_const.h>
#include <gsl/gsl_poly.h>

#include <memory>

namespace earthxpert {
// stateless deleter
struct EX3RDPARTY_EXPORTS GslPolyComplexWorkspaceDx {
  void operator()(gsl_poly_complex_workspace* ptr) const {
    gsl_poly_complex_workspace_free(ptr);
  }
};

using UPGslPolyComplexWorkspace =
  std::unique_ptr<gsl_poly_complex_workspace, GslPolyComplexWorkspaceDx>;
}
