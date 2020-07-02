/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaiepipolar.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace bpo = boost::program_options;

/**
 *  The console program epipolar generates epipolar or pseudo epipolar images
 *  from a pair of overlapping images acquired by the same sensor.
 *
 *  \detail Epipolar geometry is from the geometry of stereo vision. When a camera
 *  views a scene from two or more distinct positions, there are a number of
 *  geometric relations between the 3D surface and its projections onto the
 *  2D images that lead to constraints between the image pixels.
 *
 *  usage:  epipolar.exe [options]
 *      --sensorname  <frame|rational>
 *      --prjdir      [path_of_project_directory_or_file]
 *      --leftimg     <path_of_left_image_file>    // "images/img01.tif"
 *      --leftmdl     <path_of_left_model_file>    // "images/img01_rpc.tif"
 *      --rightimg    <path_of_right_image_file>  // "images/img02.tif"
 *      --rightmdl    <path_of_right_model_file>  // "images/img02_rpc.tif"
 *      --bands       [list_of_band_ids]             // "1,2,3"
 *      --sample      [downsample_factor]           // 2
 *      --outdir      [path_of_output_directory]    // "d:/out/"
 *      --resample    [bilinear|cubic|gaussian|sinc]
 *      --memsize     [memory_size_in_MB]          // 4000
 *
 *  examples:
 *
 *    exepipolar.exe --sensorname="rational" --leftimg="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034657-P2AS-052522862010_01_P001.TIF" --leftmdl="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034657-P2AS-052522862010_01_P001.RPB" --rightimg="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.TIF" --rightmdl="D:/home/workspace/TestData/Imagery/Satellite/052522862010_01_P001_PAN/09JAN15034801-P2AS-052522862010_01_P001.RPB" --bands=1 --sample=1 --outdir="D:/home/workspace/build/windows/vs2019/tests/output/" --resample="cubic" --memsize=4000
 *    exepipolar.exe --sensorname="rational" --leftimg="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000.tif" --leftmdl="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000_rpc.txt" --rightimg="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000.tif" --rightmdl="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000_rpc.txt" --bands=1 --sample=1 --outdir="D:/home/workspace/build/windows/vs2019/tests/output/" --resample="cubic" --memsize=4000
 */
int main(int argc, char** argv) {
  earthxpert::ExaiEpipolar cmd;

  cmd.SetCmdArgs(argc, argv);
  cmd.Execute();
  return 0;
}
