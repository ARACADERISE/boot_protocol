# Python script to remove all binary files from /usr/lib/FAMP_tools_bin
# Also removes "runner" scripts located in /usr/bin
import subprocess, os, sys

# Names of scripts/binaries
scripts = ['FAMP_fdi']
binaries = ['format_disk_image.o']
fimg_binaries = [i for i in os.listdir('bin') if '.fimg' in i]

# Name of directories
usr_lib = '/usr/lib/FAMP_tools_bin/'
usr_bin = '/usr/bin/'
disk_image_path = '/usr/lib/FAMP_disk_images'

# Remove `done`, if it exists, from `user_preferences`
if os.path.isfile('config/user_preferences/done'):
    subprocess.run('rm -rf config/user_preferences/done', shell=True, cwd=os.getcwd())

# Function gets called if this script does not recieve `--purge` as an argument
def delete_all_binaries():
    if os.path.exists(usr_lib):
        for i in range(len(binaries)):
            subprocess.run(f'sudo rm -rf {usr_lib}{binaries[i]}', shell=True, cwd=os.getcwd())
    if os.path.exists(disk_image_path):
        for i in range(len(fimg_binaries)):
            subprocess.run(f'sudo rm -rf {disk_image_path}/{fimg_binaries[i]}', shell=True, cwd=os.getcwd())

# Check and see if `--purge` was passed as an argument
if len(sys.argv) > 1:
    if sys.argv[1] == '--purge': 
        subprocess.run(f'sudo rm -rf {usr_lib}', shell=True, cwd=os.getcwd())
        subprocess.run(f'sudo rm -rf {disk_image_path}', shell=True, cwd=os.getcwd())
    else: delete_all_binaries()
else: delete_all_binaries()

# Delete all the scripts in /usr/bin
for i in range(len(scripts)):
    if os.path.isfile(f'{usr_bin}{scripts[i]}'):
        subprocess.run(f'sudo rm -rf {usr_bin}{scripts[i]}', shell=True, cwd=os.getcwd())
