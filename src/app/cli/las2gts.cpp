/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "io/pointcloud/pointcloudreader.h"
#include <gts.h>
#include <boost/format.hpp>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <omp.h>

using namespace earthxpert;
using fgt = FieldGroup;

static gdouble triangle_cost(GtsTriangle* t, gpointer* data) {
  gdouble* min_quality = (gdouble*)data[0];
  gdouble* max_area = (gdouble*)data[1];
  gdouble quality = gts_triangle_quality(t);
  gdouble area = gts_triangle_area(t);
  
  if (quality < *min_quality || area > *max_area)
    return quality;
  return 0.0;
}

int main(int argc, char** argv) {
  const std::string lasFile(argv[1]);
  PointcloudReader reader(lasFile, { fgt::kBITXYZ });

  const bool keepHull{true};
  //const bool removeHoles{false};
  const bool conform{true};
  const bool refine{true};
  const gint steiner_max{-1};
  gdouble quality{0.0};
  gdouble area{G_MAXDOUBLE};
  const bool verbose{true};
  const bool checkDelaunay{true};

  const size_t chunkNumPts = 1024;
  const size_t filePointCount = reader.GetPointCount();

  Pointcloud pc(chunkNumPts, { fgt::kBITXYZ });

  GPtrArray* vertices = g_ptr_array_new();
  g_ptr_array_set_size(vertices, filePointCount);

  for (size_t i = 0; i < filePointCount; i += chunkNumPts) {
    const auto readNumPts = reader.ReadPointcloud(i, &pc);
    for (size_t j = 0; j < readNumPts; ++j) {
      const auto xyz = pc.GetField<XYZF>(j);
      //std::cout << xyz << std::endl;
      g_ptr_array_index (vertices, i+j) = 
        gts_vertex_new(gts_vertex_class(), xyz.x, xyz.y, xyz.z);
    }
  }

  // create triangle enclosing all the vertices
  GSList * list = NULL;
  for (guint i = 0; i < vertices->len; i++)
    list = g_slist_prepend(list, g_ptr_array_index(vertices, i));
  GtsTriangle* t = gts_triangle_enclosing(gts_triangle_class(), list, 100.);
  g_slist_free(list);

  GtsVertex *v1, *v2, *v3;
  gts_triangle_vertices (t, &v1, &v2, &v3);

  // create surface with one face: the enclosing triangle 
  GtsSurface* surface = gts_surface_new(gts_surface_class(),
  			     gts_face_class(), gts_edge_class (), gts_vertex_class());
  gts_surface_add_face(surface, gts_face_new(
      gts_face_class(), t->e1, t->e2, t->e3));

  // add vertices 
  for (guint i = 0; i < vertices->len; i++) {
    GtsVertex* v1 = (GtsVertex*)g_ptr_array_index(vertices, i);
    GtsVertex* v = gts_delaunay_add_vertex(surface, v1, nullptr);
    assert(v != v1);

    if (v) 
      gts_vertex_replace(v1, v);
  }
  g_ptr_array_free(vertices, TRUE);

  // destroy enclosing triangle 
  gts_allow_floating_vertices = TRUE;
  gts_object_destroy(GTS_OBJECT(v1));
  gts_object_destroy(GTS_OBJECT(v2));
  gts_object_destroy(GTS_OBJECT(v3));
  gts_allow_floating_vertices = FALSE;

  if (!keepHull)
    gts_delaunay_remove_hull(surface);

  //if (removeHoles)
  //  gts_delaunay_remove_holes(surface);

  if (conform) {
    guint encroachedNumber = gts_delaunay_conform(surface, steiner_max, 
                    (GtsEncroachFunc)gts_vertex_encroaches_edge, nullptr);
    if (0 == encroachedNumber && refine) {
      gpointer data[2] = {&quality, &area};
      guint unrefinedNumber = gts_delaunay_refine(surface, steiner_max,
      			     (GtsEncroachFunc)gts_vertex_encroaches_edge, nullptr,
			         (GtsKeyFunc)triangle_cost, data);
      if (verbose && unrefinedNumber > 0)
	    std::cerr << "delaunay: ran out of Steiner points during refinement\n";
    }
    else if (verbose && encroachedNumber > 0)
      std::cerr << "delaunay: ran out of Steiner points during conforming\n";
  }

  if (verbose) 
    gts_surface_print_stats(surface, stderr);

  if (checkDelaunay && gts_delaunay_check(surface))
    std::cerr << "delaunay: triangulation is not Delaunay\n";

  const std::string gtsFile("D:\\home\\workspace\\TestData\\PointClouds\\TIN\\points_small.gts");
  FILE* fp = fopen(gtsFile.c_str(), "wt");
  gts_surface_write(surface, fp);   // write triangulation
  fclose(fp);

  return 0;
}
