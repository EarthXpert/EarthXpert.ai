/* ----------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rdparty/boost/exboost.h"

namespace bfs = boost::filesystem;

namespace earthxpert::thirdparty {
/**
 *  \brief insert prefix for filename component
 *
 *  \param [in] fileName input full path filename
 *  \param [in] prefix string to be inserted
 *  \return Returns file path with prefix
 */
std::string PathPrefixFilename(
  const std::string& fileName, const std::string& prefix) {
  bfs::path filePath(fileName);
  auto fn = prefix + filePath.filename().string();

  return fn;
}

/**
 *  \return Return distance of point to polygon edges.
 */
template <typename Ty> Ty distance(const PointXY<Ty>& p, const Polygon2<Ty>& poly) {
  auto dist = std::numeric_limits<Ty>::max();
  if (bg::within(p, poly)) {
    bgc::Segment nearestSeg;

    bg::for_each_segment(poly, [&dist, &p, &nearestSeg](const auto& seg) {
      double cmpDist = bg::comparable_distance(seg, p);
      if (cmpDist < dist) {
        dist = cmpDist;
        nearestSeg = seg; 
      }
    });

    dist = bg::distance(nearestSeg, p);    // calculate exact distance
  }
  else {
    dist = bg::distance(p, poly);
  }

  return dist;
}

/**
 *  \brief set the i18n/l10n environment
 */
void ConfigureI18N_BL() {

}
}
