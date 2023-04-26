# Py script to move all scripts for FAMP tools(written in both Python and C)
# This script moves all the scripts(located in `scripts`) to `/usr/bin/`
import subprocess, os, sys

all_tool_scripts = ['FAMP_fdi']
all_tool_binaries = ['format_disk_image.o']
all_fimg_binaries = [i for i in os.listdir('bin') if '.fimg' in i]
usr_bin = '/usr/bin/'
scripts_path = f'{os.getcwd()}/config/scripts/'
formats_path = f'{os.getcwd()}/config/formats'
script_formats = [f'{formats_path}/FAMP_fdi_format']
moved_something = False
file_contents = None

if not os.path.exists('/usr/lib/FAMP_tools_bin'):
    subprocess.run('sudo mkdir /usr/lib/FAMP_tools_bin', shell=True, cwd=os.getcwd())
    print('Created /usr/lib/FAMP_tools_bin')
if not os.path.exists('/usr/lib/FAMP_disk_images'):
    subprocess.run('sudo mkdir /usr/lib/FAMP_disk_images', shell=True, cwd=os.getcwd())
    print('Created /usr/lib/FAMP_disk_images')

if len(sys.argv) > 1 and sys.argv[1] == 'move_disk_image':
    for i in all_fimg_binaries:
        subprocess.run(f'sudo mv {os.getcwd()}/bin/{i} /usr/lib/FAMP_disk_images', shell=True, cwd=os.getcwd())
    
    print(f'Moved {len(all_fimg_binaries)} disk image{"s" if len(all_fimg_binaries) > 1 else ""} into /usr/lib/FAMP_disk_images')
    sys.exit(0)

if len(os.listdir('/usr/lib/FAMP_tools_bin')) == 0:
    print('Moving ', end = '')
    for i in all_tool_binaries:
        if not i == all_tool_binaries[len(all_tool_binaries)-1]:print(i + ', ', end = '')
        else:print(i, end = '')
    print(' into /usr/lib/FAMP_tools_bin')
    for i in range(len(all_tool_binaries)):
        subprocess.run(f'sudo mv {os.getcwd()}/tools_bin/{all_tool_binaries[i]} /usr/lib/FAMP_tools_bin', shell=True, cwd=os.getcwd())

# Make sure all scripts are located in `/usr/bin`
# If not, move the script there
for i in range(len(all_tool_scripts)):
    if not os.path.isfile(f'{usr_bin}{all_tool_scripts[i]}'):
        print(f'Moving {all_tool_scripts[i]} into {usr_bin}')
        if os.path.isfile(f'{scripts_path}{all_tool_scripts[i]}'):
            subprocess.run(f'sudo mv {scripts_path}{all_tool_scripts[i]} /usr/bin', shell=True, cwd=os.getcwd())
            moved_something = True
        else:
            file_contents = open(script_formats[i], 'r').read()
            with open(f'{scripts_path}{all_tool_scripts[i]}', 'w') as file:
                file.write(file_contents)
                file.close()
            subprocess.run(f'sudo mv {scripts_path}{all_tool_scripts[i]} /usr/bin', shell=True, cwd=os.getcwd())
            moved_something = True
                

# If there was a script moved into `/usr/bin`, then this Python script did something
# In that case, prompt to the user that setting up FAMP tools was indeed successful
if moved_something:
    # Make sure all the scripts are runnable
    for i in range(len(all_tool_scripts)):
        subprocess.run(f'sudo chmod +x {usr_bin}{all_tool_scripts[i]}', shell=True, cwd=os.getcwd())
    
    # Make sure all the binaries are runnable
    for i in range(len(all_tool_binaries)):
        subprocess.run(f'sudo chmod +x /usr/lib/FAMP_tools_bin/{all_tool_binaries[i]}', shell=True, cwd=os.getcwd())
    print('Setup for FAMP tools successful!')
