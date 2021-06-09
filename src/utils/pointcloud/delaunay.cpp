/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/delaunay.h"
#include "core/system/clock.h"
#include "io/pointcloud/pointcloudreader.h"

#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <boost/format.hpp>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

namespace earthxpert {
namespace detail {
  /**
   *  \param [in] data [min_quality, max_area]
   */
  gdouble triangle_cost(GtsTriangle* t, gdouble* data) {
    const gdouble quality = gts_triangle_quality(t);
    const gdouble area = gts_triangle_area(t);
    if (quality < data[0] || area > data[1])
      return quality;
    return 0.0;
  }
}
using fgt = FieldGroup;

/**
 *  \brief Delaunay triangualtion by OpenCV lib.
 */
std::unique_ptr<CvDelaunay2d>
CvDelaunay(const std::string& lasFile, const CvOptions& cvOpt) {
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });
  auto pc = reader.ReadPointcloud();
  auto bbox = pc->GetBufTile();

  WallClock wclock;

  // create rectangle enclosing all the vertices
  DTVertex2f origin{ float(bbox.minX) - 1, float(bbox.minY) - 1 };
  cv::Rect rect{ 0, 0, int(ceil(bbox.width)) + 2, int(ceil(bbox.height)) + 2 };
  
  auto dt = std::make_unique<CvDelaunay2d>(rect);
  dt->SetOrigin(origin);

  // add vertices into triangulation
  const size_t count = pc->GetPointCount();
  for (size_t j = 0; j < count; ++j) {
    const auto xyz = pc->GetField<XYZF>(j);
    dt->Insert(xyz.x - origin.x, xyz.y - origin.y);
  }
  pc.reset();

  if (cvOpt.verbose) {
    std::cerr << "delaunay: " << count << " vertices\n"
      << "          Triangulation time " << wclock.TimeSinceStart() << " s" << std::endl;
  }
  return dt;
}

/**
 *  \brief Delaunay triangualtion by Geogram lib.
 */
std::unique_ptr<GeogramDelaunay2d>
GeogramDelaunay(const std::string& lasFile, const GeogramOptions& ggOpt) {
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });
  auto pc = reader.ReadPointcloud();
  const auto& XYZ = pc->GetField<XYZF>();

  const size_t count = pc->GetPointCount();
  std::vector<double> vertices(count*2);

  for (size_t i = 0; i < count; ++i) {
    const size_t i2 = i * 2;
    vertices[i2] = XYZ[i].x;
    vertices[i2+1] = XYZ[i].y;
  }
  pc.reset();

  WallClock wclock;

  auto dt = std::make_unique<GeogramDelaunay2d>();
  //dt->SetOrigin(origin);

  // add vertices into triangulation
  dt->Insert(std::move(vertices));

  if (ggOpt.verbose) {
    std::cerr << "delaunay: " << count << " vertices\n"
      << "          Triangulation time " << wclock.TimeSinceStart() << " s" << std::endl;
  }
  return dt;
}

/**
 *  \brief Delaunay triangualtion by GEOS lib.
 */
std::unique_ptr<GeosDelaunay2d>
GeosDelaunay(const std::string& lasFile, const GeosOptions& gsOpt) {
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });
  auto pc = reader.ReadPointcloud();
  const auto& XYZ = pc->GetField<XYZF>();

  const size_t count = pc->GetPointCount();
  std::vector<gsg::Coordinate> coords(count);

  for (size_t i = 0; i < count; ++i) {
    coords[i].x = XYZ[i].x;
    coords[i].y = XYZ[i].y;
  }
  pc.reset();

  auto csFac = gsg::CoordinateArraySequenceFactory::instance();
  auto cs = csFac->create(std::move(coords), 2);

  WallClock wclock;

  auto dt = std::make_unique<GeosDelaunay2d>();
  //dt->SetOrigin(origin);

  // add vertices into triangulation
  dt->Insert(*cs);
  cs.reset();

  if (gsOpt.verbose) {
    std::cerr << "delaunay: " << count << " vertices\n"
      << "          Triangulation time " << wclock.TimeSinceStart() << " s" << std::endl;
  }
  return dt;
}

