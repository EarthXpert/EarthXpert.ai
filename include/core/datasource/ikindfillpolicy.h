/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "core/wxcore.h"

namespace watxtract {
/**
 *  The interface IKindFillPolicy defines the margin filling behaviour.
 */
class WXCORE_EXPORTS IKindFillPolicy {
public:
  virtual ~IKindFillPolicy() {}

  /**
   *  \brief fill margin pixel on top side
   *
   *  \param [in] c col of margin pixel
   *  \param [in] r row of margin pixel
   */
  virtual void FillTopPixel(int c, int r) = 0;

  /**
   *  \brief fill margin pixel on bottom side
   */
  virtual void FillBottomPixel(int c, int r) = 0;

  /**
   *  \brief fill margin pixel on left side
   */
  virtual void FillLeftPixel(int c, int r) = 0;

  /**
   *  \brief fill margin pixel on right side
   */
  virtual void FillRightPixel(int c, int r) = 0;

  /**
   *  \brief fill margin pixel on top-left corner
   */
  virtual void FillTLPixel(int c, int r) = 0;

  /**
   *  \brief fill margin pixel on top-right corner
   */
  virtual void FillTRPixel(int c, int r) = 0;

  /**
   *  \brief fill margin pixel on bottom-left corner
   */
  virtual void FillBLPixel(int c, int r) = 0;

  /**
   *  \brief fill margin pixel on bottom-right corner
   */
  virtual void FillBRPixel(int c, int r) = 0;
};
}
