import json
import sys
import urllib2

from PyQt4 import QtGui, uic
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

        self.received_barcode.textChanged.connect(self.get_barcode)
        #self.connect(self.received_barcode, SIGNAL('textChanged(QString)'), self.get_barcode())

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
        try:
            barcode = self.received_barcode.text()
        except KeyboardInterrupt:
            sys.exit(1)
        if barcode is '':
            return
        upc_dictionary = self.lookup_barcode(barcode)
        self.name = self.get_item_name(upc_dictionary)
        if self.name is None:
            self.update_text(self.ui.item_name, 'Item name not found')
        else:
            self.update_text(self.ui.item_name, self.name)
            package = self.uart.create_package(barcode, self.name)
            self.update_text(self.ui.serving_prompt, 'Enter Servings')
            self.update_text(self.ui.servings_input, '')
            self.uart.send_uart_data(package)

    def get_servings(self):
        pass

    def update_text(self, widget, text):
        widget.setText(text)
