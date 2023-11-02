import matplotlib.pyplot as plt
from tabulate import tabulate
from os.path import exists
from os import system
from sys import exit
import numpy as np
import subprocess
import argparse
import json
import re

'''
This assosiation arrays (dicts) set the paths to data files.
If we will need to use enother files or, store it in anothe place,
we just need to change paths in this dict and don't change other code.
'''
dis_files = {"vec_dis" : "vec_dis",
             "sim_dis" : "sim_dis"}
stat_files = {"vec_stat" : "vec_stat",
              "sim_stat" : "sim_stat"}

json_config_file_path = "config.json"

'''
This is a list of function we compare instruction in both implementations.
'''
function_list = ["apply_x", "apply_ls", "apply_inv_l"]

'''
Find in dis file dis_file_path labels of function with name function_name
'''
def get_list_of_function_lables(dis_file_path: str, function_name: str) -> list:
    with open(dis_file_path) as f:
        content = list(filter(None, f.read().split('\n')))

    flag = 0
    tmp = []
    for el in content:
        if function_name not in el and flag == 0:
            continue
        elif flag == 0: # TODO: review using regex.
            flag = 1
            continue

        '''
        In python regex \S is a non space symbol
        '''
        match = re.search(r'\S+ <\S+>:', el)
        if match:
            break
        tmp.append(el)

    lables_list = []
    for item in tmp:
        s = " ".join(item.split())
        splited_line = s.split(" ")[:-1]
        # cause labels in dis file writed like:
        # label: ...
        # so, we don't need symbol ":"
        lables_list.append(splited_line[0][:-1])

    assert (len(lables_list) != 0)
    return lables_list

'''
Get the list of labels of some function, that we have got by function
get_list_of_function_lables.  Return the dict of this labels with amount of
calls of each label using information in stat_file_path.
'''
def get_labels_call_counts(function_name: str, dis_file_path: str, stat_file_path: str) -> dict:
    labels_list = get_list_of_function_lables(dis_file_path, function_name)

    with open(stat_file_path) as f:
        content = list(filter(None, f.read().split('\n')))

    label_call_couts = {}
    for line in content:
        split_line = line.split(' ')
        label_name, call_count = split_line[0], split_line[1]

        if label_name in labels_list:
            label_call_couts[label_name] = int(call_count)

    assert(len(label_call_couts) != 0)
    return label_call_couts

'''
Return total amount of calls of all labels in dict.
'''
def get_function_instruction_call_count(function_name: str, dis_file_path: str, stat_file_path: str) -> int:
    labels_call_counts = get_labels_call_counts(function_name, dis_file_path, stat_file_path)
    res = 0
    for val in labels_call_counts.values():
        res += val
    return res


'''
Compare bar plot for one function
'''
def plot_one_function_innstructions_calls(function_name: str) -> None:
    vec_function_call_count = get_function_instruction_call_count(function_name, dis_files["vec_dis"], stat_files["vec_stat"])
    sim_function_call_count = get_function_instruction_call_count(function_name, dis_files["sim_dis"], stat_files["sim_stat"])

    fig = plt.figure()

    plt.bar(0, vec_function_call_count, label='vectorized')
    plt.text(0, vec_function_call_count, vec_function_call_count, ha='center')

    plt.bar(1, sim_function_call_count, label='simple')
    plt.text(1, sim_function_call_count, sim_function_call_count, ha='center')

    # make x ax ticks invisible
    plt.xticks([])
    plt.ylabel("Operations number")
    plt.title(f"Compare operations number in function {function_name}")
    plt.legend()
    fig.savefig(f'./images/one_function_innstructions_calls.png')

'''
Add value labels
'''
def add_bar_labels(obj_x, obj_y) -> None:
    for x, y in zip(obj_x, obj_y):
        plt.text(x, y, y, ha='center')

