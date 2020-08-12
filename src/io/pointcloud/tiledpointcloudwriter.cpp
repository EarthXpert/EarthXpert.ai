/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/tiledpointcloudwriter.h"
#include "3rdparty/gettext/exgettext.h"
#include "3rdparty/laslib/exlaslib.h"
#include "core/datasource/pointcloudutils.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;
using pdt = earthxpert::thirdparty::PointDataTraits;

TiledPointcloudWriter::TiledPointcloudWriter(std::vector<SpatialTileD>&& tiles,
  std::vector<std::string>&& lasFiles, const FieldGroup fieldGroup,
  std::vector<LASquantizer>&& quantizers, const std::string format)
  : mTiles(std::move(tiles)), mLasFiles(std::move(lasFiles)), 
    mFieldGroup(fieldGroup), mQuantizers(quantizers) {
  Initialize();
}

TiledPointcloudWriter::TiledPointcloudWriter(std::vector<SpatialTileD>&& tiles,
  std::vector<std::string>&& lasFiles, std::vector<LASquantizer>&& quantizers,
  const std::string format) : TiledPointcloudWriter(std::move(tiles), 
    std::move(lasFiles), { FieldGroup::kBITXC }, std::move(quantizers), format) {

}

void TiledPointcloudWriter::Initialize() {
  mNumTiles = int(mTiles.size());
}
