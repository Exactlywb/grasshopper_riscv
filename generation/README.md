### Key generation
For now you need to generate random key for encryption.
Use
```colsol
python3 KeyGeneration.py
```
to generate key. It will contain in file __key__ in your current directory. 

__Be careful! Save this key somewhere. If you will loose your key, you will not be able to decrypt your text!__

To encrypt text, pass the file with key to main programm as a command line parametr.

If you want store random set of byte to the file with custom name, you can use option _-f_ or 
_--file_ to set custom file name.

```consol
python3 KeyGeneration.py -f file_name
```
```consol
python3 KeyGeneration.py --file file_name
```

In this case, your key will be store in current directory in the file with name, you have set.