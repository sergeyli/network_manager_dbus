#ifndef DBUS_TUTORIAL_NM_CLI_CONSTANTS_H_
#define DBUS_TUTORIAL_NM_CLI_CONSTANTS_H_

inline constexpr auto kNMBusName{"org.freedesktop.NetworkManager"};
inline constexpr auto kDBusPropertiesIFaceName{"org.freedesktop.DBus.Properties"};
inline constexpr auto kNMIFaceName{"org.freedesktop.NetworkManager"};
inline constexpr auto kNMIFacePath{"/org/freedesktop/NetworkManager"};
inline constexpr auto kNMBusSettingsIFacePath{"/org/freedesktop/NetworkManager/Settings"};
inline constexpr auto kNMBusSettingsIFaceName{"org.freedesktop.NetworkManager.Settings"};
inline constexpr auto kNMBusSettingsConnectionIFaceName{"org.freedesktop.NetworkManager.Settings.Connection"};
inline constexpr auto kNMBusDeviceIFaceName{"org.freedesktop.NetworkManager.Devices.Device"};

#endif  // DBUS_TUTORIAL_NM_CLI_CONSTANTS_H_
