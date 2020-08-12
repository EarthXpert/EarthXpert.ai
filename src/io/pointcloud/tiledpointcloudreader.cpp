/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/tiledpointcloudreader.h"
#include "3rdparty/gettext/exgettext.h"
#include "3rdparty/laslib/exlaslib.h"
#include "core/datasource/pointcloudutils.h"
#include "core/exceptions/exexcept.h"

#include <limits>
#include <omp.h>
#include <optional>

using namespace earthxpert;
using thirdparty::CreateLasFilter;
using thirdparty::PointDataTraits;

TiledPointcloudReader::TiledPointcloudReader(std::vector<SpatialTileD>&& tiles,
  std::vector<std::string>&& lasFiles, const FieldGroup fieldGroup) 
  : mTiles(tiles), mLasFiles(lasFiles), mFieldGroup(fieldGroup) {
  Initialize();
}

void TiledPointcloudReader::Initialize() {
  mNumTiles = int(mTiles.size());
}
