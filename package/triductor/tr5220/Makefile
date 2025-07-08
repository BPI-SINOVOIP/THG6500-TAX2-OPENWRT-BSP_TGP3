#
# Makefile for the Linux Wireless network device drivers.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=tr5220
PKG_VERSION:=
PKG_RELEASE:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)

include $(INCLUDE_DIR)/package.mk

define Package/tr5220/Default
  SECTION:=base
  CATEGORY:=Triductor
  SUBMENU:=WiFi
  TITLE:=tr5220
  URL:=
  DEPENDS:=+kmod-cfg80211 +@DRIVER_11AC_SUPPORT +@DRIVER_11N_SUPPORT +@DRIVER_11AX_SUPPORT
endef

define Package/tr5220-eFEM-tr6560
$(call Package/tr5220/Default)
  TITLE += (with tr6560 external FEM)
  VARIANT:=eFEM
endef

define Package/tr5220-iFEM-tr6560
$(call Package/tr5220/Default)
  TITLE += (with tr6560 internal FEM)
  VARIANT:=iFEM
endef


define Package/tr5220-iFEM-tr6560p
$(call Package/tr5220/Default)
  TITLE += (with tr6560p internal FEM)
  VARIANT:=iFEM
endef


define Package/tr5220-eFEM-tr6560p
$(call Package/tr5220/Default)
  TITLE += (with tr6560p external FEM)
  VARIANT:=eFEM
endef



define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -R ./files/* $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

autoload_mods_$(SUBTARGET) := peanut_plat \
                              peanut_wifi \
                              wifi_debug

define Package/tr5220/install
	$(INSTALL_DIR) $(1)/$(MODULES_SUBDIR)
	$(INSTALL_DIR) $(1)/etc/wifi
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/lib/netifd/wireless

	$(CP) $(PKG_BUILD_DIR)/ko/*.ko $(1)/$(MODULES_SUBDIR)
	$(call ModuleAutoLoad,tr5220,$(1),06-,,$(autoload_mods_$(SUBTARGET)))
	$(CP) $(PKG_BUILD_DIR)/ko/*.bin $(1)/etc/wifi/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/* $(1)/etc/wifi/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/* $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lib/netifd/wireless/mac80211.sh $(1)/lib/netifd/wireless
endef


define Package/tr5220-eFEM-tr6560/install
$(call Package/tr5220/install,$(1))
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/T_6560_5220_2.4g_IPA_5G_EPA_cali.in $(1)/etc/wifi/cfg_vspm3xx_T.ini
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/C_6560_5220_2.4g_IPA_5G_EPA_cali.in $(1)/etc/wifi/cfg_vspm3xx_C.ini
endef

define Package/tr5220-iFEM-tr6560/install
$(call Package/tr5220/install,$(1))
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/T_6560_5220_2.4g_5G_ipa_cali.in $(1)/etc/wifi/cfg_vspm3xx_T.ini
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/C_6560_5220_2.4g_5G_ipa_cali.in $(1)/etc/wifi/cfg_vspm3xx_C.ini
endef


define Package/tr5220-iFEM-tr6560p/install
$(call Package/tr5220/install,$(1))
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/T_6560p_5220_2.4g_5G_ipa_cali.in $(1)/etc/wifi/cfg_vspm3xx_T.ini
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/C_6560p_5220_2.4g_5G_ipa_cali.in $(1)/etc/wifi/cfg_vspm3xx_C.ini
endef


define Package/tr5220-eFEM-tr6560p/install
$(call Package/tr5220/install,$(1))
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/T_6560p_5220_2.4g_5G_epa_cali.in $(1)/etc/wifi/cfg_vspm3xx_T.ini
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/config/C_6560p_5220_2.4g_5G_epa_cali.in $(1)/etc/wifi/cfg_vspm3xx_C.ini
endef




$(eval $(call BuildPackage,tr5220-eFEM-tr6560))
$(eval $(call BuildPackage,tr5220-iFEM-tr6560))
$(eval $(call BuildPackage,tr5220-eFEM-tr6560p))
$(eval $(call BuildPackage,tr5220-iFEM-tr6560p))
