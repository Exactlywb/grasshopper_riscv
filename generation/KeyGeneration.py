import os

'''
os.urandom(k) generates k random bytes, using entory of operation system.
As Key consist of two blocks of 16 bytes, we generate 16*2 random bytes and write it into file.
'''
def GenKey():
    BLOCK_SIZE = 16
    double_random_block = os.urandom(BLOCK_SIZE*2)
    return double_random_block

def main():
    with open('key', 'wb') as fd:
        fd.write(GenKey())

if __name__ == '__main__':
    main()