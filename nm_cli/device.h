#ifndef DBUS_TUTORIAL_NM_CLI_DEVICE_H_
#define DBUS_TUTORIAL_NM_CLI_DEVICE_H_

#include <string>
#include <string_view>

#include "properties.h"

class Device {
 public:
  explicit Device(std::string_view device_object_path);
  ~Device();

  // Disable copy
  Device(const Device &) = delete;
  Device &operator=(const Device &) = delete;
  Device(Device &&) = default;
  Device &operator=(Device &&) = default;

  /**
   * @brief Get path to connection
   * @return Path to connection (e.g: /org/freedesktop/NetworkManager/Devices/1)
   */
  [[nodiscard]] std::string Path() const noexcept { return device_object_path_; }

  /**
   * @brief Get device interface
   * @return Name of device interface
   */
  [[nodiscard]] std::string Interface() const noexcept;

  /**
   * @brief Check if device type is wireless
   * @return True if device type is wireless, false otherwise
   */
  [[nodiscard]] bool IsWireless() const noexcept;

 private:
  std::string device_object_path_;
  Properties properties_;
  GDBusProxy *device_proxy_;
};

#endif  // DBUS_TUTORIAL_NM_CLI_DEVICE_H_
