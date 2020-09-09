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
class EXGUI_EXPORTS SingleInstance {
public:
  SingleInstance(const QString& key);
  SingleInstance(const SingleInstance&) = delete;
  SingleInstance(SingleInstance&&) = delete;
  ~SingleInstance();

  SingleInstance& operator=(const SingleInstance&) = delete;
  SingleInstance& operator=(SingleInstance&&) = delete;

  bool TryToRun();
  bool IsRunning();
  void Release();

private:
  const QString mKey;
  const QString mSharedMemKey;
  const QString mMemLockKey;

  QSharedMemory mSharedMem;
  QSystemSemaphore mSemaphore;
};
}