'''
Compare bar plot for all function in the list
'''
def plot_functions_instructions_calls(functions_name_list: list) -> None:
    # make x labels for bar plot + some magic for plot view
    x_ticks = np.arange(0, len(functions_name_list)*5, 5)
    standart_width = 2

    # make an arrays of function calls for every function for vectorized and simple implementations
    vec_functions_calls_list = []
    sim_functions_calls_list = []
    for function_name in functions_name_list:
        vec_functions_calls_list.append(get_function_instruction_call_count(function_name,
                                                                            dis_files["vec_dis"],
                                                                            stat_files["vec_stat"]))
        sim_functions_calls_list.append(get_function_instruction_call_count(function_name,
                                                                            dis_files["sim_dis"],
                                                                            stat_files["sim_stat"]))

    fig = plt.figure(figsize=(21, 7))
    plt.bar(x_ticks - standart_width/2, vec_functions_calls_list, label='vectorized', width=standart_width)
    add_bar_labels(x_ticks - standart_width/2, vec_functions_calls_list)

    plt.bar(x_ticks + standart_width/2, sim_functions_calls_list, label='simple', width=standart_width)
    add_bar_labels(x_ticks + standart_width/2, sim_functions_calls_list)

    plt.xticks(x_ticks, functions_name_list)
    plt.xlabel("function names")
    plt.ylabel("Operations number")
    plt.title("Compare operations number in all functions")
    plt.legend()
    fig.savefig(f'./images/functions_instructions_calls.png')

'''
Plot tables of comparison
'''
def plot_compare_table(functions_name_list: list) -> None:
    # make an arrays of function calls for every function for vectorized and simple implementations
    vec_functions_calls_list = []
    sim_functions_calls_list = []
    for function_name in functions_name_list:
        vec_functions_calls_list.append(get_function_instruction_call_count(function_name,
                                                                            dis_files["vec_dis"],
                                                                            stat_files["vec_stat"]))
        sim_functions_calls_list.append(get_function_instruction_call_count(function_name,
                                                                            dis_files["sim_dis"],
                                                                            stat_files["sim_stat"]))

    table_content = []
    for i in range(len(functions_name_list)):
        function_content_row = []
        function_content_row.append(functions_name_list[i])
        function_content_row.append(vec_functions_calls_list[i])
        function_content_row.append(sim_functions_calls_list[i])
        function_content_row.append(round(sim_functions_calls_list[i] / vec_functions_calls_list[i], 2))
        table_content.append(function_content_row)

    head = ['Function', 'Vectorized instruction count', 'Simple instruction count', 'Boost']
    # display table
    print(tabulate(table_content, headers=head, tablefmt="grid"))

'''
for parametr criteria possible valus 'ticks' 'cycles' 'instructions'
'''
def plot_total_ticks(vec_stat_file: str, sim_stat_file: str, criteria='ticks') -> None:
    with open(vec_stat_file) as f:
        vec_stat_file_content = list(filter(None, f.read().split('\n')))
    with open(sim_stat_file) as f:
        sim_stat_file_content = list(filter(None, f.read().split('\n')))

    vec_impl_measure = 0
    sim_impl_measure = 0
    for line in vec_stat_file_content:
        if criteria in line:
            s = " ".join(line.split())
            splited_line = s.split(" ")
            assert(splited_line[1] == criteria)
            vec_impl_measure = int(splited_line[0])
            break
    for line in sim_stat_file_content:
        if criteria in line:
            s = " ".join(line.split())
            splited_line = s.split(" ")
            assert(splited_line[1] == criteria)
            sim_impl_measure = int(splited_line[0])
            break

    assert(vec_impl_measure != 0)
    assert(sim_impl_measure != 0)

    fig = plt.figure()
    plt.bar(0, vec_impl_measure, label='vectorized')
    plt.text(0, vec_impl_measure, vec_impl_measure, ha='center')

    plt.bar(1, sim_impl_measure, label='simple')
    plt.text(1, sim_impl_measure, sim_impl_measure, ha='center')

    # make x ax ticks invisible
    plt.xticks([])
    plt.ylabel(f"Total {criteria} number in programms")
    plt.title(f"Compare total {criteria} number in programm")
    plt.legend()
    fig.savefig(f'./images/total_ticks.png')

def run_child_proces(cmd: str) -> bool:
    ret = system(cmd)
    while ret == None:
        continue
    return True

