#
# Makefile for triductor bootloader
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:= start-boot_6560
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk



    
define Package/start-boot_6560
  CATEGORY:=Triductor
  TITLE:= Triductor First level Bootloader
  MAINTAINER:= rlsha@triductor.com
  DEPENDS:=
endef




define Package/start-boot_6560/description
	This package contains triductor first level boot code
endef


ifeq ($(CONFIG_TARGET_PROFILE),"DEVICE_THG6500-TAX2") 
        SUFFIX:=AX
endif

ifeq ($(CONFIG_TARGET_PROFILE),"DEVICE_THG6500R-TAX2")
        SUFFIX:=AX
endif


ifeq ($(CONFIG_TARGET_PROFILE),"DEVICE_THG6500P-TAX2")
        SUFFIX:=AX
endif


ifeq ($(CONFIG_TARGET_PROFILE),"DEVICE_THG6500PR-TAX2")
        SUFFIX:=AX
endif


ifeq ($(CONFIG_TARGET_PROFILE),"DEVICE_THG6400-TAC2")
        SUFFIX:=AC
endif


ifeq ($(CONFIG_TARGET_PROFILE),"DEVICE_THG6400R-TAC2")
        SUFFIX:=AC
endif


ifeq ($(CONFIG_TARGET_PROFILE),"DEVICE_THG6400PR-TAC2")
        SUFFIX:=AX
endif

define Build/Prepare
	$(CP) ./* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef


define Build/Compile
	echo "----------Install boot image $(CONFIG_TARGET_PROFILE) $(SUFFIX)----------"
endef

define Package/start-boot_6560/install
	$(CP) $(PKG_BUILD_DIR)/TR6560-startbootimage_$(SUFFIX).bin $(PKG_BUILD_DIR)/TR6560-startbootimage.bin
	$(CP) $(PKG_BUILD_DIR)/TR6560-startbootimage.bin  $(STAGING_DIR_HOST)/bin/
endef

define Build/InstallDev
endef

$(eval $(call BuildPackage,start-boot_6560))
