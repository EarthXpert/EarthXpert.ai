/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "pcproc/filter/pcfilter.h"

namespace earthxpert {
/**
 *  The class PMFilter implements the progressive morphological filter.
 */
class EXPCPROC_EXPORTS PMFilter final : public PCFilter {
public:
  enum PMFAlgo {
    eEXAI,    // own implementation
    ePDAL     // PDAL backend
  };

  struct PMFParams {

  };

  ~PMFilter() = default;

  void SetParams();

  bool Process(Pointcloud* pc) override { return ProcessExai(pc); }

  bool ProcessExai(Pointcloud* pc);
  bool ProcessPdal(Pointcloud* pc);

private:
};
}
