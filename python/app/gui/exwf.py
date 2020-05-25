#  -----------------------------------------------------------------------------
#   Copyright (c) 2019.  All rights reserved.
#  -----------------------------------------------------------------------------

from exgui import Ui_MainWindow
import os, sys, time
from PyQt5 import QtWidgets
    
'''
  The script exwf starts the GUI for EarthXpert's workflow.

  usage:  exwf.py
            --help
            
  example:
  
  python exwf.py 
'''

# *****************************************************************************
#  ExGui
# *****************************************************************************
##
# The class ExGui 
#
class ExGui():
  # *************************************************************************
  #   __init__()
  # *************************************************************************
  ## Initializer
  #
  # param[in] argv Specify list of command line arguments
  def __init__(self, argv):
    app = QtWidgets.QApplication(argv)
    
    self.ui = Ui_MainWindow()                           
    self.MainWindow = QtWidgets.QMainWindow()      
    self.ui.setupUi(self.MainWindow)        
    
    self.setTitle()
    self.configureActions()      
    self.MainWindow.show()
    
    sys.exit(app.exec_())
    
  # *************************************************************************
  #   configureActions()
  # *************************************************************************
  def configureActions(self):
    self.ui.actionExit.setStatusTip('Click to exit the application')    # use ui reference to update status bar
    self.ui.actionExit.triggered.connect(self.closeMainWindow)  # connect widget to method when triggered (clicked)
      
  # *************************************************************************
  #   closeMainWindow()
  # *************************************************************************
  def closeMainWindow(self): 
    self.MainWindow.close()
       
  # *************************************************************************
  #   setTitle()
  # *************************************************************************
  def setTitle(self):
    self.MainWindow.setWindowTitle('EarthXpert Workflow') 

 
# *****************************************************************************
# Main program.
# *****************************************************************************
if __name__ == "__main__":
  ExGui(sys.argv)   
