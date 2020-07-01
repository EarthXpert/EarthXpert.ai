/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/exai/exaijob.h"
#include <boost/program_options.hpp>

/**
 *  The console program exai provides a suite of commands for photogrammetric
 *  computer vision problems.
 *
 *  Usage: exai <command> <command options>
 *    --command        The GDAL command
 *    --help, -h       Display help text
 *    --list-commands  List available commands
 *    --version        Show program version
 *
 *    Commands :
 *      epipolar
 *      tortho
 *
 *  examples:
 *
 *    exai epipolar --sensorname="rational" --leftimg="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000.tif" --leftmdl="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0000000_rpc.txt" --rightimg="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000.tif" --rightmdl="D:/home/workspace/TestData/Imagery/Satellite/po_162796_pan/po_162796_pan_0010000_rpc.txt" --bands=1 --sample=1 --outdir="D:/home/workspace/build/windows/vs2019/tests/output/" --resample="cubic" --memsize=4000
 */
int main(int argc, char* argv[]) {
  earthxpert::ExaiJob job;

  job.SetCmdArgs(argc, argv);
  return job.Run();
}
