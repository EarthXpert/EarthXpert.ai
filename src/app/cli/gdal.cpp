/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "packages/gdal/gdaljob.h"

/**
 *  The console program gdal provides a suite of commands for processing 
 *  raster and vector data in numerous formats.
 *
 *  Usage: exgdal <command> <command options>
 *    --command        The GDAL command
 *    --help, -h       Display help text
 *    --list-commands  List available commands
 *    --version        Show program version
 *
 *    Commands :
 *      info
 *      srsinfo
 *      translate
 *      ogrinfo
 *
 *  examples:
 *
 *    exgdal info D:/home/workspace/TestData/Imagery/image_small.tif
 */
int main(int argc, char* argv[]) {
  earthxpert::GdalJob job;

  job.SetCmdArgs(argc, argv);
  return job.Run();
}
