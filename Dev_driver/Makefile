obj-m := motor.o
KDIR := /home/fa/linux-3.4.y/
all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm
clean:
		rm -rf ./.motor* ./.tmp* *.ko *.o *.mod.o *.mod.c *.symvers *.bak *.order
