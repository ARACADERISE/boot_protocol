import yaml
import os
import sys
import json
import binascii

if os.path.isfile('../boot.yaml'):
    PAD = bytes('\0', encoding='utf-8')
    format_needed = [
        # "Needed" section
        'n',
        # Pad
        PAD,
        # Length of files for OS to work
        # 2 for 32-bit: second-stage and kernel
        # 3 for 64-bit: second-stage, protected-mode stub, 64-bit kernel
        0,
        PAD,
        # 'f'
        'f',
        # Pad2
        PAD,
        # 's' before second stage file/binary file
        # 'k' before kernel file/binary file
        # Filename size pad Filename pad binary file pad address pad file size
    ]
    format_extras = [
        # "Extra" section
        'e',
        # Pad
        PAD,
        # Extra section info
        'i',
        # Pad2
        PAD,
        # Total pad filename size pad Filename pad binary file pad address pad file size
    ]

    REQUIRED = [
        'kernel_binary',
        'kernel_addr',
        'kernel_source_code_file',
        'second_stage_binary',
        'second_stage_addr',
        'second_stage_source_code_file',
        'os_type'
    ]

    file = open('../boot.yaml', 'r')
    data = yaml.safe_load(file)
    file.close()

    for i in REQUIRED:
        if not i in data:
            print(f'Error: Expected `{i}` in `boot.yaml`.')
            sys.exit(1)
    
    # Check the type of OS(32bit/64bit)
    if data['os_type'] == '32bit': format_needed[2] = bytes(str(2), encoding='utf-8')
    elif data['os_type'] == '64bit': format_needed[2] = bytes(str(3), encoding='utf-8')
    else: format_needed[2] = bytes(str(2), encoding='utf-8')

    # Extra binaries/addresses/source code files
    extra_bins = None
    extra_source_files = None
    extra_addresses = None

    if 'extras' in data:
        extra = data['extras']

        extra_source_files = extra['files']
        extra_bins = extra['binaries']
        extra_addresses = extra['addresses']

        format_extras.append(len(extra_source_files))
        format_extras.append(PAD)

        for i in range(len(extra_source_files)):
            format_extras.append(bytes(str(len(extra_source_files[i])), encoding='utf-8'))
            format_extras.append(bytes('\0',encoding='utf-8'))
            format_extras.append(bytes(extra_source_files[i], encoding='utf-8'))
            format_extras.append(bytes('\0',encoding='utf-8'))
            format_extras.append(bytes(str(len(extra_bins[i])), encoding='utf-8'))
            format_extras.append(bytes('\0',encoding='utf-8'))
            format_extras.append(bytes(extra_bins[i], encoding='utf-8'))
            format_extras.append(bytes('\0',encoding='utf-8'))
            format_extras.append(bytes(str(extra_addresses[i]), encoding='utf-8'))
            format_extras.append(bytes('\0',encoding='utf-8'))

            if i == len(extra_source_files)-1: break

    # Second stage ideals
    second_stage_source_code_file = data['second_stage_source_code_file']
    second_stage_source_code_file_o = second_stage_source_code_file.split('/')
    for i in range(len(second_stage_source_code_file_o)):
        if not '.' in second_stage_source_code_file_o[i]: del second_stage_source_code_file_o[i]
        if i == len(second_stage_source_code_file_o)-1: break
    
    second_stage_source_code_file_o = second_stage_source_code_file_o[0]
    second_stage_source_code_file_o = second_stage_source_code_file_o[:len(second_stage_source_code_file_o)-2]
    second_stage_source_code_file_o = f'{second_stage_source_code_file_o}.o'
    
    # Making the hex value of ss_addr look good
    ss_addr = str(hex(data['second_stage_addr']))
    ss_addr = ss_addr[2:]
    ss_addr = f'0x0{ss_addr}'
    r_ss_addr = ss_addr[3:]
    r_ss_addr = f'0x{r_ss_addr}0'

    format_needed.append('s')
    format_needed.append(PAD)
    format_needed.append(0x07E0)
    format_needed.append(PAD)
    format_needed.append(bytes(str(int(r_ss_addr,base=16)), encoding='utf-8'))
    format_needed.append(PAD)
    format_needed.append(bytes(str(len(second_stage_source_code_file)), encoding='utf-8'))
    format_needed.append(PAD)
    format_needed.append(bytes(second_stage_source_code_file, encoding='utf-8'))
    format_needed.append(PAD)
    format_needed.append(bytes(str(len(second_stage_source_code_file_o)), encoding='utf-8'))
    format_needed.append(PAD)
    format_needed.append(bytes(second_stage_source_code_file_o, encoding='utf-8'))
    format_needed.append(PAD)

    file = open('config/something.bin', 'wb')
    for i in format_needed:
        try:
            file.write(i)
            print(i)
        except:
            file.write(bytes(str(i), encoding='utf-8'))
    for i in format_extras:
        try:
            file.write(i)
        except:
            try:
                file.write(bytes(int(i), encoding='utf-8'))
            except: file.write(bytes(str(i), encoding='utf-8'))
    file.close()
    sys.exit(1)