/**
 *  \brief Delaunay triangualtion by GNU Triangulated Surface lib.
 *  \todo memory management by GTS
 */
GtsSurface* GtsDelaunay(const std::string& lasFile, const GtsOptions& gtsOpt) {
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });
  auto pc = reader.ReadPointcloud();
  const size_t count = pc->GetPointCount();

  WallClock wclock;

  GPtrArray* vertices = g_ptr_array_new();
  g_ptr_array_set_size(vertices, count);

  for (size_t j = 0; j < count; ++j) {
    const auto xyz = pc->GetField<XYZF>(j);
    g_ptr_array_index(vertices, j) =
      gts_vertex_new(gts_vertex_class(), xyz.x, xyz.y, xyz.z);
  }
  pc.reset();

  // create triangle enclosing all the vertices
  GSList* list{};
  for (guint i = 0; i < vertices->len; ++i)
    list = g_slist_prepend(list, g_ptr_array_index(vertices, i));
  GtsTriangle* t = gts_triangle_enclosing(gts_triangle_class(), list, 100.);
  g_slist_free(list);

  // create surface with one face: the enclosing triangle 
  GtsSurface* surface = gts_surface_new(gts_surface_class(),
    gts_face_class(), gts_edge_class(), gts_vertex_class());
  gts_surface_add_face(surface, gts_face_new(
    gts_face_class(), t->e1, t->e2, t->e3));

  // add vertices into triangulation
  for (guint i = 0; i < vertices->len; ++i) {
    GtsVertex* v = (GtsVertex*)g_ptr_array_index(vertices, i);
    GtsVertex* v0 = gts_delaunay_add_vertex(surface, v, nullptr);
    if (v0)
      gts_vertex_replace(v, v0);
  }
  g_ptr_array_free(vertices, TRUE);

  // destroy enclosing triangle 
  GtsVertex* va[3];
  gts_triangle_vertices(t, &va[0], &va[1], &va[2]);
  //gts_allow_floating_vertices = TRUE;
  gts_object_destroy(GTS_OBJECT(va[0]));
  gts_object_destroy(GTS_OBJECT(va[1]));
  gts_object_destroy(GTS_OBJECT(va[2]));
  //gts_allow_floating_vertices = FALSE;

  if (!gtsOpt.keepHull)
    gts_delaunay_remove_hull(surface);

  //if (removeHoles)
  //  gts_delaunay_remove_holes(surface);

  if (gtsOpt.conform) {
    const guint encroachedNumber = gts_delaunay_conform(surface,
      gtsOpt.steiner_max, (GtsEncroachFunc)gts_vertex_encroaches_edge, nullptr);
    if (gtsOpt.refine && 0 == encroachedNumber) {
      gdouble data[2] = { gtsOpt.quality, gtsOpt.area };
      const guint unrefinedNumber = gts_delaunay_refine(surface,
        gtsOpt.steiner_max, (GtsEncroachFunc)gts_vertex_encroaches_edge,
        nullptr, (GtsKeyFunc)detail::triangle_cost, data);
      if (gtsOpt.verbose && 0 < unrefinedNumber)
        std::cerr << "delaunay: ran out of Steiner points during refinement\n";
    }
    else if (gtsOpt.verbose && 0 < encroachedNumber)
      std::cerr << "delaunay: ran out of Steiner points during conforming\n";
  }

  if (gtsOpt.verbose) {
    std::cerr << "delaunay: " << count << " vertices\n"
      << "          Triangulation time " << wclock.TimeSinceStart() << " s" << std::endl;
    gts_surface_print_stats(surface, stderr);
  }

  if (gtsOpt.checkDelaunay && gts_delaunay_check(surface))
    std::cerr << "delaunay: triangulation is not Delaunay\n";

  return surface;
}
}
