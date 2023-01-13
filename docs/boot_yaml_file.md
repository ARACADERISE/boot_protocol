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

  <li>Next, if you <i>do not</i> have your own second-stage bootloader binary code, do the following:</li>
  
  ```yaml
    has_second_stage: "no"
  ```
  
  <p>If you <i>do have</i> a binary file for your second-stage bootloader C code, do the following:</p>
  
  ```yaml
    has_second_stage: "yes"
  ```
  
  <p>If you have a second-stage bootloader written in C, then the following information is needed:</p>
  <li>First, make sure you have a directory in the parent directory of your OS code where the binary files will be stored</li>
  <li>Once you have that, the first thing the protocol needs from you is the binary object(.o) file where the second-stage binary will be written to:</li>
  <p>P.S: The binary files that you give to <i>both</i> the second-stage bootloader fields and the kernel fields <i>do not</i> need to exist. The protocol <i>creates</i> the binaries <i>for you</i></p>
</ol>
