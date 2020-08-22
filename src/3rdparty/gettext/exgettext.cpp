/* ----------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.                          -
 * ----------------------------------------------------------------------
 */
#include "3rdparty/gettext/exgettext.h"

namespace earthxpert {

namespace thridparty {
/**
 * \brief set the i18n/l10n environment
 */
void ConfigureI18N_GNU(const int category, const std::string locale,
  const std::string& domain, const std::string& dir) {
  setlocale(category, locale.c_str());
  bindtextdomain(domain.c_str(), dir.c_str());
  textdomain(domain.c_str());
}
}
}