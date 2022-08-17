import os
import time

base = '/home/carson/libs/test_funcs/output/'

counter = 0
while counter < 10:
    filename = str(counter).zfill(2) + '.mp4'
    dir_objs = os.listdir(base)
    if filename not in dir_objs:
        filename = base + filename
        print("Creating file {}".format(filename))
        with open(filename, 'w') as emptyfile:
            pass
        time.sleep(1)
    else:
        print("File {} exists...".format(filename))
    counter += 1

            
