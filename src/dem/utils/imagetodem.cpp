/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "dem/utils/imagetodem.h"

using namespace earthxpert;

ImageToDem::ImageToDem(ISensorModel const* const model, 
  IDem const* const dem) : mModel(model), mDem(dem) {
  Initialize();
}

void ImageToDem::Initialize() {

}
