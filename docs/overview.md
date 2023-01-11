## What is FAMP?

<p>FAMP(Functional And Macro based Protocol) is, well, a functional/macro based approach to a boot protocol.</br>I had gained inspiration from the new modern boot protocol Limine. However, limine is based on a user-request/protocol-response type of system, and, to save myself some blood, sweat and tears, I decided to take a more formal approach, that being a functional/macro based approach.</br>With this, programmers can define a macro and get access to specific functionality. No need to worry about knowing what the name of a certain magic number is etc. Just define the macro(read the docs!!), and then when it comes to needing that functionality, it will be at your finger tips.</br>I decided to take this approach so that way the protocol could be "defined" as the OS is being built. The protocol ships with <i>many</i> tools and utensils. However, the programmer making the OS <i>might</i> not need them all. So, that is why I decided to take this approach: to make sure the protocol ships with every last bit of functionality, but only certain functionality is implied depending on macro definitions.</p></br>

## How does FAMP work?
<p>FAMP does not ship with a working bootloader, however, it ships with <i>formats</i> of many different things that ultamitely come together to make the OS work.</br>FAMP gives the programmer the choice of writing there own second-stage bootloader C code or letting the protocol take over that duty. See <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/boot_yaml_file.md">boot_yaml_file.md</a></u></b> for further information.</br>If the programmer does not want to write there own second-stage bootloader C code, then the protocol will fill out the format that it has for its own second-stage bootloader and apply all the values depending on what you have in the file <b><u>boot.yaml</u></b>.</br>The protocol depends on the file <b><u>boot.yaml</u></b> to fill out the formats accordingly. <b><u>boot.yaml</u></b> allows the protocol to be flexible. With <b><u>boot.yaml</u></b>, the MBR comes <i>after</i> you have written second-stage bootloader code(if you want) and after you write some kernel code. The reason is that the protocol needs to know the <i>binary sizes</i> of the kernel/second-stage bootloader binary files. Depending on the size, we get the accurate amount of sectors per binary and read in the according sectors into memory at the physicall memory address given to us by the programmer via <b><u>boot.yaml</u></b>.</br></p>

## Contributions
<p>As of current, I am not open to any contributions. I am attempting to get a working boot protocol up and running <i>by myself</i>.</br>After I am capable of getting the protocol in a working state(a working beta version that can be used efficiently), I will be open to contributions for at that time I am sure there will be a few ideas people have come up with to add better, or make better, functionality.</p>

## Outline
<h4>If you're writing your own second-stage bootloader C code:</h4>
<ul>
<li><b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/init_bootloader.md">init_bootloader.md</a></u></b></br><ul><li>Hi</li></ul></li>
<li><b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/second_stage_macros.md">second_stage_macros.md</a></u></b></li>
<li><b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/gdt.md">gdt.md</a></u></b></li>
<li><b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/loading_kernel.md">loading_kernel.md</a></u></b></li>
<li><b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/memory_map.md">memory_map.md</a></u></b></li>
</ul>
