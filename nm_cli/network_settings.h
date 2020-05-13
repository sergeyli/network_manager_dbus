#ifndef DBUS_TUTORIAL_NM_CLI_NETWORK_SETTINGS_H_
#define DBUS_TUTORIAL_NM_CLI_NETWORK_SETTINGS_H_

#include <gio/gio.h>

#include <functional>
#include <string_view>

#include "connection.h"

class NetworkSettings {
  using ConnectionSettingsVisitor = std::function<void(Connection)>;

 public:
  explicit NetworkSettings();
  ~NetworkSettings();

  NetworkSettings(const NetworkSettings &) = delete;
  NetworkSettings &operator=(const NetworkSettings &) = delete;
  NetworkSettings(NetworkSettings &&) = default;
  NetworkSettings &operator=(NetworkSettings &&) = default;

  [[nodiscard]] bool AddWiFiNetwork(std::string_view name, GVariant *ssid, std::string_view key_mgmt,
                                    std::string_view password);
  void RemoveNetwork(std::string_view id);
  void ListConnections(const ConnectionSettingsVisitor &visitor);

 private:
  GDBusProxy *settings_proxy_;
};

#endif  // DBUS_TUTORIAL_NM_CLI_NETWORK_SETTINGS_H_
