/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/pcutils.h"
#include "3rdparty/stl/exstl.h"
#include "core/system/clock.h"

namespace earthxpert {
namespace detail {
  // GNU triangulated surface format .gts
  void WriteGtsFile(const std::string& gtsFile,
    const Point2f& origin, const std::vector<Point2f>& vertices,
    const std::vector<DTEdge>& edges, const std::vector<DTTriangle>& tris) {
    WallClock wclock;

    std::ofstream ofs(gtsFile.c_str());
    ofs << vertices.size() << ' ' << edges.size() << ' ' << tris.size()
      << " GtsSurface GtsFace GtsEdge GtsVertex\n";
    ofs << std::fixed;

    for (auto& v : vertices) {
      ofs << v.x + origin.x << ' ' << v.y + origin.y << " 0.0\n";
    }
    for (auto& e : edges) {
      ofs << e.v0 + 1 << ' ' << e.v1 + 1 << "\n";
    }
    for (auto& t : tris) {
      ofs << t.t0 + 1 << ' ' << t.t1 + 1 << ' ' << t.t2 + 1 << "\n";
    }

    std::cerr << "          WriteGtsFile time " << wclock.TimeSinceStart() << " s" << std::endl;
  }

  // Polygon file format .ply
  void WritePlyFile(const std::string& plyFile, const Point2f& origin, 
    const std::vector<Point2f>& vertices, const std::vector<DTTriangle>& tris) {
    WallClock wclock;

    std::ofstream ofs(plyFile.c_str());
    ofs << "ply\nformat ascii 1.0\n"
      << "element vertex " << vertices.size()
      << "\nproperty float x\nproperty float y\nproperty float z\n"
      << "element face " << tris.size()
      << "\nproperty list uchar int vertex_index\nend_header\n";
    ofs << std::fixed;

    for (auto& v : vertices) {
      ofs << v.x + origin.x << ' ' << v.y + origin.y << " 0.0\n";
    }
    for (auto& t : tris) {
      ofs << "3 " << t.t0 << ' ' << t.t1 << ' ' << t.t2 << "\n";
    }

    std::cerr << "          WritePlyFile time " << wclock.TimeSinceStart() << " s" << std::endl;
  }
}

/**
 *  \brief pointcloud format conversion from .las to .gts
 */
void Las2Gts(const std::string& lasFile,
  const std::string& gtsFile, const CvOptions& cvOpt) {
  auto dt = CvDelaunay(lasFile, cvOpt);

  WallClock wclock;

  std::vector<Point2f> vertices;
  std::vector<DTEdge> edges;
  std::vector<DTTriangle> tris;
  dt->GetGtsInfo(vertices, edges, tris);

  if (cvOpt.verbose) {
    std::cerr << "          GetGtsInfo time " << wclock.TimeSinceStart() << " s" << std::endl;
  }

  const auto origin = dt->GetOrigin();
  detail::WriteGtsFile(gtsFile, origin, vertices, edges, tris);
}

/**
 *  \brief pointcloud format conversion from .las to .ply
 */
void Las2Ply(const std::string& lasFile,
  const std::string& plyFile, const CvOptions& cvOpt) {
  auto dt = CvDelaunay(lasFile, cvOpt);

  WallClock wclock;

  std::vector<Point2f> vertices;
  std::vector<DTTriangle> tris;
  dt->GetPlyInfo(vertices, tris);

  if (cvOpt.verbose) {
    std::cerr << "          GetPlyInfo time " << wclock.TimeSinceStart() << " s" << std::endl;
  }

  const auto origin = dt->GetOrigin();
  detail::WritePlyFile(plyFile, origin, vertices, tris);
}

/**
 *  \brief pointcloud format conversion from .las to .gts
 */
void Las2Gts(const std::string& lasFile,
  const std::string& gtsFile, const GtsOptions& gtsOpt) {
  auto surface = GtsDelaunay(lasFile, gtsOpt);
  
  WallClock wclock;

  // WriteGtsFile
  UPFile fp{fopen(gtsFile.c_str(), "wt"), fclose};
  gts_surface_write(surface, fp.get());

  if (gtsOpt.verbose) {
    std::cerr << "          WriteGtsFile time " << wclock.TimeSinceStart() << " s" << std::endl;
  }
}
}
