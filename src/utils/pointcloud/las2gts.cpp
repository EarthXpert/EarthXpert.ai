/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "utils/pcutils.h"
#include "core/system/clock.h"
#include "io/pointcloud/pointcloudreader.h"

#include <gts.h>
#include <boost/format.hpp>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>

namespace earthxpert {
namespace detail {
  gdouble triangle_cost(GtsTriangle* t, gpointer* data) {
    gdouble* min_quality = (gdouble*)data[0];
    gdouble* max_area = (gdouble*)data[1];
    gdouble quality = gts_triangle_quality(t);
    gdouble area = gts_triangle_area(t);

    if (quality < *min_quality || area > *max_area)
      return quality;
    return 0.0;
  }
}

using fgt = FieldGroup;

/**
 *  \brief pointcloud format conversion from .las to .gts
 */
void Las2Gts(const std::string& lasFile, 
  const std::string& gtsFile, const GtsOptions& gtsOpt) {
  const size_t chunkNumPts = 1024;
  Pointcloud pc(chunkNumPts, { fgt::kBITXYZ });
  
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });
  const size_t filePointCount = reader.GetPointCount();

  GPtrArray* vertices = g_ptr_array_new();
  g_ptr_array_set_size(vertices, filePointCount);

  for (size_t i = 0; i < filePointCount; i += chunkNumPts) {
    const auto readNumPts = reader.ReadPointcloud(i, &pc);
    for (size_t j = 0; j < readNumPts; ++j) {
      const auto xyz = pc.GetField<XYZF>(j);
      //std::cout << xyz << std::endl;
      g_ptr_array_index(vertices, i + j) =
        gts_vertex_new(gts_vertex_class(), xyz.x, xyz.y, xyz.z);
    }
  }

  WallClock wclock;

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

  // add vertices 
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
    guint encroachedNumber = gts_delaunay_conform(surface, gtsOpt.steiner_max,
      (GtsEncroachFunc)gts_vertex_encroaches_edge, nullptr);
    if (gtsOpt.refine && 0 == encroachedNumber) {
      gdouble quality = gtsOpt.quality;
      gdouble area = gtsOpt.area;
      gpointer data[2] = { &quality, &area };
      guint unrefinedNumber = gts_delaunay_refine(surface, gtsOpt.steiner_max,
        (GtsEncroachFunc)gts_vertex_encroaches_edge, nullptr,
        (GtsKeyFunc)detail::triangle_cost, data);
      if (gtsOpt.verbose && unrefinedNumber > 0)
        std::cerr << "delaunay: ran out of Steiner points during refinement\n";
    }
    else if (gtsOpt.verbose && encroachedNumber > 0)
      std::cerr << "delaunay: ran out of Steiner points during conforming\n";
  }
  
  if (gtsOpt.verbose) {
    std::cerr << "delaunay: ellapsed time (s): " << wclock.TimeSinceStart() << std::endl;
    gts_surface_print_stats(surface, stderr);
  }

  if (gtsOpt.checkDelaunay && gts_delaunay_check(surface))
    std::cerr << "delaunay: triangulation is not Delaunay\n";

  FILE* fp = fopen(gtsFile.c_str(), "wt");
  gts_surface_write(surface, fp);   // write triangulation
  fclose(fp);
}
}
