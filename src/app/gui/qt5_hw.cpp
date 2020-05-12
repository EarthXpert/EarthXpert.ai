/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "srcconfig.h"

#include "qapplication.h"
#include "qpushbutton.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  QPushButton hw("Hello world!", 0);
  hw.resize(100, 30);
  hw.show();

  return app.exec();
}
