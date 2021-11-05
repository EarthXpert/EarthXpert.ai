/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "io/pointcloud/ipointcloudwriter.h"
#include "core/utils/fsutils.h"
#include "core/utils/spatialtile.h"

#include <lasquantizer.hpp>
#include <laswriter.hpp>
#include <mutex>
#include <vector>

namespace earthxpert {
/**
 *
 */
class EXIO_EXPORTS TiledPointcloudWriter /*final*/ : public IPointcloudWriter{
public:
  /**
   *  \brief Open/create the las files for write
   */
  TiledPointcloudWriter(std::vector<SpatialTileD>&& tiles,
    std::vector<std::string>&& lasFiles, FieldGroup fieldGroup,
    std::vector<LASquantizer>&& quantizers, const std::string format = "LAS");
  TiledPointcloudWriter(std::vector<SpatialTileD>&& tiles,
    std::vector<std::string>&& lasFiles, std::vector<LASquantizer>&& quantizers, 
    const std::string format = "LAS");
  TiledPointcloudWriter(const TiledPointcloudWriter&) = delete;
  TiledPointcloudWriter(TiledPointcloudWriter&&) = delete;
  ~TiledPointcloudWriter() = default;

  TiledPointcloudWriter& operator=(const TiledPointcloudWriter&) = delete;
  TiledPointcloudWriter& operator=(TiledPointcloudWriter&&) = delete;

private:
  std::vector<SpatialTileD> mTiles;  // 
  std::vector<std::string> mLasFiles; // one file per tile

  void Initialize();

  std::unique_ptr<LASwriter> mWriter; // writer of current tile
  FieldGroup mFieldGroup;   // fields group to write
  std::vector<LASquantizer> mQuantizers;  // scale & offset parameters used to normalize XYZ

  // cache
  int mNumTiles{};
  int mCurTile{};   // id of current tile

  UPIPointcloudWriter mTileWriter;   // currently active writer
};
}
