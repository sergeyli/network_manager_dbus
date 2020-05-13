#ifndef DBUS_TUTORIAL_NM_CLI_CONNECTION_H_
#define DBUS_TUTORIAL_NM_CLI_CONNECTION_H_

#include <gio/gio.h>

#include <functional>
#include <memory>
#include <string>
#include <string_view>

class Connection {
 public:
  /**
   * @brief Constructor of connection class
   * @param object_path Connection setting path (e.g: /org/freedesktop/NetworkManager/Settings/1)
   */
  explicit Connection(std::string_view object_path);
  ~Connection() = default;

  Connection(const Connection &) = delete;
  Connection &operator=(const Connection &) = delete;
  Connection(Connection &&) = default;
  Connection &operator=(Connection &&) = default;

  /**
   * @brief Delete connection from network manager
   */
  void Remove() const noexcept;

  /**
   * @brief Gets identification of connection
   * @return Connection id
   */
  std::string Id() const noexcept;

  /**
   * @brief Gets Service Set Identifier
   * @return Service Set Identifier
   */
  std::string SSID() const noexcept;

  /**
   * @brief Get path to connection
   * @return Path to connection (e.g: /org/freedesktop/NetworkManager/Settings/1)
   */
  std::string Path() const noexcept { return object_path_; }

 private:
  mutable std::string id_;
  mutable std::string ssid_;

  using DBusProxyPtrT = std::unique_ptr<GDBusProxy, std::function<void(GDBusProxy *)>>;
  using GVariantPtrT = std::unique_ptr<GVariant, std::function<void(GVariant *)>>;
  std::string object_path_;
  GVariantPtrT settings_;
  DBusProxyPtrT settings_proxy_;
};

#endif  // DBUS_TUTORIAL_NM_CLI_CONNECTION_H_
