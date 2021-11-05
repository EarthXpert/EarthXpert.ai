/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rdparty/laslib/exlaslib.h"
#include "core/exceptions/exexcept.h"

#include <boost/format.hpp>
#include <lasindex.hpp>
#include <lasquadtree.hpp>
#include <lasreader.hpp>
#include <memory>
#include <sstream>
#include <sys/stat.h>

namespace earthxpert::thirdparty {
/**
 *  \param [in] minX x location of the lower-left corner of the tile
 *  \param [in] minY y location of the lower-left corner of the tile
 *  \param [in] width the tile width [unit: meter]
 *  \param [in] height the tile height
 *
 *  \return Return a LASfilter object for the given region.
 */
std::unique_ptr<LASfilter> CreateLasFilter(const double minX,
  const double minY, const double width, const double height) {
  std::stringstream ss;
  ss << boost::format("-keep_xy %1% %2% %3% %4%") \
    % minX % minY % (minX + width) % (minY + height);
  std::string str{ ss.str() };

  auto filter = std::make_unique<LASfilter>();
  filter->parse(str.data());

  return filter;
}

 /**
 *  \brief create a lax index file for storing quadtree of the
 *   las file, return if the lax file already exist
 *
 *  \param [in] lasFile .LAS file name
 *  \return Return name of LAX file
 */
std::string CreateLAX(const std::string &lasFile) {
  auto laxFile = lasFile;
  laxFile.replace(laxFile.end() - 4, laxFile.end(), ".lax");

  // if the lax file already exist, return
  struct stat buffer;
  if (!stat(laxFile.c_str(), &buffer))
    return laxFile;

  LASreadOpener readOpener;
  std::unique_ptr<LASreader> reader(readOpener.open(lasFile.c_str()));
  if (!reader) 
    throw std::exception(_("CreateLAX: failed to open las file"));

  const auto& header{reader->header};
  LASquadtree *quadtree = new LASquadtree;
  quadtree->setup(header.min_x, header.max_x, header.min_y, header.max_y);

  LASindex lasIndex;
  lasIndex.prepare(quadtree);

  while (reader->read_point()) {
    lasIndex.add(reader->point.get_x(), reader->point.get_y(),
      (U32)(reader->p_count - 1));
  }
  lasIndex.complete();
  lasIndex.write(laxFile.c_str());

  return laxFile;
}

/**
 *  \brief traverse points to obtain summarized information
 *
 *  \param [in] lasFile .LAS file name
 *  \return Return LASsummary object
 */
std::unique_ptr<LASsummary> ObtainLasSummary(const std::string& lasFile) {
  LASreadOpener readOpener;
  std::unique_ptr<LASreader> reader(readOpener.open(lasFile.c_str()));
  if (!reader) 
    throw std::exception(_("ObtainLasSummary: failed to open las file"));

  auto summary = std::make_unique<LASsummary>();
  while (reader->read_point()) {
    summary->add(&reader->point);
  }
  return summary;
}
}
