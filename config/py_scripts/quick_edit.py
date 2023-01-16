import yaml
import json
import sys
import subprocess
from colorama import Fore, Style

if len(sys.argv) == 1: sys.exit(0)

yaml_data = None

REQUIRED = [
    'os_type',
    'bin_folder',
    'has_second_stage',
    'second_stage_entry_point',
    'second_stage_bin_o_filename',
    'second_stage_bin_filename',
    'second_stage_source_code_file',
    'kernel_entry_point',
    'kernel_o_binary',
    'kernel_bin_filename',
    'kernel_addr',
    'kernel_source_code_file'
]

with open('../boot.yaml', 'r') as file:
    yaml_data = yaml.full_load(file)
    file.close()

# Check and make sure data is correct
if len(yaml_data) < len(REQUIRED) or len(yaml_data) > len(REQUIRED):
    print('Error:\n\tToo much(or too little) data was given in `boot.yaml`.')
    sys.exit(1)
index = 0
for i in yaml_data:
    if not i == REQUIRED[index]:
        print(f'\n{Fore.RED}Error{Fore.WHITE}:\n\t{"The data that was needed after" if index >= 1 else "The data that was needed was"}\n\t{Style.BRIGHT+Fore.CYAN+"`"}{REQUIRED[index-1] if index >= 1 else REQUIRED[index]}{"`"+Style.RESET_ALL} {"was" if index >= 1 else ""}{Style.BRIGHT+Fore.CYAN+" `" if index >= 1 else ""}{REQUIRED[index] if index >= 1 else ""}{"`"+Style.RESET_ALL if index >= 1 else ""}\n\t{"Instead got" if index >= 1 else ""}{Style.BRIGHT+Fore.CYAN+" `"}{i if index >= 1 else ""}{"`" if index >= 1 else ""}\n\n{Style.RESET_ALL}')
        sys.exit(1)
    index+=1

if sys.argv[1] == '-gdt_ideals':
    gdt_ideals_s_file_data = ''
    with open('protocol/gdt/gdt_ideals.s', 'r') as file:
        gdt_ideals_s_file_data = file.read()
        file.close()

    gdt_ideals_s_file_data = gdt_ideals_s_file_data.replace('%%', '%')

    # Kernel address
    kernel_addr = str(hex(yaml_data['kernel_addr']*16))
    kernel_addr = kernel_addr.replace('0x', '')

    with open('protocol/gdt/gdt_ideals.s', 'w') as file:
        file.write(gdt_ideals_s_file_data.format(kernel_addr))
        file.close()