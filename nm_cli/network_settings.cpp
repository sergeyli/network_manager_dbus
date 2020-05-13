#include "network_settings.h"

#include <iostream>

#include "constants.h"

NetworkSettings::NetworkSettings() {
  settings_proxy_ = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, nullptr, kNMBusName,
                                                  kNMBusSettingsIFacePath, kNMBusSettingsIFaceName, nullptr, nullptr);

  if (settings_proxy_ == nullptr) {
    throw std::bad_alloc();
  }
}

NetworkSettings::~NetworkSettings() {
  if (settings_proxy_ != nullptr) {
    g_object_unref(settings_proxy_);
  }
}

bool NetworkSettings::AddWiFiNetwork(std::string_view name, GVariant *ssid, std::string_view key_mgmt,
                                     std::string_view password) {
  bool error{false};
  ListConnections([&error, id = name](Connection settings) {
    if (settings.Id() == id) {
      error = true;
    }
  });

  if (!error) {
    auto params = g_variant_new_parsed(
        "({"
        "'connection': {'type': <%s>, 'id' : <%s>},"
        "'802-11-wireless' : { 'ssid': <%*>, 'security': <%s> },"
        "'802-11-wireless-security' : { 'key-mgmt': <%s>, 'psk':<%s>}"
        "},)",
        "802-11-wireless", name.data(), ssid, "802-11-wireless-security", key_mgmt.data(), password.data(),
        password.data());

    GError *p_error = nullptr;
    auto ret =
        g_dbus_proxy_call_sync(settings_proxy_, "AddConnection", params, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &p_error);

    // Free memory
    if (ret != nullptr) {
      g_variant_unref(ret);
    }

    if (p_error != nullptr) {
      std::cout << "AddConnection" << (p_error == nullptr ? "unknown" : p_error->message) << std::endl;
      g_error_free(p_error);
    }
  } else {
    // Free memory
    if (ssid != nullptr) {
      g_variant_unref(ssid);
    }
  }

  return error;
}

void NetworkSettings::RemoveNetwork(std::string_view id) {
  ListConnections([&](Connection settings) {
    if (settings.Id() == id) {
      settings.Remove();
    }
  });
}

void NetworkSettings::ListConnections(const NetworkSettings::ConnectionSettingsVisitor &visitor) {
  GError *error = nullptr;
  auto ret =
      g_dbus_proxy_call_sync(settings_proxy_, "ListConnections", nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

  if (ret == nullptr) {
    std::cout << "ERROR add conn S " << (error == nullptr ? "unknown" : error->message) << std::endl;
  }

  if (error) {
    g_error_free(error);
  }

  gchar *connection_settings_object_path;
  auto connection_settings_items = g_variant_get_child_value(ret, 0);
  auto connection_settings_iter = g_variant_iter_new(connection_settings_items);
  while (g_variant_iter_loop(connection_settings_iter, "o", &connection_settings_object_path)) {
    if (connection_settings_object_path) {
      visitor(Connection(connection_settings_object_path));
    }
  }

  // Free memory
  if (connection_settings_object_path != nullptr) {
    g_free(connection_settings_object_path);
  }
  if (connection_settings_items != nullptr) {
    g_variant_unref(connection_settings_items);
  }
  if (connection_settings_iter != nullptr) {
    g_variant_iter_free(connection_settings_iter);
  }
}
