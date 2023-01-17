## What is boot.yaml?
<p><b><u>boot.yaml</u></b> stores information over a <i>possible</i> second-stage binary file and a <i>required</i> kernel binary file(second-stage bootloader in C is optional). It also stores information over the source-code file of the second-stage bootloader C code(if there is any) and the kernel C code, alongside the address of the second-stage bootloader C code(if there is one) and the kernel code.</br>This file simply just makes the protocols life easier. It allows the protocol to take all the data from the file itself, store it and then reference it to fill out the formats it has correctly.</br></p>

## Complete Guide
<ol>
  <li>In <b><u>boot.yaml</u></b>, the first thing we <i>have</i> to do is specify the <i>type of OS</i>("bit32", "bit64") that we are writing.</br>This is done via <b><u>os_type</u></b>.</br></br>Setting the OS type to 32-bit:</li>
  
  ```yaml
    os_type: "bit32"
  ```
  
  <p>Setting the OS type to 64-bit:</li>
  
  ```yaml
    os_type: "bit64"
  ```
  
  <li>Then, after that, you have to tell the protocol where the bin folder is. This is done via <b><u>bin_folder</u></b>:</li>
  
  ```yaml
  bin_folder: "bin"
  ```
  
  <li>Next, if you <i>did not</i> write your own second-stage bootloader code in C, do the following:</li>
  
  ```yaml
    has_second_stage: "no"
  ```
  
  <p>If you <i>did write</i> your own second-stage bootloader code in C, do the following:</p>
  
  ```yaml
    has_second_stage: "yes"
  ```
  
  <p>If you have a second-stage bootloader written in C, then the following information is needed:</p>
  <ul>
    <li>First, make sure you have a directory in the parent directory of your OS code where the binary files will be stored</li>
    <li>Once you have that, the first thing the protocol needs from you is the binary object(.o) file where the second-stage binary will be written to: <b>second_stage_bin_o_filename</b></li>
    <p></br><u><b>P.S: The binary files that you give to <i>both</i> the second-stage bootloader fields and the kernel fields <i>do not</i> need to exist. The protocol <i>creates</i> the binaries <i>for you</i></b></u></p></br>
    <li>The next thing the protocol needs to know is the binary file(.bin) where the <i>flat binary</i> of the second-stage bootloader will be written: <b>second_stage_bin_filename</b></li>
    <li>Then, the protocol needs to know the <i>source code file</i> of the second-stage bootloader: <b>second_stage_source_code_file</b></li>
  </ul></br>
  <li>Here is an example of what the <b><u>boot.yaml</u></b> would look like with a second-stage bootloader written in C:</li>
  
  ```yaml
    # Yes, we have a second stage bootloader
    has_second_stage: "yes"
    # Binary object(.o) file where the second-stage bootloaders binary will be written to
    second_stage_bin_o_filename: "bin/second_stage.o"
    # Binary file(.bin) where the seonc-stage bootloaders flat binary will be written to
    second_stage_bin_filename: "bin/second_stage.bin"
    # Source code for the second-stage bootloader
    second_stage_source_code_file: "second-stage.c"
  ```
  
  <li>After you get the second-stage portion of the file done, you're going to do the <i>exact same thing</i>, just replace <b>second_stage</b> with <b>kernel</b></br></br>The complete <b>boot.yaml</b> file <i>with</i> a second-stage bootloader will look like:</li>
  
  ```yaml
  os_type: "bit32" # or "bit64" for 64-bit OS
  
  bin_dir: "bin"
  
  # Second-stage information
  has_second_stage: "yes"
  second_stage_bin_o_filename: "bin/second_stage.o"
  second_stage_bin_filename: "bin/second_stage.bin"
  second_stage_source_code_file: "second_stage.c"
  
  # Kernel information
  kernel_bin_o_filename: "bin/kernel.o"
  kernel_bin_filename: "bin/kernel.bin"
  kernel_source_code_file: "kernel.c"
  ```
  </br>
  <p>The complete <b>boot.yaml</b> file <i>without</i> a second-stage bootloader will look like:</p>
  
  ```yaml
  os_type: "bit32" # or "bit64" for 64-bit OS
  
  bin_dir: "bin"
  
  # Second-stage information
  has_second_stage: "no"
  
  # If you continue to specify information about the second-stage bootloader
  # an error will occur
  
  # Kernel information
  kernel_bin_o_filename: "bin/kernel.o"
  kernel_bin_filename: "bin/kernel.bin"
  kernel_source_code_file: "kernel.c"
  ```
  
</ol>

## What more?
<p>To be more extensive, I have written my own YAML parser. I did this so I could imply <i>my own functionality</i>, and so that way I could easily read the <b>boot.yaml</b> file and assign the data accordingly.</br>The protocol takes the <i>source code files</i> passed via <b>second_stage_source_code_file</b> and <b>kernel_source_code_file</b> and generates the according binary object(.o) files passed via <b>second_stage_bin_o_filename</b> and <b>kernel_bin_o_filename</b>.</br>The protocol then takes the binary object(.o) files, and links them. The linker scripts are filled out automatically by the protocol. The protocol locates the second-stage bootlaoder C code(if there is any) at the address <code>0x7E00</code>, and maps the kernel into higher-half.</br>The protocol produces binary output(.out) files from the linker. It then <code>objdump</code>s the .out files and assigns the output to whatever .bin file was passed via <b>second_stage_bin_filename</b> and <b>kernel_bin_filename</b>.</br></br>If there is a second-stage bootloader written in C, the protocol <i>links</i> the binary object(.o) file for the second-stage bootloader with the protocols bootloader util binary object(.o) file.</br>Furthermore, the protocol <i>links</i> the binary object file for the kernel with the protocols kernel util binary object(.o) file.</p></br></br>

## Possible Features
<ol>
    <li>Libraries.</br>I am aiming on adding a way for developers to write a "library" for the protocol, aswell as aiming on adding a way for people <i>using</i> the protocol to tell the protocol <i>what libraries</i> they want to use</li>
    <li>I am attempting to add a way for developers <i>using</i> the protocol to be able to load into memory data/code they have written that they want to be able to access.</br></br>With this, the protocol will have to be in charge of:</br>1. Reading in the correct amount of sectors</br>2. Referencing the address the developer gives as to where the code/data will be located</br>3. Obtaining the data/labels from the address where the data/code is loaded to</li></br>
    <p>A note to developers who aim to write a library for the protocol: the code <i>has</i> to be written <i>inside</i> labels. There can not be any "globally" scoped assembly and/or C code. As of right now, this feature will only be capable of working with assembly.</p>
</ol>
