# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2006-2022  triductor


include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

# 自定义image前缀，例如：tgp3.0.0
IMG_PREFIX:=$(VERSION_DIST_SANITIZED)$(VERSION_NUMBER)

# 制作带DTB boot
ADD_BOOT_DBT=$(shell cp $(KDIR)/image-$(DEVICE_DTS).dtb $(BUILD_DIR)/tri_boot/src/;  mv $(BUILD_DIR)/tri_boot/src/image-$(DEVICE_DTS).dtb $(BUILD_DIR)/tri_boot/src/boot.dtb;  chmod +x $(BUILD_DIR)/tri_boot/src/tools/gen_boot.sh;  sh $(BUILD_DIR)/tri_boot/src/tools/gen_boot.sh $(BUILD_DIR)/tri_boot/src;)

COPY_BOOT=$(shell if [ -f $(BUILD_DIR)/tri_boot/bin/TR6560-bootimage.bin ];then cp $(BUILD_DIR)/tri_boot/bin/TR6560-bootimage.bin $(BIN_DIR)/TR6560-bootimage.bin;fi;)
define Build/append-boot
	$(ADD_BOOT_DBT)  
	$(COPY_BOOT)  
	dd if=$(BIN_DIR)/TR6560-bootimage.bin >> $@
endef

define Build/BrnImage
	cp $(BIN_DIR)/TR6560-bootimage.bin $(STAGING_DIR_HOST)/bin
	chmod +x $(STAGING_DIR_HOST)/bin/gen_ecc.sh
	sh $(STAGING_DIR_HOST)/bin/gen_ecc.sh $(STAGING_DIR_HOST)/bin $(KDIR)/tmp $(IMAGE_KERNEL) $(IMAGE_ROOTFS).tmp $(IMG_PREFIX) $(DEVICE_MODEL) $(FILESYSTEMS) $@ $(ROOTFS_DATA_TRI_SIZE) $(FLASH_SIZE)
endef

define Build/BrnImageNor
	cp $(BIN_DIR)/TR6560-bootimage.bin $(STAGING_DIR_HOST)/bin
	chmod +x $(STAGING_DIR_HOST)/bin/gen.sh
	sh $(STAGING_DIR_HOST)/bin/gen.sh $(STAGING_DIR_HOST)/bin $(KDIR)/tmp $(IMAGE_KERNEL) $(IMAGE_ROOTFS) $(IMG_PREFIX) $(DEVICE_MODEL) $(FILESYSTEMS) $@ $(ROOTFS_DATA_TRI_SIZE) $(FLASH_SIZE)
endef

define Build/append-ubi-rootfs-data
	$(TOPDIR)/scripts/triductor/ubinize-image-rootfs-data.sh \
		$(foreach part,$(UBINIZE_PARTS_DATA),--part $(part)) \
		$@.tmp \
		-p $(BLOCKSIZE:%k=%KiB) -m $(PAGESIZE) \
		$(if $(SUBPAGESIZE),-s $(SUBPAGESIZE)) \
		$(if $(VID_HDR_OFFSET),-O $(VID_HDR_OFFSET)) \
		$(UBINIZE_OPTS)
	cat $@.tmp >> $@
	rm $@.tmp
endef

define Build/append-jffs2-rootfs-data
	dd if=/dev/zero of=rootfs_data.bin bs=$(ROOTFS_DATA_TRI_SIZE) count=1
	$(STAGING_DIR_HOST)/bin/mkfs.jffs2 -p $(patsubst %k,%KiB,$(ROOTFS_DATA_TRI_SIZE)) \
		$(if $(CONFIG_BIG_ENDIAN),-b,-l) \
		-v -e $(patsubst %k,%KiB,$(BLOCKSIZE)) \
		-o $@.new \
		-d $(1) \
		2>&1 1>/dev/null
	dd if=$@.new of=rootfs_data.bin conv=notrunc
	cat rootfs_data.bin >> $@
	-rm -f rootfs_data.bin
endef

### Devices ###
define Device/Default
  PROFILES = Default
  DEVICE_VENDOR := triductor
  DEFAULT_SOC := tr6560
  KERNEL_LOADADDR := 0x80608000
  COMPILE :=
  KERNEL_NAME := zImage
  KERNEL_DEPENDS = $(wildcard $(DTS_DIR)/$(DEVICE_DTS).dts)
  KERNEL := kernel-bin | append-dtb | uImage none
  FILESYSTEMS := squashfs
endef

define Device/THG6500-TAX2
  DEVICE_MODEL := THG6500-TAX2
  DEVICE_DTS := THG6500-TAX2
  DEVICE_PACKAGES := tr5220-eFEM-tr6560 lftp tcpdump nand-utils
  $(Device/$(SUBTARGET))
  # IMAGES += factory.bin
  # IMAGE/factory.bin := append-kernel | pad-to $$(KERNEL_SIZE) | append-rootfs | pad-to $$(KERNEL_ROOTFS_SIZE) | append-ubi-rootfs-data
endef

