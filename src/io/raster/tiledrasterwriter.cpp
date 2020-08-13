/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/raster/tiledrasterwriter.h"
#include "core/datasource/cvraster.h"
#include "core/datasource/gdalraster.h"
#include "core/datasource/rasterutils.h"
#include "core/exceptions/exexcept.h"

using namespace earthxpert;

TiledRasterWriter::TiledRasterWriter(std::vector<RasterWindow>&& tiles,
  std::vector<std::string>&& imageFiles, const std::vector<int>& bands) {
  Initialize();
}

TiledRasterWriter::TiledRasterWriter(std::vector<RasterWindow>&& tiles,
  std::vector<std::string>&& imageFiles, const std::string format,
  const GDALDataType dataType, const int numBands) {
  Initialize();
}

void TiledRasterWriter::Initialize() {

}
