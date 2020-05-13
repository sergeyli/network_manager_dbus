#include "connection.h"

#include "constants.h"

Connection::Connection(std::string_view object_path) : object_path_(object_path) {
  auto p_obj = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, nullptr, kNMBusName,
                                             object_path.data(), kNMBusSettingsConnectionIFaceName, nullptr, nullptr);

  if (p_obj) {
    settings_proxy_ = DBusProxyPtrT(p_obj, [](GDBusProxy *p_proxy) { g_object_unref(p_proxy); });
  } else {
    throw std::bad_alloc();
  }

  auto p_settings = g_dbus_proxy_call_sync(p_obj, "GetSettings", nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, nullptr);
  if (p_settings) {
    settings_ = GVariantPtrT(p_settings, [](GVariant *p) { g_variant_unref(p); });
  } else {
    throw std::bad_alloc();
  }
}

void Connection::Remove() const noexcept {
  auto ret =
      g_dbus_proxy_call_sync(settings_proxy_.get(), "Delete", nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, nullptr);
  if (ret != nullptr) {
    g_variant_unref(ret);
  }
}

std::string Connection::Id() const noexcept {
  if (id_.empty()) {
    auto conn =
        g_variant_lookup_value(g_variant_get_child_value(settings_.get(), 0), "connection", G_VARIANT_TYPE_DICTIONARY);
    if (conn) {
      auto conn_id = g_variant_lookup_value(conn, "id", G_VARIANT_TYPE_STRING);
      if (conn_id) {
        id_ = g_variant_get_string(conn_id, nullptr);
      }
    }
  }

  return id_;
}

std::string Connection::SSID() const noexcept {
  if (ssid_.empty()) {
    auto wifi = g_variant_lookup_value(g_variant_get_child_value(settings_.get(), 0), "802-11-wireless",
                                       G_VARIANT_TYPE_DICTIONARY);
    if (wifi) {
      auto g_ssid = g_variant_lookup_value(wifi, "ssid", nullptr);
      auto ssid_len = g_variant_get_size(g_ssid);
      auto ssid_array = g_variant_get_fixed_array(g_ssid, &ssid_len, sizeof(guint8));
      if (ssid_array) {
        ssid_ = std::string((char *)ssid_array, ssid_len);
      }
    }
  }

  return ssid_;
}
