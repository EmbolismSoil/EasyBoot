OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")   
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
. = 0x30000000;                  
. = ALIGN(4);                                      
.text :                                               
{
  cpu/s3c2440/start.o (.text)           
  *(.text)
}
. = ALIGN(4);
.rodata : { *(.rodata) }                        
. = ALIGN(4);
.data : { *(.data) }                              

. = ALIGN(4);
. = .;                                                        
__device_start = .;                          
.u_boot_cmd : { *(.__easy_boot_device) }             
__device_end = .;                     
__img_end = .;
 
. = _start + 0x100000;                       
. = ALIGN(32);                                 
__bss_start = .;                                  
.bss1 (NOLOAD) : {*(.bss)}
_bss_end = .;
. = ALIGN(4);
__stack = .;
}
 
