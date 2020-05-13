#include "manager.h"

#include <iostream>

#include "constants.h"

Manager::Manager() {
  nm_proxy_ = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, nullptr, kNMBusName,
                                            kNMIFacePath, kNMIFaceName, nullptr, nullptr);
  if (nm_proxy_ == nullptr) {
    throw std::bad_alloc();
  }
}

Manager::~Manager() {
  if (nm_proxy_ != nullptr) {
    g_object_unref(nm_proxy_);
  }
}

bool Manager::AddWifiNetwork(std::string_view ssid, std::string_view key_mgmt, std::string_view password) {
  auto ssid_len = ssid.size();
  GBytes *p_bytes = g_bytes_new(ssid.data(), ssid_len);
  auto error = nm_settings_.AddWiFiNetwork(ssid, g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, ssid_len),
                                           key_mgmt, password);

  // Free memory
  if (p_bytes != nullptr) {
    g_bytes_unref(p_bytes);
  }

  return error;
}

void Manager::RemoveNetwork(std::string_view id) { nm_settings_.RemoveNetwork(id); }

bool Manager::ActivateNetwork(std::string_view id) {
  // Get Wireless device
  std::string device;
  this->ListDevices([&device](Device &&visitor) {
    if (visitor.IsWireless()) {
      device = visitor.Path();
    }
  });

  // Get connection
  std::string connection;
  nm_settings_.ListConnections([&connection, &id](Connection&& settings) {
    if (settings.Id() == id) {
      connection = settings.Path();
    }
  });

  std::clog << "Device: " << device << " | connection path: " << connection << std::endl;

  bool error{false};
  // Setup error
  if (device.empty() || connection.empty()) {
    error = true;
  }

  // Activate
  if (!error) {
    GError *p_error = nullptr;
    auto ret = g_dbus_proxy_call_sync(nm_proxy_, "ActivateConnection",
                                      g_variant_new("(ooo)", connection.c_str(), device.c_str(), "/"),
                                      G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &p_error);
    if (ret == nullptr) {
      error = true;
      std::cout << "AddConnection" << (p_error == nullptr ? "unknown" : p_error->message) << std::endl;
    } else {
      device = g_variant_get_string(g_variant_get_child_value(ret, 0), nullptr);
      std::cout << g_variant_print(ret, true) << std::endl;

      // Free variant memory
      g_variant_unref(ret);
    }
    // Free error memory
    if (p_error) {
      g_error_free(p_error);
    }
  }

  return error;
}

void Manager::ListDevices(const Manager::DevicesVisitor &visitor) {
  GError *p_error = nullptr;
  auto list_devices =
      g_dbus_proxy_call_sync(nm_proxy_, "GetAllDevices", nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &p_error);

  if (list_devices == nullptr) {
    std::cout << "ERROR get list of devices " << (p_error == nullptr ? "unknown" : p_error->message) << std::endl;
  }

  if (p_error) {
    g_error_free(p_error);
  }

  gchar *device_object_path;
  auto device_items = g_variant_get_child_value(list_devices, 0);
  auto device_iter = g_variant_iter_new(device_items);
  while (g_variant_iter_loop(device_iter, "o", &device_object_path)) {
    if (device_object_path != nullptr) {
      visitor(Device(device_object_path));
    }
  }

  // Free memory
  if (device_object_path != nullptr) {
    g_free(device_object_path);
  }
  if (device_items != nullptr) {
    g_variant_unref(device_items);
  }
  if (device_iter != nullptr) {
    g_variant_iter_free(device_iter);
  }
}
