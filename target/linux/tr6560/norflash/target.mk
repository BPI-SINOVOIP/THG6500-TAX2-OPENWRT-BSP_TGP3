# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2006-2022  triductor


ARCH:=arm
BOARDNAME:=norflash
CPU_TYPE:=cortex-a9
KERNELNAME:=zImage
# FEATURES+= small_flash
DEFAULT_PACKAGES += jffs2

define Target/Description
	Build firmware images for Triductor TR6560 ARM based boards of nor flash.
endef
