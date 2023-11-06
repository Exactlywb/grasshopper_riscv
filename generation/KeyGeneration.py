import os
import argparse

'''
os.urandom(k) generates k random bytes, using entory of operation system.
As Key consist of two blocks of 16 bytes, we generate 16*2 random bytes and write it into file.
'''
def GenKey():
    BLOCK_SIZE = 16
    double_random_block = os.urandom(BLOCK_SIZE*2)
    return double_random_block

def main():
    '''
    Argparse is used to provide to set the name of output file.
    If user doesn't set the option, default name out output file will be "key"
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", type=str, help="The name of file used to store result")
    args = parser.parse_args()

    if args.file != None:
        store_file_name = args.file
    else:
        store_file_name = "key"
    
    with open(store_file_name, 'wb') as fd:
        fd.write(GenKey())

if __name__ == '__main__':
    main()