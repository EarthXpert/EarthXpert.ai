/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rd_party/laslib/wxlaslib.h"

#include "lasindex.hpp"
#include "lasquadtree.hpp"
#include <lasreader.hpp>
 //#include <experimental/filesystem>
#include <memory>
#include <sys/stat.h>

using namespace watxtract;

/**
 *  \brief create a lax index file for storing quadtree of the
 *   las file, return if the lax file already exist
 *
 *  \param [in] lasFile .LAS file name
 */
void CreateLAX(const std::string &lasFile) {
  auto laxFile = lasFile;
  laxFile.replace(laxFile.end() - 4, laxFile.end(), ".lax");

  // if the lax file already exist, return
  struct stat buffer;
  if (!stat(laxFile.c_str(), &buffer))
    return;

  LASreadOpener lasReadOpener;
  std::unique_ptr<LASreader> lasReader(lasReadOpener.open(lasFile.c_str()));

  const auto& header{lasReader->header};
  LASquadtree *lasQuadtree = new LASquadtree;
  lasQuadtree->setup(header.min_x, header.max_x, header.min_y, header.max_y);

  LASindex lasIndex;
  lasIndex.prepare(lasQuadtree);

  while (lasReader->read_point()) {
    lasIndex.add(lasReader->point.get_x(), lasReader->point.get_y(),
      (U32)(lasReader->p_count - 1));
  }
  lasIndex.complete();
  lasIndex.write(laxFile.c_str());
}
