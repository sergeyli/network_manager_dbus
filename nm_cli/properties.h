#ifndef DBUS_TUTORIAL_NM_CLI_PROPERTIES_H_
#define DBUS_TUTORIAL_NM_CLI_PROPERTIES_H_

#include <gio/gio.h>

#include <functional>
#include <memory>
#include <string>

class Properties {
 public:
  explicit Properties(const std::string &object_path, std::string interface_path);
  ~Properties();

  // Disable copy
  Properties(const Properties &) = delete;
  Properties &operator=(const Properties &) = delete;
  Properties(Properties &&) = default;
  Properties &operator=(Properties &&) = default;

  /**
   * @brief Get property by name
   * @param name Property name
   * @return Property
   */
  [[nodiscard]] GVariant *GetPropertyByName(const std::string &name) const noexcept;
  [[nodiscard]] std::string GetStringPropertyByName(const std::string &name) const noexcept;

 private:
  GDBusProxy *device_property_proxy_;
  std::string interface_path_;
};

#endif  // DBUS_TUTORIAL_NM_CLI_PROPERTIES_H_
