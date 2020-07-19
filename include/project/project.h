/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "project/iproject.h"
#include "3rdparty/gdal/exgdal.h"
#include "project/types.h"

#include <memory>
#include <map>

namespace earthxpert {
/**
 *  The base class Project implementes teh template methods to access entires
 *  of a project composed of imagery data.
 */
class EXPROJECT_EXPORTS Project : public IProject {
public:
  virtual ~Project() = default;

protected:
  std::string mPath;    // absolute path of .epj
  std::vector<ExImage> mImages;
  
  std::vector<TiePoint> mTPs;  // tie points
  std::vector<ControlPoint> mCPs;  // GCPs, ICPs
  std::shared_ptr<SpatialReferenceSystem> mSrsGP;   // Coordinate system of ground points

  std::map<std::string, std::vector<unsigned int>> mStripes;  // pair of stripe_name and the associated array of images

  // cahced

private:
};
}
