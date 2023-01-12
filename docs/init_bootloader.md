## Second stage bootloader, in C
<p>FAMP enables the user to write 16-bit C code for the second stage bootloader. The second stage bootloader is, however, optional. See <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/boot_yaml_file.md">boot_yaml_file.md</a></u></b> for more information.</br></p>

## First, what about the MBR?
<p>If you don't know, FAMP does not ship with a bootloader(MBR), it ships with <i>formats</i> of a MBR, second stage bootloader and other ideals that come together to make the OS work. The MBR requires the <i>binary</i> files of the kernel and second-stage bootloader C code(if you wrote a custom second-stage bootloader in C). If you want more information over how the MBR is ultamitely configured, see <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/boot_yaml_file.md">boot_yaml_file.md</a></u></b> where it explains what goes into the file <b><u><i>boot.yaml</i></u></b> and how the file <b><u><i>boot.yaml</i></u></b> helps the protocol fill out the MBR format.</br></p>

## How to setup the second stage bootloader
<p>As of current, the protocol has both 32-bit only GDT support and 32-bit/16-bit GDT support. This is going to change into the protocol only having support for a 32-bit/16-bit GDT.</p></br></br>
<ol>
<li>The first thing you're going to want to do is let the protocol know what type of GDT you are aiming to use. If you want a 32-bit/16-bit GDT, you have to define the following macro: </li>

```bit16_bit32_GDT```

<p>The above macro tells the protocol to create a bit32/bit16 struct(a struct that holds section information for both the 32-bit selector and 16-bit selector). If the above macro <i>is not defined</i>, the protocol will create a 32-bit only GDT. For more information over the GDT in FAMP, see <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/gdt.md">gdt.md</a></u></b>.</br></p>
<li>Next, if you don't want to fill out the GDT struct fields manually, there is a macro that will make your life easier:</li>

```default_gdt```

<p>If you want the protocol to fill out the GDT struct fields <i>for you</i>, you must define the above macro. If the above macro <i>is not</i> defined, you will have to manually fill out the GDT.</br>Note: If you initialize the second stage bootloader with a setting other than the ones that load a clean GDT/GDT description into memory(<b><i><u>CLEAN_GDT_DEF_VID_MODE</u></i></b> , <b><i><u>CLEAN_GDT_VESA_VID_MODE</u></i></b> , <b><i><u>DEFAULT_ALL</u></i></b>), the macro definition <b><i><u>default_gdt</u></i></b> is useless.</p>

<li>Finally, after you have defined the according macros depending on your requirements, you will include the protocols boot header file.</br>For a second-stage bootloader with a clean 32-bit/16-bit GDT/GDT description loaded into memory, 32-bit/16-bit GDT support, and for a second-stage bootloader that makes the protocol fill out the GDT, the code should look something like: </li>

```c
#define bit32_bit16_GDT
#define default_all
#include "boot_protocol/boot_header.h"
```
</br></br>
<p>For a second-stage bootloader with a already-working 32-bit/16-bit GDT/GDT description loaded into memory and 32-bit/16-bit GDT support, the code should look something like:</p>

```c
#define bit32_bit16_GDT
#include "boot_protocol/boot_header.h"
```
</br></br>
<p>And, for a second-stage bootloader with a already-working 32-bit only GDT/GDT description loaded into memory and 32-bit only GDT support, the code will be:</p>

```c
#include "boot_protocol/boot_header.h"
```

</ol>
