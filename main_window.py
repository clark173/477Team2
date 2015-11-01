import sys

from PyQt4 import QtGui, uic


class Ui_MainWindow(QtGui.QMainWindow):
    def __init__(self):
        super(Ui_MainWindow, self).__init__()
        uic.loadUi('my_grocery_pal.ui', self)
        self.show()
