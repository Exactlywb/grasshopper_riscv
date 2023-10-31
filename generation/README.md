### Key generation

#### Introducing

For now you need to generate random key for encryption. Use
```bash
python3 KeyGeneration.py
```
to generate key. It will output the binary key in file `key` in the directory
from where script has been run.

#### Advanced options

If you want store generated key to the file with custom path, you may use `-f`
or `--file` options. E.g.:

```bash
python3 KeyGeneration.py -f file_name
```

or

```bash
python3 KeyGeneration.py --file file_name
```

### Tables generation

#### Introducing

In the encryption and decryption algorithms we use tables with magic numbers.
This numbers represent a mathematical coefficients for operations in Galois
field to make mathematical operations easier. Of course, it is possible to
evaluate content of this tables at the program runtime, but in this case we
would like to decrease the perfomance.

Note, that we generate this tables once for simple implementation and for
vectorized one. Hence, this trick does not influence on comparison of perfomance
of both versions.

Do not run it by yourself. Seriously. Only for maintainers.

