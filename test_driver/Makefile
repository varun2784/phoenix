ifneq (${KERNELRELEASE},)
	obj-m := ofd.o
else
	KERNEL_SOURCE := /lib/modules/3.5.0-23-generic/build
	PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules

clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif

