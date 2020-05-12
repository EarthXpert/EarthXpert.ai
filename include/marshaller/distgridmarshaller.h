/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/wxmarshaller.h"
#include "sensormodel/distortiongrid.h"

#include <istream>
#include <ostream>

namespace watxtract {

/**
 *  Marshall distortion grid files.
 *
 *  \detail An example grid file in The Z/I format is given below:
 *
 *  \codeblock
file_version: 1

begin gridgroup "Image deformation grids"
enabled:     yes
order_index: 0
camera_name: "DIGITAL"
description: "BINGO self calibration grid - from all additional parameters"

begin grid "Image deformation grid 1"
    enabled:       yes
    inverse_scale: 1.000000
    columns:          41
    rows:             41
    description:   "Created by program BgExport"

    begin gridpoints
           -59.6988    -59.6988    -60.0000    -60.0000
           -56.7791    -59.7674    -57.0000    -60.0000
           -53.8386    -59.8207    -54.0000    -60.0000
           ......
            53.8386     59.8207     54.0000     60.0000
            56.7791     59.7674     57.0000     60.0000
            59.6988     59.6988     60.0000     60.0000
    end gridpoints
end grid
end gridgroup
 */
class WXMARSHALLER_EXPORTS DistGridMarshaller {
public:
  static void Serialize(const std::string &gridFile,
                        DistortionGridF const *const distGrid);
  static void Serialize(std::ostream &outStream,
                        DistortionGridF const *const distGrid);

  static std::unique_ptr<DistortionGridF> Deserialize(const std::string &gridFile);
  static std::unique_ptr<DistortionGridF> Deserialize(std::istream &inStream);
};
} // namespace watxtract
