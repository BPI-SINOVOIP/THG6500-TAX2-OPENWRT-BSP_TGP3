# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2006-2022  triductor

include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=tr6560
BOARDNAME:=triductor TR6560
SUBTARGETS:=generic norflash
FEATURES:=squashfs

KERNEL_PATCHVER:=5.10
KERNEL_TESTING_PATCHVER:=5.10

KERNELNAME:=bzImage

include $(INCLUDE_DIR)/target.mk

# 开源wifi相关package
# 开源luci相关package
# 开源工具package
# 开源内核package
# easycwmp相关package
# triductor自研package
DEFAULT_PACKAGES += curl hostapd-utils wireless-tools wpad-basic-wolfssl wpa-cli uledd getopt\
luci luci-compat luci-ssl luci-app-omcproxy\
conntrack ethtool ip-bridge memtester nvram xl2tpd udhcpsnoop\
kmod-gpio-button-hotplug kmod-leds-gpio \
dmc-215 easycwmp tr069_stun \
start-boot_6560 triboot kmod-kgpio_adapter tri_bsp cs_cli kmod-igmp_mdb_notify  luci-app-triqos trinft-qos firewall4br\
libamxb  mod-amxb-ubus prplmesh  meshinfo offload_qos_manager

$(eval $(call BuildTarget))

