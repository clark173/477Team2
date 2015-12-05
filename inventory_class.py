import datetime
import os


INVENTORY_DIR = '/home/pi/inventory/'

class Inventory:
    # File Structure:
    # NAME
    # SERVINGS
    # DATE_IN
    # DATE_EXP
    def update_file(self, filename, servings, date, name, in_out):
        filename = '%s%s.txt' %(INVENTORY_DIR, filename)

        if not os.path.isfile(filename):
            if in_out is 'IN':
                f = open(filename, 'w')
                f.write('%s\n%s\n%s\n' %(name, servings, date))
                return servings
            return 0
        else:
            f = open(filename, 'r')
            f.readline()
            current_servings = int(f.readline())
            f.close()
            f = open(filename, 'w')
            if (in_out == 'IN'):
                new_servings = servings + current_servings
                print new_servings
                f.write('%s\n%s\n%s\n' %(name, new_servings, date))
            else:
                new_servings = current_servings - int(servings)
                if new_servings < 0:
                    new_servings = 0
                f.write('%s\n%s\n%s\n' %(name, new_servings, date))
            f.close()
            return new_servings
