#include "device.h"

#include "constants.h"

#include <iostream>

Device::Device(std::string_view device_object_path)
    : device_object_path_(device_object_path),
      properties_(device_object_path_, "org.freedesktop.NetworkManager.Device") {
  // Bus for devices
  device_proxy_ = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, nullptr, kNMBusName,
                                                device_object_path_.c_str(), kNMBusDeviceIFaceName, nullptr, nullptr);

  if (device_proxy_ == nullptr) {
    throw std::bad_alloc();
  }
}

Device::~Device() {
  if (device_proxy_ != nullptr) {
    g_object_unref(device_proxy_);
  }
}

std::string Device::Interface() const noexcept { return properties_.GetStringPropertyByName("Interface"); }

bool Device::IsWireless() const noexcept {
  // Trying to get Wireless interface
  auto p_device = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, nullptr, kNMBusName,
                                                device_object_path_.c_str(),
                                                "org.freedesktop.NetworkManager.Devices.Wireless", nullptr, nullptr);

  if (p_device != nullptr) {
    g_object_unref(p_device);

    // Case insensitive find wireless property
    auto interface = Interface();
    std::transform(std::begin(interface), std::end(interface), std::begin(interface),
                   [](unsigned char c) { return std::toupper(c); });
    if (interface.find("WLAN") != std::string::npos) {
      return true;
    }
  }

  return false;
}
