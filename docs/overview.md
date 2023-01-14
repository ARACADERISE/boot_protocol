## What is FAMP?

<p>FAMP(Functional And Macro based Protocol) is, well, a functional/macro based approach to a boot protocol.</br>I had gained inspiration from the new modern boot protocol Limine. However, limine is based on a user-request/protocol-response type of system, and, to save myself some blood, sweat and tears, I decided to take a more formal approach, that being a functional/macro based approach.</br>With this, programmers can define a macro and get access to specific functionality. No need to worry about knowing what the name of a certain magic number is etc. Just define the macro(read the docs!!), and then when it comes to needing that functionality, it will be at your finger tips.</br>I decided to take this approach so that way the protocol could be "defined" as the OS is being built. The protocol ships with <i>many</i> tools and utensils. However, the programmer making the OS <i>might</i> not need them all. So, that is why I decided to take this approach: to make sure the protocol ships with every last bit of functionality, but only certain functionality is implied depending on macro definitions.</p></br>

## How does FAMP work?
<p>FAMP does not ship with a working bootloader, however, it ships with <i>formats</i> of many different things that ultamitely come together to make the OS work.</br>FAMP gives the programmer the choice of writing their own second-stage bootloader C code or letting the protocol take over that duty. See <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/boot_yaml_file.md">boot_yaml_file.md</a></u></b> for further information.</br>If the programmer does not want to write their own second-stage bootloader C code, then the protocol will fill out the format that it has for its own second-stage bootloader and apply all the values depending on what you have in the file <b><u>boot.yaml</u></b>.</br>The protocol depends on the file <b><u>boot.yaml</u></b> to fill out the formats accordingly. <b><u>boot.yaml</u></b> allows the protocol to be flexible. With <b><u>boot.yaml</u></b>, the MBR comes <i>after</i> you have written second-stage bootloader code(if you want) and after you write some kernel code. The reason is that the protocol needs to know the <i>binary sizes</i> of the kernel/second-stage bootloader binary files. Depending on the size, we get the accurate amount of sectors per binary and read in the according sectors into memory at the physicall memory address given to us by the programmer via <b><u>boot.yaml</u></b>.</br></p>

## Contributions
<p>As of current, I am not open to any contributions. I am attempting to get a working boot protocol up and running <i>by myself</i>.</br>After I am capable of getting the protocol in a working state(a working beta version that can be used efficiently), I will be open to contributions for at that time I am sure there will be a few ideas people have come up with to add better, or make better, functionality.</p>

## Outline
<h4>If you're writing your own second-stage bootloader C code:</h4>
<ul>
<li>
  <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/init_bootloader.md">init_bootloader.md</a></u></b></br>
  <ul>
    <li>Learn all settings that can be passed via <b><i><u>init_bootloader</u></i></b></li>
    <li>Learn how the protocol initializes the second-stage bootloader</li>
    <li>You will learn the following settings:</li>
    <ul>
      <li>CLEAN_GDT_DEF_VID_MODE</li>
      <li>CLEAN_GDT_VESA_VID_MODE</li>
      <li>DEFAULT_ALL - Same as CLEAN_GDT_DEF_VIDEO_MODE</li>
      <li>BIT16_BIT32_GDT_DEF_VID_MODE</li>
      <li>BIT16_BIT32_GDT_VESA_VID_MODE</li>
      <li>BIT32_GDT_DEF_VID_MODE</li>
      <li>BIT32_GDT_VESA_VID_MODE</li>
    </ul>
    <li>Learn all macros that are used for a second-stage bootloader</li>
    <li>You will learn the following macros:</li>
    <ul>
      <li>bit16_bit32_GDT</li>
      <li>default_gdt</li>
    </ul>
    <li>Learn what each macro does, what affects it has and how it works</li>
    <li>Other things you will learn:</li>
    <ul>
      <li>Learn how the bootloader(MBR) is configured</li>
      <li>Learn how the protocol loads the second-stage bootloader</li>
      <li>Learn why the protocol loads the second-stage bootloader code at address <b><i><u>0x7E00</u></i></b></li>
    </ul>
  </ul>
</li>
<li>
  <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/gdt.md">gdt.md</a></u></b></br>
  <ul>
    <li>Learn what a GDT is</li>
    <li>Learn how the protocol loads a pre-existing or a empty GDT into memory via the function <b><i><u>init_bootloader</u></i></b></li>
    <li>Learn how the protocol is capable of determining whether a GDT has 16-bit segments or not</li>
    <li>Learn how the protocol keeps a status on the GDT</li>
    <li>Learn why the protocol supports 32-bit/16-bit GDT</li>
  </ul>
</li>
<li>
  <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/loading_kernel.md">loading_kernel.md</a></u></b></br>
  <ul>
    <li>Learn how the protocol loads the kernel into memory</li>
    <li>Learn how the protocol <i>links</i> the kernel</li>
    <li>Learn what a higher-half kernel is - FAMP supports higher-half kernels only</li>
  </ul>
</li>
<li>
  <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/memory_map.md">memory_map.md</a></u></b></br>
  <ul>
    <li>Learn what functions are available to you to efficiently read the memory map and configure it</li>
    <li>Learn how to use the memory map</li>
    <li>Learn how the protocol handles memory</li>
    <li>* MIGHT DELETE</br> Learn how the protocol reserves 5 sectors for "critical" kernel/bootloader data</li>
    <li>Learn how the protocol formats the memory map, and learn how the protocol releases the "pool of memory" into the memory map</li>
    <li>Learn what memory the protocol marks as "read only", and what memory the protocol marks as "writeable/readable"</li>
  </ul>
</li>
  <li><b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/additional_second_stage_info.md">additional_second_stage_info.md</a></u></b></br>
  <ul>
    <li>Any additional information the programmer might need to know</li>
  </ul>
</li>
</ul>
