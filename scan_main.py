import json
import urllib2


UPC_DATABASE_URL = 'http://api.upcdatabase.org/json/'
UPC_LOOKUP_API_KEY = '85043ef8601462a39e97cee08c9baa5b/'


def get_item_name(upc_dictionary):
    item_name = upc_dictionary['itemname']
    description = upc_dictionary['description']
    if item_name == "":
        print description.title()
    else:
        print item_name.title()

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
        get_item_name(upc_dictionary)
