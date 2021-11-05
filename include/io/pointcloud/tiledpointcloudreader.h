/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudreader.h"
#include "core/utils/fsutils.h"
#include "core/utils/spatialtile.h"

#include <lasreader.hpp>
#include <iostream>
#include <mutex>
#include <vector>

namespace earthxpert {
/**
 *
 */
class EXIO_EXPORTS TiledPointcloudReader /*final*/ : public IPointcloudReader{
public:
  TiledPointcloudReader(std::vector<SpatialTileD>&& tiles, 
    std::vector<std::string>&& lasFiles, FieldGroup fieldGroup = {});
  TiledPointcloudReader(const TiledPointcloudReader&) = delete;
  TiledPointcloudReader(TiledPointcloudReader&&) = delete;
  ~TiledPointcloudReader() = default;

  TiledPointcloudReader& operator=(const TiledPointcloudReader&) = delete;
  TiledPointcloudReader& operator=(TiledPointcloudReader&&) = delete;

private:
  std::vector<SpatialTileD> mTiles;   // layout of spatail region
  std::vector<std::string> mLasFiles; // one file per tile

  void Initialize();

  std::unique_ptr<LASreader> mReader{}; // reader of current file
  FieldGroup mFieldGroup{};   // fields group to read

  // cached information
  int mNumTiles{};

  UPIPointcloudReader mTileReader;   // currently active reader
};
}
