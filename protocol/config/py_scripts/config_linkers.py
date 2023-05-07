import yaml
import subprocess
import os

# `boot.yaml` data
yaml_data = None

# Obtain the yaml data from `boot.yaml`
with open('../boot.yaml', 'r') as file:
    yaml_data = yaml.full_load(file)
    file.close()

# Make the directory
subprocess.run('mkdir linker', shell=True, cwd=os.getcwd())

# `linker.ld` SECTIONS info
bootloader_section_data = '''{
    /* Important memory addresses for FAMP. */
    g_gdt_address = 0x7E00;
    g_gdt_desc_address = 0x7E30;
    g_gdt_status_address = 0x7E38;

    /* Second-stage starts at 0x7F00. */
    . = 0x7F00;
    second_stage_start = .;
    .text : AT(0x7F00)
    {
        *(__start*);
        *(.text*);
    }

    .data :
    {
        *(.bss*);
        *(.data*);
        *(COMMON*);
    }
    
    second_stage_end = .;
}'''

# Write it to `linker.ld`
with open('linker/linker.ld', 'w') as file:
    file.write(f'''ENTRY({yaml_data["second_stage_entry_point"]})
SECTIONS
{bootloader_section_data}
''')
    file.close()

# `kernel.ld` SECTIONS info
kernel_section_data = '''{
    /* Important memory addresses for FAMP. */
    g_gdt_address = 0x7E00;
    g_gdt_desc_address = 0x7E30;
    g_gdt_status_address = 0x7E38;

    /* The kernel starts at address 0xA000. */
    . = 0xA000;
    kernel_start = .;

    .text : AT(0xA000)
    {
        *(__start*);
        *(.text*);
    }

    .data :
    {
        *(.bss*);
        *(.data*);
        *(.rodata*);
        *(COMMON*);
    }

    kernel_end = .;
}'''

# Write it to `kernel.ld`
with open('linker/kernel.ld', 'w') as file:
    file.write(f'''ENTRY({yaml_data["kernel_entry_point"]})
SECTIONS
{kernel_section_data}
''')
    file.close()
