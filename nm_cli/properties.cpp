#include "properties.h"

#include <iostream>

#include "constants.h"

Properties::~Properties() {
  if (device_property_proxy_ != nullptr) {
    g_object_unref(device_property_proxy_);
  }
}

Properties::Properties(const std::string& object_path, std::string interface_path)
    : interface_path_(std::move(interface_path)) {
  // Bus for properties
  device_property_proxy_ =
      g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, nullptr, kNMBusName,
                                    object_path.c_str(), kDBusPropertiesIFaceName, nullptr, nullptr);

  if (device_property_proxy_ == nullptr) {
    throw std::bad_alloc();
  }
}

GVariant* Properties::GetPropertyByName(const std::string& name) const noexcept {
  GError* p_error = nullptr;
  // Get packaged property
  auto packaged_property = g_dbus_proxy_call_sync(device_property_proxy_, "Get",
                                                  g_variant_new("(ss)", interface_path_.c_str(), name.c_str()),
                                                  G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &p_error);
  if (packaged_property == nullptr) {
    std::clog << "Get property " << (p_error == nullptr ? "unknown" : p_error->message) << std::endl;
  }

  // Free error memory
  if (p_error) {
    g_error_free(p_error);
  }

  // Extract property variant
  GVariant* property = nullptr;
  if (packaged_property != nullptr) {
    g_variant_get(packaged_property, "(v)", &property);

    // Free property memory
    g_variant_unref(packaged_property);
  }

  return property;
}

std::string Properties::GetStringPropertyByName(const std::string& name) const noexcept {
  auto property = GetPropertyByName(name);
  std::string property_str;
  if (property != nullptr) {
    property_str = g_variant_get_string(property, nullptr);

    // Free  memory
    g_variant_unref(property);
  }

  return property_str;
}
