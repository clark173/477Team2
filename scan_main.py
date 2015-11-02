import sys

from main_window import Ui_MainWindow
from PyQt4 import QtCore, QtGui, uic


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = Ui_MainWindow()
    app.exec_()
