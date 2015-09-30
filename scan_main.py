import json
import serial
import sys
import time
import urllib2


BAUD = 115200
PORT = '/dev/ttyAMA0'

UPC_DATABASE_URL = 'http://api.upcdatabase.org/json/'
UPC_LOOKUP_API_KEY = '85043ef8601462a39e97cee08c9baa5b/'


# Data Structure:
# <UPC_CODE><NAME><SERVINGS><DATE_IN><IN/OUT>
def create_package(barcode, name, servings, in_out):
    return '<%s><%s><%s><%s><%s>\r\n' % (barcode, name, servings,
                                     time.strftime("%m/%d/%Y"), in_out)

def send_uart_data(package):
    port = serial.Serial(PORT, BAUD)
    port.write(package)

def get_item_name(upc_dictionary):
    try:
        item_name = upc_dictionary['itemname']
        description = upc_dictionary['description']
        if item_name == "":
            return description.title()
        else:
            return item_name.title()
    except KeyError:
        return None

def lookup_barcode(barcode):
    # API doesn't accept the first int in the barcode
    barcode = barcode[1:]
    api_request = urllib2.Request('%s%s%s' % (UPC_DATABASE_URL,
                                              UPC_LOOKUP_API_KEY,
                                              barcode))
    upc_database = urllib2.urlopen(api_request).read()
    return json.loads(upc_database)

if __name__ == "__main__":
    while True:
        try:
            barcode = raw_input('UPC-A Barcode: ')
        except KeyboardInterrupt:
            print '\nKeyboard Interrupt. Exiting Script'
            sys.exit(1)
        upc_dictionary = lookup_barcode(barcode)
        name = get_item_name(upc_dictionary)
        if name is None:
            print 'Barcode not found'
            send_uart_data('Barcode not found\r\n')
            continue
        package = create_package(barcode, name, 1, 'IN')
        send_uart_data(package)
