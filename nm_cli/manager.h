#ifndef DBUS_TUTORIAL_NM_CLI_MANAGER_H_
#define DBUS_TUTORIAL_NM_CLI_MANAGER_H_

#include <gio/gio.h>

#include <functional>
#include <string_view>

#include "device.h"
#include "network_settings.h"

class Manager {
  using DevicesVisitor = std::function<void(Device)>;

 public:
  explicit Manager();
  ~Manager();

  bool AddWifiNetwork(std::string_view ssid, std::string_view key_mgmt, std::string_view password);
  void RemoveNetwork(std::string_view id);
  bool ActivateNetwork(std::string_view id);

  void ListDevices(const DevicesVisitor& visitor);

 private:
  GDBusProxy* nm_proxy_{nullptr};
  NetworkSettings nm_settings_;
};

#endif  // DBUS_TUTORIAL_NM_CLI_MANAGER_H_
