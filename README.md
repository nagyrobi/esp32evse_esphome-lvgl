# esp32-evse_esphome-lvgl
## HMI for ESP32-EVSE based on ESPHome and LVGL

This is a replacement Human-Machine Interface for the Nextion screen of the excellent [ESP32-EVSE project by Miroslav Dzúrik](https://github.com/dzurikmiroslav/esp32-evse).
It relies on the [external ESPHoome component](https://github.com/nagyrobi/esp32evse-esphome) based AT commands protocol. 

![Screen](images/esp32-evse_esphome-lvgl.gif)

ESPHome supports [LVGL graphics library](https://esphome.io/components/lvgl/). Using ESPHome as a framework instead of trying to implement LVGL natively has the advantage that besides making it a lot more simpler to configure, it brings in all the modularity of the ecosystem: 
- any graphical UI can be rendered via LVGL, with fully open source resources (no proprietary editor needed), just edit a YAML file
- multiple UARTs can be configured (eg one talking to `esp32-evse`, other talking to smart meter or inverter etc.)
- can use free GPIOs on the display for anything else, like garage door openers, RF transmitters, RFiD readers for authorization etc. in addition to the AUXes of the EVSE, with all the logic offloaded from there
- has Bluetooth and with it you could pair with your car and implement additional related features (eg open charging flap automatically, poll battery status, temperature, etc., which wouldn't have to interfere with EVSE board at all)
- can connect to Home Assistant via native API (or MQTT to other systems), can expose any chosen EVSE sensors and controls to it
- easier to make own Dynamic Load Management through HA or other ESPHome components (eg use data from other meters in various ways; freely implement DLM logic either in HA or in ESPHome based on any available data soruces)

The device consists of any graphical display with touch screen supported by [ESPHome](https://esphome.io/components/#display-hardware-platforms). The example file _esp32-evse_esphome-lvgl_display.yaml_ contains example configuration for displays like Wireless-Tag/Panlee WT32-SC01 Plus, Guition JC3248w535 which can be connected directly to ESP32-EVSE board.
What makes these a good choice is:
- has a pretty nice case that can be mounted on a box which has `esp32-evse` inside
- has resolution of 480*320
- has capacitive touch screen
- it runs an ESP32-S3
- has a considerable number of additional free GPIOs
- has a speaker output, possible to play audible warnings
- way less expensive than Nextion

Any other ESPHome supported graphical display could be used, the config file needs to be adjusted accordingly. If the resolution differs, placement of the widgets, font sizes may need correction too.

## Preparation

The display communicates with ESP32-EVSE (min. version 2.0.0) via UART at 921600 baud rate. 
Enter your ESP32-EVSE web UI and in _Settings_ > _Serial_, select for the UART port _Mode_: _AT Commands_. _Baud rate_: _921600_,  _Data bits_: _8_,  _Stop bits_: _1_, _Parity_: _Disable_. 
Press Submit and reboot the evse from  _System_ > _Restart_.

If you use the [esp32s2-evse-d-a board](https://github.com/dzurikmiroslav/esp32-evse/wiki/ESP32-S2-DA) you can use a 4-pin cable with 2.5mm JST wired correctly for RX/TX/GND/5V:

| ESP32-EVSE U6 UART (1 leftmost looking from top, 2.5mm JST) | Guition JC3248w535 P1 (1 closest to USB-C, 1.25mm JST) | WT32-SC01 Plus I/O (leftmost under R11) |
| -------- | ------- | ------- |
| 1: GND | 1: GND | 1: +5V |
| 2: TX | 2: RX | 2: GND |
| 3: RX | 3: TX | 3: TX |
| 4: +5V | 4: +5V | 4: RX |

## Installation

If you're not familiar yet with ESPHome, check out their [Getting Started guide](https://esphome.io/guides/getting_started_hassio). You need at least version 2025.5. I recommend to use their Dashboard.
Place the ready-made _esp32-evse_esphome-lvgl_display.yaml_ configuration file from this repo in ESPHome's config directory. In the ``packages:`` section make sure you only have uncommented the line corresponding to your display. Create your own `secrets.yaml` file in the same directory with your own data:
```yaml
wifi_ssid: YOUR_WIFI_SSID
wifi_password: YOUR_WIFI_KEY
ota_password: CHOOSE_A_PASSWORD
encryption_key: GENERATE_A_KEY
```
If you look at the configuartion file, you'll notice that the encryption key is required by Home Assistant API. [See the doc](https://esphome.io/components/api.html) how to generate an encryption string manually.
_Note:_ It's not mandatory to use Home Assistant. If you don't use it, remove the entire `api:` key from the configuration, to avoid periodic self-reboot of the display waiting for it to connect.

In ESPHome Dashboard click the 3-dots icon corresponding to the configuration YAML file and choose _Install_ > _Manual download_. Let it do it's thing, and at the end it should prompt you to save a `.bin` file (choose the _factory_ format) which you can flash to your device [in a number of ways](https://esphome.io/guides/faq.html#how-do-i-install-esphome-onto-my-device). _Guition JC3248w535_ display can be connected to the PC directly with an USB-C cable, can be flashed through it's built-in UART (just keep the BOOT button pressed while pluggig it in to enter bootloader mode).

After initial flashing, any future changes can be uploaded wirelessly, no need to physically access the screen anymore.
