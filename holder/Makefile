#
# This software is licensed under the CC0-1.0 license.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=holder
PKG_VERSION:=1.1
PKG_RELEASE:=1
PKG_LICENSE:=CC0-1.0

include $(INCLUDE_DIR)/package.mk

define Package/holder
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:=+libcurl +libopenssl +libstdcpp
	TITLE:=Holder package
	MAINTAINER:=Moritz Warning <moritzwarning@web.de>
endef

define Package/holder/description
	Holder is a package that compiles and installs a local src/main.cpp file.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/holder/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/holder $(1)/usr/bin/holder

	$(CP) ./files/* $(1)/
endef

define Build/Compile
	$(TARGET_CXX) $(TARGET_CXXFLAGS) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/holder $(PKG_BUILD_DIR)/main.cpp -lcurl -lssl -lcrypto
endef

$(eval $(call BuildPackage,holder))