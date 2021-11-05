/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "gui/qt/singleinstance.h"
#include "3rdparty/qt/exqt.h"

using namespace earthxpert;

SingleInstanceQt::SingleInstanceQt(const QString& key)
  : mKey(key + "_SingleInstanceQt"), mSharedMem(mKey) {
}

SingleInstanceQt::~SingleInstanceQt() {
  Release();
}

bool SingleInstanceQt::TryToRun() {
  if (IsRunning())   // Extra check
    return false;

  const bool res = mSharedMem.create(sizeof(quint64));
  if (!res) {
      Release();
      return false;
  }

  return true;
}

bool SingleInstanceQt::IsRunning() {
  if (mSharedMem.isAttached())
    return false;

  const bool isRunning = mSharedMem.attach();
  if (isRunning)
    mSharedMem.detach();

  return isRunning;
}

void SingleInstanceQt::Release() {
  if (mSharedMem.isAttached())
    mSharedMem.detach();
}
