/* ----------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rdparty/qt/exqt.h"

#include <QCryptographicHash>

namespace earthxpert {
/**
 *  \brief 
 */
QString QtHash(const QString& key, const QString& salt) {
  QByteArray data;
  data.append(key.toUtf8());
  data.append(salt.toUtf8());

  data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
  return data;
}
}
