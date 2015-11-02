import serial
import sys
import time


BAUD = 9600
PORT = '/dev/ttyAMA0'


class Uart:
    def send_uart_data(self, package):
        port = serial.Serial(PORT, BAUD)
        port.write(package)

    # Date Structure:
    # <UPC_CODE><NAME><SERVINGS><DATE_IN>
    def create_package(self, barcode, name):
        return '///<%s><%s><%s>*/' % (barcode, name, time.strftime('%m/%d/%Y'))