define Device/THG6400-TAC2
  DEVICE_MODEL := THG6400-TAC2  
  DEVICE_DTS := THG6400-TAC2
  DEVICE_PACKAGES := tr5120-basic lftp tcpdump nand-utils
  $(Device/$(SUBTARGET))
  # IMAGES += factory.bin
  # IMAGE/factory.bin := append-kernel | pad-to $$(KERNEL_SIZE) | append-rootfs | pad-to $$(KERNEL_ROOTFS_SIZE) | append-jffs2-rootfs-data $(PLATFORM_DIR)/$(SUBTARGET)/manufacture_OpenWrt
endef

define Device/THG6500P-TAX2
  DEVICE_MODEL := THG6500P-TAX2
  DEVICE_DTS := THG6500P-TAX2
  DEVICE_PACKAGES := tr5220-iFEM-tr6560p lftp tcpdump nand-utils
  $(Device/$(SUBTARGET))
endef

define Device/generic
  BOOT_SIZE := 384k
  KERNEL_SIZE := 8192k
  KERNEL_BOOT_SIZE := 8576k
  KERNEL_ROOTFS_BOOT_SIZE := 41344k
  KERNEL_ROOTFS_SIZE := 40960k
  UBINIZE_OPTS := -s 2048 -e 1 -E 5
  BLOCKSIZE := 128k
  PAGESIZE := 2048
  VID_HDR_OFFSET := 2048
  ROOTFS_DATA_TRI_SIZE := 2048K
  FLASH_SIZE := 131072
  UBINIZE_PARTS_DATA := rootfs_data=$(PLATFORM_DIR)/$(SUBTARGET)/manufacture_OpenWrt
  MKUBIFS_OPTS := -m 2048 -e 124KiB -c 4096
  IMAGES=$(if $(BURNING),sysupgrade.bin fullimage.bin BurningImage.bin,sysupgrade.bin fullimage.bin)
  IMAGE/sysupgrade.bin := append-kernel | pad-to $$$$(KERNEL_SIZE) | append-ubi | append-string "fullimage=0" | append-crc32 | append-metadata 
  IMAGE/fullimage.bin := append-boot | pad-to $$$$(BOOT_SIZE) | append-kernel | pad-to $$$$(KERNEL_BOOT_SIZE) | append-ubi | append-string "fullimage=1" | append-crc32 | append-metadata
  IMAGE/BurningImage.bin := append-ubi-rootfs-data | BrnImage
endef

define Device/norflash
  BOOT_SIZE := 384k
  KERNEL_SIZE := 2560k
  KERNEL_BOOT_SIZE := 2944k
  KERNEL_ROOTFS_BOOT_SIZE := 14080k
  KERNEL_ROOTFS_SIZE := 13696k
  ROOTFS_SIZE := 11136k
  BLOCKSIZE := 64k
  FLASH_SIZE := 16384
  ROOTFS_DATA_TRI_SIZE := 1984K
  IMAGES=$(if $(BURNING),sysupgrade.bin fullimage.bin BurningImage.bin,sysupgrade.bin fullimage.bin)
  IMAGE/sysupgrade.bin := append-kernel | pad-to $$$$(KERNEL_SIZE) | append-rootfs | append-string "fullimage=0" | append-crc32 | append-metadata 
  IMAGE/fullimage.bin := append-boot | pad-to $$$$(BOOT_SIZE) | append-kernel | pad-to $$$$(KERNEL_BOOT_SIZE) | append-rootfs | append-string "fullimage=1" | append-crc32 | append-metadata
  IMAGE/BurningImage.bin := append-jffs2-rootfs-data $(PLATFORM_DIR)/$(SUBTARGET)/manufacture_OpenWrt | BrnImageNor
endef

define Device/THG6500R-TAX2
  DEVICE_MODEL := THG6500R-TAX2
  DEVICE_DTS := THG6500R-TAX2
  DEVICE_PACKAGES := tr5220-eFEM-tr6560
  $(Device/$(SUBTARGET))
endef


define Device/THG6500PR-TAX2
  DEVICE_MODEL := THG6500PR-TAX2
  DEVICE_DTS := THG6500PR-TAX2
  DEVICE_PACKAGES := tr5220-iFEM-tr6560p
  $(Device/$(SUBTARGET))
endef


define Device/THG6400R-TAC2
  DEVICE_MODEL := THG6400R-TAC2  
  DEVICE_DTS := THG6400R-TAC2
  DEVICE_PACKAGES := tr5120-basic
  $(Device/$(SUBTARGET))
endef

define Device/THG6400PR-TAC2
  DEVICE_MODEL := THG6400PR-TAC2  
  DEVICE_DTS := THG6400PR-TAC2
  DEVICE_PACKAGES := tr5120-basic
  $(Device/$(SUBTARGET))
endef

ifeq ($(SUBTARGET),generic)
  TARGET_DEVICES += THG6500-TAX2 THG6400-TAC2 THG6500P-TAX2
endif

ifeq ($(SUBTARGET),norflash)
  TARGET_DEVICES += THG6500R-TAX2 THG6400R-TAC2 THG6400PR-TAC2 THG6500PR-TAX2
endif

$(eval $(call BuildImage))
