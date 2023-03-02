## Quick Disclaimer

<p>This protocol, currently, has to be installed <i>locally</i>. So, wherever your OS code is located, that is where the boot protocol, too, has to be located.</p>
<p>It is currently a work-in progress for the protocol to be located in the standard includes for GCC/Clang.</p>

# Setting up

<p>1. Before you clone the repo, go ahead and make a new directory where your OS code will reside.</p>

```
mkdir my_os
```

<p>Where `my_os` is whatever name you want to call your OS</p></br>

<p>2. Enter the directory you created, and clone the protocols repository:</p>

```
cd my_os
```

```
git clone https://github.com/ARACADERISE/boot_protocol.git
```

<p>Once this is done, go ahead and refer to the boot protocols documentation on <i>how to use it</i>, then return after you are done.</p></br>
<p>3. After you have written some code, enter the directory `boot_protocol` of which you just cloned before writing some code.</p></br>
<p>4. Edit the `boot.yaml` file to store the according binary files for the second-stage bootloader and kernel, along with your second-stage bootloader C source code file and the C kernel source code file. You can refer to the protocols documentation for more information over the file `boot.yaml`.</p>
<p>After this, run `make`. Then, go back to the directory where your OS code resides, create a Makefile with the following(and add whatever for your likings):</p>

```makefile
.PHONY: run

run:
  @cd boot_protocol && make run
```

</br><p>Congrats! You now have a running OS :D</p>
