import json
import os
import sys
import time
import urllib2

from PyQt4 import QtGui, QtCore, uic
from uart_class import Uart


UPC_DATABASE_URL = 'http://api.upcdatabase.org/json/'
UPC_LOOKUP_API_KEY = '85043ef8601462a39e97cee08c9baa5b/'


class Ui_MainWindow(QtGui.QMainWindow):
    def __init__(self):
        super(Ui_MainWindow, self).__init__()
        self.ui = uic.loadUi('my_grocery_pal.ui', self)
        self.show()
        self.uart = Uart()
        self.name = ''
        self.barcode = ''

        self.received_barcode.textChanged.connect(self.get_barcode)

        self.uart_timer = QtCore.QTimer()
        self.uart_timer.timeout.connect(self.get_servings)

        self.screen_timeout = QtCore.QTimer()
        self.screen_timeout.timeout.connect(self.blank_screen)
        self.screen_timeout.start(120000)

    def get_item_name(self, upc_dictionary):
        try:
            item_name = upc_dictionary['itemname']
            description = upc_dictionary['description']
            if item_name == '':
                return description.title()
            else:
                return item_name.title()
        except KeyError:
            return None

    def lookup_barcode(self, barcode):
        # API doesn't accept the first int in the barcode
        barcode = barcode[1:]
        api_request = urllib2.Request('%s%s%s' % (UPC_DATABASE_URL,
                                                  UPC_LOOKUP_API_KEY,
                                                  barcode))
        upc_database = urllib2.urlopen(api_request).read()
        return json.loads(upc_database)

    def get_barcode(self):
        self.screen_timeout.stop()
        try:
            barcode = self.received_barcode.text()
        except KeyboardInterrupt:
            sys.exit(1)
        if barcode is '':
            return
        self.barcode = barcode
        upc_dictionary = self.lookup_barcode(barcode)
        self.name = self.get_item_name(upc_dictionary)
        if self.name is None:
            self.update_text(self.ui.item_name, 'Item name not found')
        else:
            self.update_text(self.ui.item_name, self.name)
            self.update_text(self.ui.serving_prompt, 'Enter Servings')
            self.update_text(self.ui.servings_input, '')
            self.uart_timer.start(200)

    def get_servings(self):
        self.screen_timeout.stop()
        servings_package = self.uart.receive_uart_data()
        self.uart_timer.stop()
        number = servings_package.split('>')[1][1:]
        self.update_servings_display(number)
        time.sleep(0.25)
        self.uart_timer.start(200)
        self.screen_timeout.start(120000)

    def update_servings_display(self, number):
        if number is not '*' and number is not '#':
            current = self.ui.servings_input.text()
            self.ui.servings_input.setText('%s%s' %(current, number))
        elif number is '*':
            current = self.ui.servings_input.text()
            if len(current) > 0:
                self.ui.servings_input.setText(current[:-1])
        elif number is '#':  # Send via UART
            self.uart_timer.stop()
            current = self.ui.servings_input.text()
            if len(current) is 0:
                current = 1
            uart_package = self.uart.create_barcode_package(self.barcode, self.name, current)
            self.uart.send_uart_data(uart_package)

    def update_text(self, widget, text):
        widget.setText(text)

    def blank_screen(self):
        os.system('xset dpms force off')
