/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "platform/explatform.h"
#include "3rdparty/gdal/exgdal.h"
#include "core/parallel/ijob.h"

namespace earthxpert {
/**
 *  \brief the GDAL utils and their C API offer a rich set of raster and vector CLIs, 
 *  and is wrapped for easier customization.
 */
class EXPLATFORM_EXPORTS GdalJob final : public IJob {
public:
  GdalJob();
  GdalJob(const GdalJob&) = delete;
  GdalJob(GdalJob&&) = delete;
  ~GdalJob() = default;

  GdalJob& operator=(const GdalJob&) = delete;
  GdalJob& operator=(GdalJob&&) = delete;

  int SetArgs();

  int Run() override;

private:
  std::string mCommand;
};
} // namespace earthxpert::thirdparty
