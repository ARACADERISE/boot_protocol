## GDT
<p>The magical structure that enables developers to enter, and write, 32-bit.. FAMP makes dealing with the GDT super easy. In fact, FAMP makes sure dealing with the GDT is <i>so easy</i> that all you have to do is define <b><u>bit16_bit32_GDT</u></b> if you want a 16-bit/32-bit, and then define a macro of which tells the protocol to apply the correct values to the GDT struct <i>for you</i>. Then, when you're ready for kernel, all you have to stress about is making sure you invoke <b><u>load_kernel</u></b>.</p>

## How does FAMP work with the GDT?
<p>FAMP defeaults to a 32-bit <b>ONLY</b> GDT. This is a feature that is going to be removed. The protocol will eventually support only the 16-bit/32-bit GDT. If a developer wants to use a 16-bit/32-bit GDT using FAMP, they have to define the following macro(as should have been explained in <b><u><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/init_bootloader.md">init_bootloader.md</a></u></b>):</p>

```c
#define bit16_bit32_GDT
```

<p>Furthermore, if a developer initializes the bootloader with a <i>clean GDT/GDT description</i> loaded into memory, underneath the above macro definition, the user can tell the protocol to fill out the GDT/GDT descriptions structs <i>for them</i>:</p>

```c
#define bit16_bit32_GDT
#define default_gdt
```

<p>Note: All of the above information pertains to developers writing there own second-stage bootloader C code. Alongside this, the above macros(<b><u>bit16_bit32_GDT</u></b> , <b><u>default_gdt</u></b>) are <i>only ever useful</i> if you load a <i>clean GDT/GDT description</i>(<b><u>DEFAULT_ALL</u></b> , <b><u>CLEAN_GDT_DEF_VID_MODE</u></b> , <b><u>CLEAN_GDT_VESA_VID_MODE</u></b>) into memory. If you pass <i>any other</i> setting besides the ones mentioned via the function <b><i>init_bootloader</i></b>, you do not have to worry about defining the macros <b><u>bit16_bit32_GDT</u></b> and <b><u>default_gdt</u></b></br></br>Now, <i>how exactly</i> do those macros end up working?</p>

<h3>bit16_bit32_GDT macro</h3>
<p>So, if you followed the documentation in <b><a href="https://github.com/MocaCDev/boot_protocol/blob/main/docs/init_bootloader.md">init_bootloader.md</a></b>, you will see that the macro definitions come <i>before</i> the including of the boot protocols boot header file. The header file that we include(<b><u>"boot_protocol/boot_header.h"</u></b>) is the "meeting point" for all the other header files/assembly code. That file is the "home" to <i>every last</i> possible function the protocol hosts for the bootloader; and, well, this file is <i>the home</i> to the GDT. For more information over <b><u>"boot_protocol/boot_header.h"</u></b>, see <b><a>protocol_boot_header</a></b>.</br></br>After the macros are defined, the user includes the protocols boot header. This file is home to GDT functionality. The protocols GDT api checks if the macro <b><u>bit16_bit32_GDT</u></b> is defined. If it is defined, it then defines a macro <b><u>has_rmode_access</u></b>. This is a macro that will eventually carry over to the kernel.</br>If the macro <b><u>bit16_bit32_GDT</u></b> is defined, the protocols GDT api will create a bit32/bit16 GDT struct. If it <i>is not</i> defined, the protocols GDT api will create a bit32 <b>ONLY</b> GDT.</br> If the macro <b><u>bit16_bit32_GDT</u></b> is defined, the protocols GDT api will do:</p>

```c
#ifdef bit32_bit16_GDT

/* If the GDT has 16-bit segments, we want to make sure we know this. */
#define has_rmode_access
...
...
#endif
```

<p>If the macro <b><u>bit16_bit32_GDT</u></b> is defined, the following GDT struct will be created:</p>

```c
typedef struct GDT
    {
        /* 32-bit: null descriptor. */
        unsigned long long  null_desc;

        /* 32-bit: code segment. */
        uint16              code32_limit;          // 0xFFFFF for full 32-bit range
        uint16              code32_base;
        uint8               code32_base2;
        uint8               code32_access;         // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               code32_gran;           // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               code32_base_high;

        /* 32-bit: data segment. */
        uint16              data32_limit;         // 0xFFFFF for full 32-bit range
        uint16              data32_base;
        uint8               data32_base2;
        uint8               data32_access;        // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               data32_gran;          // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               data32_base_high;

        /* 16-bit: code segment. */
        uint16              code16_limit;         // should be 0xFFFFF(same as 32-bit code/data segments)
        uint16              code16_base;
        uint8               code16_base2;
        uint8               code16_access;       // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               code16_gran;         // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               code16_base_high;

        /* 16-bit: data segment. */
        uint16              data16_limit;         // should be 0xFFFFF(same as 32-bit code/data segments)
        uint16              data16_base;
        uint8               data16_base2;
        uint8               data16_access;       // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               data16_gran;         // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               data16_base_high;
    } _GDT;
```

<p>If the macro <b><u>bit16_bit32_GDT</u></b> is not defined, the following GDT struct will be created:</p>

```c
typedef struct GDT
    {
        /* 32-bit: null descriptor. */
        unsigned long long  null_desc;

        /* 32-bit: code segment. */
        uint16              code32_limit;          // 0xFFFFF for full 32-bit range
        uint16              code32_base;
        uint8               code32_base2;
        uint8               code32_access;         // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               code32_gran;           // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               code32_base_high;

        /* 32-bit: data segment. */
        uint16              data32_limit;         // 0xFFFFF for full 32-bit range
        uint16              data32_base;
        uint8               data32_base2;
        uint8               data32_access;        // access (present, ring 0, code segment, executable, direction 0, readable)
        uint8               data32_gran;          // granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
        uint8               data32_base_high;
    } _GDT;
```

<h3>How does FAMP keep track of the GDT?</h3>
<p>FAMP assigns a specific address for the GDT status. This address is referenced throughout the bootloader, and enables the protocol to become flexible with how it loads 32-bit. The GDT status is as follows:</p>

<ul>
    <li>0 - There needs to be a working GDT/GDT description loaded into memory</li>
    <li>1 - There is an already-working 16-bit/32-bit GDT/GDT description loaded into memory</li>
    <li>2 - There is an already-working 32-bit GDT/GDT description loaded into memory</li>
</ul>

<h3>Loading of the GDT</h3>
<p>FAMPs GDT api offers multiple different functions for configuring the GDT and ultimately loading the GDT.</br></br>For the user, all you have to do is invoke the function <b><u>load_kernel</u></b>.</br> The function <b><u>load_kernel</u></b> will do the following:</p>
<ol>
    <li>Setup GDT/GDT description. This is done via:</li>
    
```c
static inline void setup_gdt_and_gdt_desc();
```
    
<ul>
    <li>In the function <b><u>setup_gdt_and_gdt_desc</u></b>, the protocol has a few extra things it has to do. First, the protocol checks and sees if the user defined <b><u>default_gdt</u></b>. If the user <i>has not</i> defined the macro, the function <b><u>setup_gdt_and_gdt_desc</u></b> will simply do <i>nothing</i></li>
    <li>Second, the protocol is going to check the GDT status and see if it is zero. If the status <i>is</i> zero, the protocol will fill out the GDT accordingly. If the status is non-zero, the function <b><u>setup_gdt_and_gdt_desc</u></b> will do <i>nothing</i>.</li>
    <li>Third, the protocol checks if there are 16-bit sections. If there are, it assigns the according 16-bit section data. If it does not, it just exits</li>
    <li>Fourth, if the macro <b><u>default_gdt</u></b> <i>is not</i> defined, and the GDT status is zero, the function will error. If you plan on manually filling out the GDT/GDT description, <b><i>DO NOT</i></b> forget to run <b><u>set_gdt_status</u></b>. Scroll down to see more information over this.</li>
</ul>

<li>After <b><u>load_kernel</u></b> invokes <b><u>setup_gdt_and_gdt_desc</u></b>, it invokes <b><u>load_32bit</u></b>. The first thing <b><u>load_32bit</u></b> does is check and sees if either the GDT/GDT description is <b>NULL</b>. If it is, it will error. Then, the function runs the function <b><u>save_gdt_and_load</u></b>.</br></br>The <i>assembly function(stub)</i> <b><u>save_gdt_and_load</u></b> can be defined like so in C:</li>

```c
static void save_gdt_and_load(struct gdt_description, struct gdt);
```

The <i>assembly function(stub)</i> <b><u>save_gdt_and_load</u></b> does the following:</li>

<ul>
    <li>Checks the GDT status. It will perform actions accordingly:</li>
    
```x86asm
save_gdt_and_load
    ; Do we need to load a already-working gdt into memory?
    mov eax, [g_GDT_status]
    cmp eax, 0
    je .load_new_gdt
    
    ; Is there already a bit32/bit16 GDT in memory that the user put there?
    cmp eax, 1
    je .load_it
    
    ; Is there already a bit32 GDT in memory?
    cmp eax, 2
    je .load_it
    
    ; If for some reason the gdt status is neither 1 or 0, error
    jmp .gdt_error
```

<ul>
    <li>GDT status is zero - The assembly stub will jump to a section of code(<b><i>load_new_gdt</i></b>) that <i>loads</i> the GDT/GDT description that was passed as arguments</li>
    <li>GDT status is one - The assembly stub will jump to a section of code(<b><i>load_it</i></b>) that <i>loads</i> the GDT description. It will ignore the arguments passed to it</li>
    <li>GDT status two - Same as GDT status one</li>
    <li>If the GDT status isn't zero, one or two the assembly stub jumps to a section of code that prints an error, then halts(<b><i>gdt_error</i></b>).</li>
</ul>
</ul>
</ol>

<h3>Why would someone want a 16-bit/32-bit GDT instead of just a 32-bit GDT?</h3>
<p>The main advantage of this is <i>being able to go back to real-mode</i>. FAMP supports just that. It is flexible in that it is able to run a OS with a 16-bit/32-bit GDT or a 32-bit <b>ONLY</b> GDT. When it comes to kernel code, the developer is given functionality such as <b><u>enter_rmode</u></b> , <b><u>enter_rmode_and_stay</u></b> , <b><u>leave_rmode</u></b> etc. These functions will <i>error</i> if the developer <i>does not</i> have a 16-bit/32-bit GDT.</p>

<h3>What is <b><i>set_gdt_status</i></b>?</h3>
<p><b><u>set_gdt_status</u></b> is a function that FAMP offers to <i>manually set</i> the GDT status. This is for if you're manually filling out the GDT/GDT description.</br>The function takes in a "<i>setting</i>". Here are the settings that can be passed via <b><u>set_gdt_status</u></b>:</p>

```c
enum gdt_status_settings
{
    NO_GDT          = 0x00,
    BIT16_BIT32_GDT = 0x01,
    BIT32_ONLY_GDT  = 0x02
}
```

<p>It <i>could</i> be considered "good practice" to do the following after initializing the boot with a clean GDT/GDT description loaded into memory:</p>

```c
void starting_point bootloader_main()
{
    init_bootloader(CLEAN_GDT_DEF_VID_MODE);
    set_gdt_status(NO_GDT);
}
```

<p>Reason being, doing the above <i>makes sure</i> the GDT status signifies that there <b>is no GDT</b> loaded into memory. It enforces "safety" amongst the feature of keeping a status on the GDT. Odds are, setting the GDT status to <b><u>NO_GDT</u></b> <i>after</i> running <b><u>init_bootloader</u></b> is useless, but it never hurts to be safe :)</p>
