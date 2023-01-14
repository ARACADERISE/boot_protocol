import yaml
import json
import sys
from colorama import Fore, Style

yaml_data = None

REQUIRED = [
    'os_type',
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

# Getting second-stage linker format
ss_linker_format = ''
with open('config/formats/ss_linker_format', 'r') as file:
    ss_linker_format = file.read()
    file.close()

# Get second-stage binary information
ss_bin_data = None
with open('../'+yaml_data['second_stage_bin_filename'], 'rb') as file:
    ss_bin_data = file.read()
    file.close()

# Format accoringly, then write
ss_linker_format = ss_linker_format.replace('{0}', yaml_data['second_stage_entry_point'])
ss_end_addr = str(hex(int((len(ss_bin_data) / 512) + 1) * 512))
ss_end_addr = str(hex(0x7E00+int(ss_end_addr, base=16))).replace('0x', '')
ss_linker_format = ss_linker_format.replace('{1}', ss_end_addr)

with open('linker/linker.ld', 'w') as file:
    file.write(ss_linker_format)
    file.close()

# Getting kernel linker format
kernel_linker_format = ''
with open('config/formats/kern_linker_format', 'r') as file:
    kernel_linker_format = file.read()
    file.close()

# Get kernel binary information
kernel_bin_data = None
with open('../'+yaml_data['kernel_bin_filename'], 'rb') as file:
    kernel_bin_data = file.read()
    file.close()

# Format accordingly, then write
kernel_linker_format = kernel_linker_format.replace('{0}', yaml_data['kernel_entry_point'])
kernel_end_addr = str(hex(int((len(kernel_bin_data) / 512) + 1) * 512))
kernel_end_addr = str(hex((yaml_data['kernel_addr']*16)+int(kernel_end_addr, base=16))).replace('0x', '')
kernel_linker_format = kernel_linker_format.replace('{1}', kernel_end_addr)

with open('linker/kernel.ld', 'w') as file:
    file.write(kernel_linker_format)
    file.close()