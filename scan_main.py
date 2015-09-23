import json
import urllib2


UPC_DATABASE_URL = 'http://api.upcdatabase.org/json/'
UPC_LOOKUP_API_KEY = '85043ef8601462a39e97cee08c9baa5b/'


def string_to_hex(name):
    hex_array = []
    for letter in name:
        hex_val = hex(ord(letter))
        hex_array.append(hex_val)
    return hex_array

def get_item_name(upc_dictionary):
    item_name = upc_dictionary['itemname']
    description = upc_dictionary['description']
    if item_name == "":
        return description.title()
    else:
        return item_name.title()

def lookup_barcode(barcode):
    api_request = urllib2.Request('%s%s%s' % (UPC_DATABASE_URL,
                                              UPC_LOOKUP_API_KEY,
                                              barcode))
    upc_database = urllib2.urlopen(api_request).read()
    return json.loads(upc_database)

if __name__ == "__main__":
    while True:
        barcode = raw_input('UPC-A Barcode: ')
        # API doesn't accept first int in UPC-A barcode
        upc_dictionary = lookup_barcode(barcode[1:])
        name = get_item_name(upc_dictionary)
        hex_array = string_to_hex(name)
