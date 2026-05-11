#pragma once

#include <stdint.h>

/* ---- Packet framing ---- */
#define M1_CMD_MAGIC   0xAB
#define M1_RESP_MAGIC  0xCD

#define M1_MAX_PAYLOAD 60

/* ---- Command IDs ---- */

/* System */
#define CMD_PING              0x01
#define CMD_GET_STATUS        0x02

/* CMD_GET_STATUS payload (protocol version 1)
 *
 * The status payload reports a single transport-agnostic capability bitmap.
 * Each M1_ESP32_CAP_* bit corresponds to a named feature the firmware
 * supports, regardless of how it is implemented internally (binary SPI
 * command vs. AT text command).  The host (STM32) gates ESP32-dependent
 * features at runtime by checking these bits.
 *
 * Bit assignments here MUST match the M1 STM32 `m1_esp32_caps.h`
 * definitions exactly — they are part of the wire protocol.
 */
#define M1_ESP32_CAPS_PROTO_VER 1u

/* WiFi scan (AP discovery) */
#define M1_ESP32_CAP_WIFI_SCAN      (1ull <<  0)
/* Station scan (promiscuous client discovery) */
#define M1_ESP32_CAP_STA_SCAN       (1ull <<  1)
/* BLE scan */
#define M1_ESP32_CAP_BLE_SCAN       (1ull <<  2)
/* BLE advertisement (custom adv payloads, spam, spoofing) */
#define M1_ESP32_CAP_BLE_ADV        (1ull <<  3)
/* WiFi deauthentication attack */
#define M1_ESP32_CAP_DEAUTH         (1ull <<  4)
/* Beacon flooding / clone / rickroll */
#define M1_ESP32_CAP_BEACON         (1ull <<  5)
/* Probe request flooding */
#define M1_ESP32_CAP_PROBE_FLOOD    (1ull <<  6)
/* Karma access-point impersonation + optional captive portal */
#define M1_ESP32_CAP_KARMA          (1ull <<  7)
/* Packet sniffer / monitor mode */
#define M1_ESP32_CAP_PKTMON         (1ull <<  8)
/* Evil portal (custom captive HTML + credential capture) */
#define M1_ESP32_CAP_PORTAL         (1ull <<  9)
/* WiFi station join / disconnect */
#define M1_ESP32_CAP_WIFI_JOIN      (1ull << 10)
/* WiFi MAC address spoofing */
#define M1_ESP32_CAP_WIFI_SET_MAC   (1ull << 11)
/* WiFi channel override */
#define M1_ESP32_CAP_WIFI_SET_CHAN  (1ull << 12)
/* Network scanner (ping / ARP / port / SSH / Telnet) */
#define M1_ESP32_CAP_NETSCAN        (1ull << 13)
/* BLE HID keyboard emulation (Bad-BT) */
#define M1_ESP32_CAP_BLE_HID        (1ull << 14)
/* Bluetooth device management (saved devices, BT info) */
#define M1_ESP32_CAP_BT_MANAGE      (1ull << 15)
/* IEEE 802.15.4 / Zigbee / Thread */
#define M1_ESP32_CAP_802154         (1ull << 16)
/* Bits 17-63 reserved for future use */

/* Capability profile reported by this firmware in CMD_GET_STATUS.
 * Lists every capability the SiN360 ESP32-C6 firmware actually implements. */
#define M1_ESP32_CAP_PROFILE_SIN360 \
    (M1_ESP32_CAP_WIFI_SCAN     | \
     M1_ESP32_CAP_STA_SCAN      | \
     M1_ESP32_CAP_BLE_SCAN      | \
     M1_ESP32_CAP_BLE_ADV       | \
     M1_ESP32_CAP_DEAUTH        | \
     M1_ESP32_CAP_BEACON        | \
     M1_ESP32_CAP_PROBE_FLOOD   | \
     M1_ESP32_CAP_KARMA         | \
     M1_ESP32_CAP_PKTMON        | \
     M1_ESP32_CAP_PORTAL        | \
     M1_ESP32_CAP_WIFI_JOIN     | \
     M1_ESP32_CAP_WIFI_SET_MAC  | \
     M1_ESP32_CAP_WIFI_SET_CHAN | \
     M1_ESP32_CAP_NETSCAN       | \
     M1_ESP32_CAP_BLE_HID)

/* WiFi scan (preserve existing functionality) */
#define CMD_WIFI_SCAN_START   0x10
#define CMD_WIFI_SCAN_NEXT    0x11
#define CMD_WIFI_SCAN_STOP    0x12

