import serial
import sys
import time


BAUD = 9600
PORT = '/dev/ttyAMA0'


class Uart:
    def receive_uart_data(self):
        received_data = ''

        while True:
            port = serial.Serial(PORT, BAUD)
            received_data += port.read(1)
            received_data += port.read(port.inWaiting())
            if len(received_data) > 12:
                return received_data

    def send_uart_data(self, package):
        port = serial.Serial(PORT, BAUD)
        port.write(package)

    # Date Structure:
    # <barcode><UPC_CODE><NAME><SERVINGS><DATE_IN>
    def create_barcode_package(self, barcode, name, servings):
        return '///<barcode><%s><%s><%s><%s>*/' % (barcode, name, servings, time.strftime('%m/%d/%Y'))
