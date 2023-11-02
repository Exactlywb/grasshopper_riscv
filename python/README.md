## Run the experiment

This script is designed to provide performance data for both simple and
vectorized implementations, allowing for a comparison of their respective
performance. This script enables you to perform the comparison whether you
already possess disassembler (from objdump util) and statistic files (from
spike with `-g` option) for both implementations or not.

### 1. Collected data

In this case you already got necessary files: *sim_dis*, *vec_dis*, *sim_stat*
and *vec_stat*, that you have made using objdump util and spike.

In this case, you are free to run python script:
```bash
python3 main.py
```
you may find output images with plots in directory **images**.

### 2. Automatic data collection

#### Requirements

At the outset of the performance experiment, ensure that you have taken certain
preparatory steps:
- build your project with nesessary compiler and generate nesessary keys (e.g.
using KeyGeneration.py).
- make sure, that you have **spike** and **pk** utils on your PC.

#### Set paths to the utils

In directory python create file with name _config.json_.
```bash
cd python
touch config.json
```

In this file you need to set absolute paths to riscv-objdump util, spike and
pk, e.g.:
```json
{
    "riscv-objdmp" : "<abs path to riscv-objdump>",
    "spike" : "<abs path to spike>",
    "pk" : "<abs path to pk>"
}
```

After all requirements are set you are free to run python script with `-p`
option:
```bash
python3 main.py -p
```
In this instance script will first generate all necessary data (using
objdump and spike). Output is stored in *data* directory.

You are free to use the custom key (or specify the path to the generated key)
using `-k` option:
```bash
python3 main.py -p -k <abs path to key>
```

See `-h` or `--help` options to get more information.

