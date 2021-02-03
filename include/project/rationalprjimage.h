/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/iprjimage.h"
#include "project/types.h"

#include <memory>
#include <map>

namespace earthxpert {
/**
 *  The base class Project implementes teh template methods to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS RationalPrjImage final : public IPrjImage {
public:
  RationalPrjImage(std::string imgPath);
  RationalPrjImage(const RationalPrjImage&) = delete;
  RationalPrjImage(RationalPrjImage&&) = delete;
  virtual ~RationalPrjImage() = default;

  RationalPrjImage& operator=(const RationalPrjImage&) = delete;
  RationalPrjImage& operator=(RationalPrjImage&&) = delete;

private:
  std::string mPath;    // absolute path of master project file

  std::vector<ImagePoint> mIPs;  // image observation points

  // cahced
};
}
