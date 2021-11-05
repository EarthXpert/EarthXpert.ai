/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "packages/exai/exaiortho.h"

/**
 *  The console program exortho generates ortho images.
 *
 *  \detail 
 *
 *  usage:  exortho.exe [options]
 *      --sensorname  <frame|rational>              // "rational"
 *      --prjdir      [path_of_project_dir_or_file]
 *      --image       <path_of_image_file>     		// "images/img01.tif"
 *      --model       <path_of_left_model_file>     // "images/img01_rpc.tif"
 *      --bands       [list_of_band_ids]            // "1,2,3"
 *      --dem         [path_of_dem_file]            // "dem.tif"
 *      --stride      [stride_interval]             // 1
 *      --rotangle    [angle_of_rotation]
 *      --resample    [linear|cubic|gaussian|sinc]  // "cubic"
 *      --outdir      [path_of_output_directory]    // "d:/out/"
 *      --outsrs      [srs_of_ortho]                // "4326" EPSG code, wkt
 *      --ulpos       [ul_corner_of_ortho]          // "5,5"
 *      --outgsd      [gsd_of_ortho]                // "0.5,0.5"
 *      --memsize     [memory_size_in_MB]           // 4000
 *
 *  examples:
 *
 *    exortho.exe
 */
int main(int argc, char** argv) {
  earthxpert::ExaiOrtho cmd;

  cmd.SetCmdArgs(argc, argv);
  cmd.Execute();
  return 0;
}
