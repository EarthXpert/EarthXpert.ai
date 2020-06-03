/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rd_party/laslib/exlaslib.h"

#include "lasindex.hpp"
#include "lasquadtree.hpp"
#include "lasreader.hpp"
 //#include <experimental/filesystem>
#include <memory>
#include <sys/stat.h>

namespace earthxpert::thirdparty {
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
  if (!reader) {
    throw std::exception("CreateLAX: failed to open las file");
  }

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
  if (!reader) {
    throw std::exception("CreateLAX: failed to open las file");
  }

  auto summary = std::make_unique<LASsummary>();

  while (reader->read_point()) {
    summary->add(&reader->point);
  }

  return summary;
}
}
