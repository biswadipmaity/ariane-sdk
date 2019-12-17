ARCH := riscv
CROSS_COMPILE := riscv64-unknown-linux-gnu-
KERNELDIR := /data/scratch/ge97toz/workspace/Biswadip/ariane-sdk/buildroot/output/build/linux-ariane-v0.7/
EXTRA_CFLAGS := -O0

obj-m := hello.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD) EXTRA_CFLAGS=$(EXTRA_CFLAGS) modules
	riscv64-unknown-elf-objdump -d hello.ko > disassemble.asm
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf disassemble.asm