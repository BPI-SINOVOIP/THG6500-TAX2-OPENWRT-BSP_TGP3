#
# Copyright (C) 2013 Triductor.ltd,co
# Author : yfcao bsp team   Create
# Date   : 2024.12.05
#

include $(TOPDIR)/rules.mk

PKG_NAME:=offload_qos_manager
PKG_VERSION:=1.0
PKG_RELEASE:=1
NO_STRIP:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
export PKG_D_TYPE = RELEASE
export ROOTFS_DIR=$(STAGING_DIR)/root-$(BOARD)

include $(INCLUDE_DIR)/package.mk

define Package/offload_qos_manager
  SECTION:=utils
  CATEGORY:=Triductor
  SUBMENU:=
  TITLE:=Triductor offload qos support
  MAINTAINER:=Triductor bsp team
  DEPENDS:= +libpthread +libubus  +libblobmsg-json +libjson-c +libuci
endef

define Package/offload_qos_manager/description
 This package contains Triductor offload_qos_manager
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(PKG_BUILD_DIR)" \
	CROSS_COMPILE="$(TARGET_CROSS)"\
	CFLAGS="$(TARGET_CFLAGS)" 
endef

define Build/InstallDev
endef

define Package/offload_qos_manager/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/bin/
endef

$(eval $(call BuildPackage,offload_qos_manager))
