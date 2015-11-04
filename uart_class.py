import serial
import sys
import time


BAUD = 9600
PORT = '/dev/ttyAMA0'


class Uart:
    def receive_uart_data(self):
        current_char = ''
        previous_char = ''
        port = serial.Serial(PORT, BAUD)

        while previous_char not '*' and current_char not '/':
            previous_char = current_char
            current_char = port.read(1)

    def send_uart_data(self, package):
        port = serial.Serial(PORT, BAUD)
        port.write(package)

    # Date Structure:
    # <barcode><UPC_CODE><NAME><SERVINGS><DATE_IN>
    def create_barcode_package(self, barcode, name):
        return '///<barcode><%s><%s><%s>*/' % (barcode, name, time.strftime('%m/%d/%Y'))

    # Data Structure:
    # <servings><NUM_SERVINGS>
    def create_servings_package(self, servings):
        return '///<servings><%s>*/' %(servings)
