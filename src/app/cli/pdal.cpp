/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/pdal/pdaljob.h"
#include <boost/program_options.hpp>

/**
 *  The console program pdal provides a suite of commands for processing point clouds.
 * 
 *  Usage: expdal <options>
 *         expdal <command> <command options>
 *    --command        The PDAL command
 *    --debug          Sets the output level to 3 (option deprecated)
 *    --verbose, -v    Sets the output level(0 - 8)
 *    --drivers        List available drivers
 *    --help, -h       Display help text
 *    --list-commands  List available commands
 *    --version        Show program version
 *    --options        Show options for specified driver(or 'all')
 *    --log            Log filename(accepts stderr, stdout, stdlog, devnull as special cases)
 *    --logtiming      Turn on timing for log messages
 *  
 *    Commands :
 *      delta
 *      density
 *      fauxplugin
 *      ground
 *      hausdorff
 *      info
 *      merge
 *      pipeline
 *      random
 *      sort
 *      split
 *      tile
 *      tindex
 *      translate
 *  
 *  See http ://pdal.io/apps/ for more detail
 *
 *  examples:
 *
 *    expdal pipeline D:/home/workspace/TestData/PointClouds/points_small_pdal_pipeline.json
 */
int main(int argc, char* argv[]) {
  earthxpert::PdalJob job;

  job.SetCmdArgs(argc, argv);
  return job.Run();
}
