+ ld obj/kern/kernel
+ mk obj/kern/kernel.img
Could not open option rom 'sgabios.bin': No such file or directory
6828 decimal is 15254 octal!
Physical memory: 66556K available, base = 640K, extended = 65532K
Total Pages: 16639
Debug info of pages....
&pages[0] = f0265000
&pages[npages-1] = f02857f0
kernel panic on CPU 0 at kern/pmap.c:791: assertion failed: page2pa(pp) != MPENTRY_PADDR
Welcome to the JOS kernel monitor!
Type 'help' for a list of commands.

QEMU: Terminated via GDBstub
