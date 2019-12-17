#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <asm/io.h>

MODULE_LICENSE("GPL");

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

int init_module(void)
{
    long unsigned int x=5;
    long unsigned int y=0;
    long unsigned int *add_x = &x;

    phys_addr_t phys_addr;

	printk(KERN_INFO "Loading fault injection module\n");

    printk(KERN_INFO "Virtual  address = %px\n", (void *) add_x);
    phys_addr = virt_to_phys(add_x);  
    printk(KERN_INFO "Physical address = %px\n", (void *) phys_addr);

    printk(KERN_INFO "Original number = %16lx \n", x);
    
    // Write address of approximate memory
    write_csr(0x801, add_x);
    // Enable approximation
    write_csr(0x803, 0xa);

    asm volatile ("nop");
    y=*add_x;

    // Disable approximation
    write_csr(0x803, 0);

    //Read paddr
    printk(KERN_INFO "paddr            = %px\n", (void *) read_csr(0x801));
    printk(KERN_INFO "Faulty   number = %16lx \n", y);
    printk(KERN_INFO "Original number = %16lx \n", x);
  

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Unloading fault injection module\n");
}