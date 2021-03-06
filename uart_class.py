import serial
import os
import sys
import time


BAUD = 9600
PORT = '/dev/ttyAMA0'


class Uart:
    def receive_uart_data(self, keypad_ready=False):
        received_data = ''
        port = serial.Serial(PORT, BAUD)

        while True:
            received_data += port.read(1)
            received_data += port.read(port.inWaiting())
            if len(received_data) > 12:
                break

        data = received_data.split('<')
        if data[0].startswith('bluetooth'):
            os.system('xset dpms force off')

        port.close()
        if keypad_ready:
            return received_data
        else:
            return None

    def send_uart_data(self, package):
        port = serial.Serial(PORT, BAUD)
        port.write(package)
        port.close()

    # Date Structure:
    # <barcode><UPC_CODE><NAME><SERVINGS><DATE_IN>
    def create_barcode_package(self, barcode, name, servings):
        return '<barcode><%s><%s><%s><%s>*/' % (barcode, name, servings, time.strftime('%m/%d/%Y'))
