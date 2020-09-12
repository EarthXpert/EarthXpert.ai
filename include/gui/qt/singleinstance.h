/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "gui/exgui.h"

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

namespace earthxpert {
class EXGUI_EXPORTS SingleInstanceQt {
public:
  SingleInstanceQt(const QString& key);
  SingleInstanceQt(const SingleInstanceQt&) = delete;
  SingleInstanceQt(SingleInstanceQt&&) = delete;
  ~SingleInstanceQt();

  SingleInstanceQt& operator=(const SingleInstanceQt&) = delete;
  SingleInstanceQt& operator=(SingleInstanceQt&&) = delete;

  bool TryToRun();
  bool IsRunning();
  void Release();

private:
  const QString mKey;
  const QString mSharedMemKey;
  QSharedMemory mSharedMem;
};
}
