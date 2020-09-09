/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "gui/qt/singleinstance.h"
#include "3rdparty/qt/exqt.h"

using namespace earthxpert;

SingleInstance::SingleInstance(const QString& key)
  : mKey(key), mSharedMemKey(QtHash(key, "_sharedMemKey")),
    mMemLockKey(QtHash(key, "_memLockKey")), 
    mSharedMem(mSharedMemKey), mSemaphore(mMemLockKey, 1) {
  mSemaphore.acquire();
  {
    QSharedMemory fix(mSharedMemKey);  // Fix for *nix: http://habrahabr.ru/post/173281/
    fix.attach();
  }
  mSemaphore.release();
}

SingleInstance::~SingleInstance() {
  Release();
}

bool SingleInstance::TryToRun() {
  if (IsRunning())   // Extra check
    return false;

  mSemaphore.acquire();
  const bool result = mSharedMem.create(sizeof(quint64));
  mSemaphore.release();
  if (!result) {
      Release();
      return false;
  }

  return true;
}

bool SingleInstance::IsRunning() {
  if (mSharedMem.isAttached())
    return false;

  mSemaphore.acquire();
  const bool isRunning = mSharedMem.attach();
  if (isRunning)
    mSharedMem.detach();
  mSemaphore.release();

  return isRunning;
}

void SingleInstance::Release() {
  mSemaphore.acquire();
  if (mSharedMem.isAttached())
    mSharedMem.detach();
  mSemaphore.release();
}