def prepare_data_files(json_config_path: str, key_path: str) -> None:
    if not exists(json_config_path):
        print(f"[-] Error, file {json_config_path} is not exits")
        exit(1)

    with open(json_config_path) as json_data:
        data = json.load(json_data)

    # check json file content
    assert(len(data) != 0)
    for val in data.values():
        assert(len(val) != 0)

    # check if we have compiled binaries
    if not exists('../build/bin/simple'):
        print(f"[-] Error, file ../build/bin/simple is not exits")
        exit(1)

    if not exists('../build/bin/vectorized'):
        print(f"[-] Error, file ../build/bin/vectorized is not exits")
        exit(1)

    try:
        # make dis files
        print("[info] objdmp simple impl disasm")
        run_child_proces(f'{data["riscv-objdmp"]} -d ../build/bin/simple > {dis_files["sim_dis"]}')
        print("[info] objdmp vector impl disasm")
        run_child_proces(f'{data["riscv-objdmp"]} -d ../build/bin/vectorized > {dis_files["vec_dis"]}')
        # make stat files
        print("[info] emulate vec prog run")
        run_child_proces(f'{data["spike"]} -g --isa=rv64gcv {data["pk"]} -s ../build/bin/vectorized ../testsuite/1.test {key_path} > {stat_files["vec_stat"]} 2>&1')
        print("[info] emulate simple prog run")
        run_child_proces(f'{data["spike"]} -g --isa=rv64gcv {data["pk"]} -s ../build/bin/simple ../testsuite/1.test {key_path} > {stat_files["sim_stat"]} 2>&1')
    except RuntimeError:
        print("[-] Error. You have cought RuntimeError. That means there are some troubles not connected with this script.")
        print("There are some possible ways you should check now:\n\t- check if you have compiled prpgramm with nesessary sompiler\n\t- check if your programm work correct\n\t- check if you have risv-objdmp, spike and pd on your system\n\t- check if you have set correct paths to this utils in the json config")
        exit(1)

def main():
    if not exists("./images"):
        system("mkdir images")
    if not exists("./data"):
        system("mkdir data")

    # parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--table", help="This option enable to output table with general statistics information", action="store_true")
    parser.add_argument("-m", "--measure", type=str, help="Measure of total operations in programms. It's possible three measure: ticks, cycles and instructions. By default ticks. Please, use only one of this values for this option.")
    parser.add_argument("-d", "--dir", type=str, help="Path to the directory with data files. By default ./data. Please, be careful if you set  this directory by hand.")

    parser.add_argument("-p", "--preparation", help="This option enable to prepare data for experiment. In this instance it's run objdump and spike to collect data. If you already have experimental data, don't use this flag. Be careful, if you want to use this flag, check README.md firstly and fill config.json correct.", action="store_true")
    parser.add_argument("-k", "--key_path", type=str, help="This flag provede to set path to the key. You may be need to use this flag only if you use -p option and you have stored your secret key in non default path. By default key path is ../generation/key")
    args = parser.parse_args()

    # set it default value if arg is not seted
    if args.dir == None:
        args.dir = './data/'

    # check if this path is exist
    if not exists(args.dir):
        print(f"[-] Error, path {args.dir} is not exits")
        exit(1)

    for key in dis_files.keys():
        dis_files[key] = args.dir + dis_files[key]
    for key in stat_files.keys():
        stat_files[key] = args.dir + stat_files[key]

    # if you set special option, you can prepare experimental data with spike and pk utils
    if args.preparation != False:
        key_path = '../generation/key'
        if args.key_path != None:
            key_path = args.key_path

        if not exists(key_path):
            print(f"[-] Error, file {key_path} is not exits. Please generate it, or set another path to your key by option -k.")
            exit(1)
        prepare_data_files(json_config_file_path, key_path)

    #? should we use absolute path?
    #? how to set data files in programm?
    # check dis files exit
    for file in dis_files.values():
        if not exists(file):
            print(f"[-] Error, file {file} is not exits")
            exit(1)

    # check stat files exit
    for file in stat_files.values():
        if not exists(file):
            print(f"[-] Error, file {file} is not exits")
            exit(1)

    # set it ticks if arg is not seted
    if args.measure == None:
        args.measure = 'ticks'
    # check if parametr criteria is possible
    assert (args.measure in ['ticks', 'cycles', 'instructions'])

    # plots
    plot_one_function_innstructions_calls(function_list[2])
    plot_functions_instructions_calls(function_list)
    plot_total_ticks(stat_files['vec_stat'], stat_files['sim_stat'], criteria=args.measure)

    if args.table != False:
        plot_compare_table(function_list)

if __name__ == '__main__':
    main()
