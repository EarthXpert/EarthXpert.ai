/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "dem/idem.h"
#include "io/vector/ivectorreader.h"
#include <string>

namespace earthxpert {
  /**
   *  The class TinDem obtains elevations from a digital elevation model
   *  in TIN format.
   */
  class EXDEM_EXPORTS TinDem final : public IDem{
  public:
    TinDem(std::string demFile, int layer);
    ~TinDem() = default;

    /**
     *  \return Returns pointer to the vector reader.
     */
    IVectorReader* GetDemReader() const {
      return mDemReader.get();
    }

    /**
     *  \brief set the SRS of the interface
     */
     //void SetSrsIF();

     float GetElev(const Point2d& xy) const override;
     float GetElev(double x, double y) const override {
       return GetElev(Point2d(x, y));
     }

     /**
      *  \brief Fetch the ground coordinate system where the DEM operates.
      *
      *  \return Returns pointer to a coordinate system
      */
     UPSpatialRefSys GetCoordinateSystem() const override;

   private:
     std::string mDemFile;
     UPIVectorReader mDemReader;

     void Initialize();
  };

  using UPTinDem = std::unique_ptr<TinDem>;
  using SPTinDem = std::shared_ptr<TinDem>;
}
