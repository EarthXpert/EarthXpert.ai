/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/pcutils.h"
#include "core/system/clock.h"

namespace earthxpert {
/**
 *  \brief pointcloud format conversion from .las to .gts
 */
void Las2Gts(const std::string& lasFile,
  const std::string& gtsFile, const CvOptions& cvOpt) {
  auto dt = CvDelaunay(lasFile, cvOpt);

  WallClock wclock;
  double ti{};

  std::vector<Point2f> vertices;
  std::vector<DTEdge> edges;
  std::vector<DTTriangle> tris;
  dt->GetGtsInfo(vertices, edges, tris);

  if (cvOpt.verbose) {
    ti = wclock.TimeSinceStart();
    std::cerr << "          GetGtsInfo time " << ti << " s" << std::endl;
  }

  // write triangulation
  std::ofstream ofs(gtsFile.c_str());
  ofs << std::fixed;
  ofs << vertices.size() << ' ' << edges.size() << ' ' << tris.size()
    << " GtsSurface GtsFace GtsEdge GtsVertex\n";

  const auto origin = dt->GetOrigin();
  for (auto& v : vertices) {
    ofs << v.x + origin.x << ' ' << v.y + origin.y << " 0.0\n";
  }
  for (auto& e : edges) {
    ofs << e.v0 + 1 << ' ' << e.v1 + 1 << "\n";
  }
  for (auto& t : tris) {
    ofs << t.t0 + 1 << ' ' << t.t1 + 1 << ' ' << t.t2 + 1 << "\n";
  }

  if (cvOpt.verbose) {
    ti = wclock.TimeSinceStart() - ti;
    std::cerr << "          WriteGtsFile time " << ti << " s" << std::endl;
  }
}

/**
 *  \brief pointcloud format conversion from .las to .ply
 */
void Las2Ply(const std::string& lasFile,
  const std::string& plyFile, const CvOptions& cvOpt) {
  auto dt = CvDelaunay(lasFile, cvOpt);

  WallClock wclock;
  double ti{};

  std::vector<Point2f> vertices;
  std::vector<DTTriangle> tris;
  dt->GetPlyInfo(vertices, tris);

  if (cvOpt.verbose) {
    ti = wclock.TimeSinceStart();
    std::cerr << "          GetPlyInfo time " << ti << " s" << std::endl;
  }

  // write triangulation
  std::ofstream ofs(plyFile.c_str());
  ofs << std::fixed;
  ofs << "ply\nformat ascii 1.0\n"
    << "element vertex " << vertices.size()
    << "\nproperty float x\nproperty float y\nproperty float z\n"
    << "element face " << tris.size()
    << "\nproperty list uchar int vertex_index\n"
    << "end_header\n";

  const auto origin = dt->GetOrigin();
  for (auto& v : vertices) {
    ofs << v.x + origin.x << ' ' << v.y + origin.y << " 0.0\n";
  }
  for (auto& t : tris) {
    ofs << "3 " << t.t0 << ' ' << t.t1 << ' ' << t.t2 << "\n";
  }

  if (cvOpt.verbose) {
    ti = wclock.TimeSinceStart() - ti;
    std::cerr << "          WritePlyFile time " << ti << " s" << std::endl;
  }
}

/**
 *  \brief pointcloud format conversion from .las to .gts
 */
void Las2Gts(const std::string& lasFile,
  const std::string& gtsFile, const GtsOptions& gtsOpt) {
  auto surface = GtsDelaunay(lasFile, gtsOpt);
  
  WallClock wclock;

  // write triangulation
  FILE* fp = fopen(gtsFile.c_str(), "wt");
  gts_surface_write(surface, fp);
  fclose(fp);

  if (gtsOpt.verbose) {
    double ti = wclock.TimeSinceStart();
    std::cerr << "          WriteGtsFile time " << ti << " s" << std::endl;
  }
}
}
