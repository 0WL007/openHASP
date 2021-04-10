/* MIT License - Copyright (c) 2019-2021 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#ifndef HASP_DEVICE_ESP32_H
#define HASP_DEVICE_ESP32_H

#include "../device.h"

#if defined(ESP32)

namespace dev {

class Esp32Device : public BaseDevice {

  public:
    Esp32Device()
    {
#ifdef MQTT_NODENAME
        _hostname = MQTT_NODENAME;
#else
        _hostname      = "plate";
#endif
        _backlight_power  = 1;
        _backlight_invert = 0;
        _backlight_level  = 255;
#ifdef TFT_BCKL
        _backlight_pin = TFT_BCKL;
#else
        _backlight_pin = -1;
#endif
    }
    void reboot() override;
    void show_info() override;

    const char* get_hostname();
    void set_hostname(const char*);
    const char* get_core_version();
    const char* get_chip_model();

    void set_backlight_pin(uint8_t pin) override;
    void set_backlight_level(uint8_t val) override;
    uint8_t get_backlight_level() override;
    void set_backlight_power(bool power) override;
    bool get_backlight_power() override;

    size_t get_free_max_block() override;
    size_t get_free_heap() override;
    uint8_t get_heap_fragmentation() override;
    uint16_t get_cpu_frequency() override;

    bool is_system_pin(uint8_t pin) override;

  private:
    std::string _hostname;

    uint8_t _backlight_pin;
    uint8_t _backlight_level;
    uint8_t _backlight_power;
    uint8_t _backlight_invert;

    void update_backlight();
};

} // namespace dev

#if defined(LANBONL8)
#warning Building for Lanbon L8
#include "lanbonl8.h"
#elif defined(M5STACK)
#warning Building for M5Stack core2
#include "m5stackcore2.h"
#else
using dev::Esp32Device;
extern dev::Esp32Device haspDevice;
#endif
#endif // ESP32

#endif // HASP_DEVICE_ESP32_H