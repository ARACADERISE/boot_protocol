import os
import subprocess

if not os.path.isfile('boot/boot.asm'):
    subprocess.run('cd config && make build', shell=True)