/* BLE */
#define CMD_BLE_SCAN_START    0x20
#define CMD_BLE_SCAN_NEXT     0x21
#define CMD_BLE_SCAN_STOP     0x22
#define CMD_BLE_ADV_START     0x23
#define CMD_BLE_ADV_STOP      0x24
#define CMD_BLE_ADV_RAW       0x25
#define CMD_BLE_SCAN_NEXT_RAW 0x26
#define CMD_BLE_ADV_RAW_EX    0x27
#define CMD_BLE_GATT_START    0x28
#define CMD_BLE_GATT_NEXT     0x29
#define CMD_BLE_GATT_STOP     0x2A
#define CMD_BLE_GATT_WRITE    0x2B
#define CMD_BLE_GATT_SUB      0x2C
#define CMD_BLE_GATT_NOTIF    0x2D
#define CMD_BLE_HID_START     0x2E
#define CMD_BLE_HID_STOP      0x2F

/* Station scan (promiscuous client discovery) */
#define CMD_STA_SCAN_START    0x13
#define CMD_STA_SCAN_NEXT     0x14
#define CMD_STA_SCAN_STOP     0x15

/* WiFi attacks */
#define CMD_DEAUTH_START      0x30
#define CMD_DEAUTH_STOP       0x31
#define CMD_BEACON_START      0x32
#define CMD_BEACON_STOP       0x33
#define CMD_PROBE_FLOOD_START 0x34
#define CMD_PROBE_FLOOD_STOP  0x35
#define CMD_BEACON_SET_FLAGS  0x36
#define CMD_KARMA_START       0x37
#define CMD_KARMA_STOP        0x38
#define CMD_DEAUTH_MULTI      0x39
#define CMD_KARMA_STATUS      0x3A
#define CMD_KARMA_PORTAL_START 0x3B
#define CMD_WIFI_RAW_ATTACK_START 0x3C
#define CMD_WIFI_RAW_ATTACK_STOP  0x3D

/* Sniffer / packet monitor
 * START payload[0] = sniff type, [1] = channel (0=hop), [2] = hop interval */
#define CMD_PKTMON_START      0x40
#define CMD_PKTMON_NEXT       0x41
#define CMD_PKTMON_STOP       0x42
#define CMD_PKTMON_SET_CHAN   0x43
#define CMD_PKTMON_RAW_NEXT   0x44

/* Sniffer type constants */
#define SNIFF_ALL             0x00
#define SNIFF_BEACON          0x01
#define SNIFF_PROBE_REQ       0x02
#define SNIFF_DEAUTH          0x03
#define SNIFF_EAPOL           0x04
#define SNIFF_SIGNAL          0x05
#define SNIFF_PWNAGOTCHI      0x06
#define SNIFF_SAE             0x07

/* Evil portal */
#define CMD_PORTAL_START      0x50
#define CMD_PORTAL_STOP       0x51
#define CMD_PORTAL_CREDS      0x52
#define CMD_PORTAL_HTML_CLEAR 0x56
#define CMD_PORTAL_HTML_ADD   0x57

/* SSID management */
#define CMD_SSID_ADD          0x53
#define CMD_SSID_CLEAR        0x54
#define CMD_SSID_COUNT        0x55

/* WiFi general */
#define CMD_WIFI_JOIN         0x58
#define CMD_WIFI_DISCONNECT   0x59
#define CMD_WIFI_SET_MAC      0x5A
#define CMD_WIFI_SET_CHANNEL  0x5B
#define CMD_NETSCAN_START     0x5C
#define CMD_NETSCAN_NEXT      0x5D
#define CMD_NETSCAN_STOP      0x5E
#define CMD_BLE_HID_REPORT    0x60
#define CMD_BLE_HID_STATUS    0x61

/* ---- Response status ---- */
#define RESP_OK    0x00
#define RESP_ERR   0x01
#define RESP_BUSY  0x02

/* ---- Packet structures ---- */

/* Command: STM32 -> ESP32 (64 bytes) */
typedef struct {
    uint8_t  magic;           /* M1_CMD_MAGIC */
    uint8_t  cmd_id;
    uint8_t  payload_len;
    uint8_t  payload[61];
} __attribute__((packed)) m1_cmd_t;

/* Response: ESP32 -> STM32 (64 bytes) */
typedef struct {
    uint8_t  magic;           /* M1_RESP_MAGIC */
    uint8_t  cmd_id;
    uint8_t  status;
    uint8_t  payload_len;
    uint8_t  payload[M1_MAX_PAYLOAD];
} __attribute__((packed)) m1_resp_t;

typedef struct {
    uint8_t  proto_ver;
    uint64_t cap_bitmap;
    char     fw_name[32];
} __attribute__((packed)) m1_esp32_status_payload_t;

_Static_assert(sizeof(m1_cmd_t) == 64, "m1_cmd_t must be 64 bytes");
_Static_assert(sizeof(m1_resp_t) == 64, "m1_resp_t must be 64 bytes");
_Static_assert(sizeof(m1_esp32_status_payload_t) == 41,
               "m1_esp32_status_payload_t must be 41 bytes");
