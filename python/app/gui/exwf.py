#  -----------------------------------------------------------------------------
#   Copyright (c) 2019.  All rights reserved.
#  -----------------------------------------------------------------------------

import os, sys, time
from exgui import Ui_MainWindow
from PyQt5 import QtWidgets


class ExGui():
  """
  
  Attributes:
    ui: 
    mianWindow: 
  """
  
  def __init__(self, argv):
    """Initializer
    
    Args:
      argv Specify list of command line arguments.
    """
      app = QtWidgets.QApplication(argv)

      self.ui = Ui_MainWindow()
      self.mainWindow = QtWidgets.QMainWindow()
      self.ui.setupUi(self.mainWindow)

      self.setTitle()
      self.configureActions()
      self.mainWindow.show()

      sys.exit(app.exec_())

  def configureActions(self):
    self.ui.actionExit.setStatusTip('Click to exit the application')  # use ui reference to update status bar
    self.ui.actionExit.triggered.connect(self.closeMainWindow)  # connect widget to method when triggered (clicked)

  def closeMainWindow(self):
    self.mainWindow.close()

  def setTitle(self):
    self.mainWindow.setWindowTitle('EarthXpert Workflow')


if __name__ == "__main__":
  """The script exwf starts the GUI for EarthXpert's workflow.

    usage:  exwf.py
              --help
            
    example:
  
    python exwf.py 
  """
  ExGui(sys.argv)
