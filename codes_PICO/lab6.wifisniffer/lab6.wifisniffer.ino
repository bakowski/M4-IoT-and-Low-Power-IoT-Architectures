#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#define uS_TO_S_FACTOR 1000000  /* Conversion factor to seconds */
#define TIME_TO_SLEEP  15       /* Time ESP32 will go to sleep (in seconds) */

esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);

uint8_t beacon_raw[] = {
  0x80, 0x00,      // 0-1: Frame Control: subtype 80 - beacon, type 00 MNGM
  0x00, 0x00,      // 2-3: Duration
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,       // 4-9: Destination address (broadcast)
  0xba, 0xde, 0xaf, 0xfe, 0x00, 0x06,       // 10-15: Source address
  0xba, 0xde, 0xaf, 0xfe, 0x00, 0x06,       // 16-21: BSSID
  0x00, 0x00,             // 22-23: Sequence / fragment number
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,   // 24-31: Timestamp 
  0x64, 0x00,             // 32-33: Beacon interval
  0x31, 0x04,             // 34-35: Capability info
  0x00, 0x00,  // 36-38: SSID parameter set, 0x00:length:content
    0x01,0x08,0x82,0x84,0x8b,0x96,0x0c,0x12,0x18,0x24, // 39-48: Supported rates
  0x03, 0x01, 0x01,    // 49-51: DS Parameter set, current channel 1 (= 0x01),
  0x05, 0x04, 0x01, 0x02, 0x00, 0x00,  // 52-57: Traffic Indication Map
};

char *my_ssids[] = {                   // dummy access points
  "01 Hello from SmartComputerLab",
  "02 Hello from SmartComputerLab",
  "03 Hello from SmartComputerLab",
  "04 Hello from SmartComputerLab",
};

#define BEACON_SSID_OFFSET 38
#define SRCADDR_OFFSET 10
#define BSSID_OFFSET 16
#define SEQNUM_OFFSET 22
#define TOTAL_LINES (sizeof(my_ssids) / sizeof(char *))

esp_err_t event_handler(void *ctx, system_event_t *event) { return ESP_OK;}

int count=120;
void spam_task(void *pvParameter) {
  uint8_t line = 0;
  // Keep track of beacon sequence numbers on a per-songline-basis
  uint16_t seqnum[TOTAL_LINES] = { 0 };
  for (;;) {
    vTaskDelay(100 / TOTAL_LINES / portTICK_PERIOD_MS);
    printf("%i %i %s\r\n", strlen(my_ssids[line]), TOTAL_LINES, my_ssids[line]);
    uint8_t beacon_rick[200];
    memcpy(beacon_rick, beacon_raw, BEACON_SSID_OFFSET - 1);
    beacon_rick[BEACON_SSID_OFFSET - 1] = strlen(my_ssids[line]);
    memcpy(&beacon_rick[BEACON_SSID_OFFSET], my_ssids[line], strlen(my_ssids[line]));
    memcpy(&beacon_rick[BEACON_SSID_OFFSET + strlen(my_ssids[line])], &beacon_raw[BEACON_SSID_OFFSET], sizeof(beacon_raw) - BEACON_SSID_OFFSET);
    // Last byte of source address/BSSID will be line number 
    beacon_rick[SRCADDR_OFFSET + 5] = line;
    beacon_rick[BSSID_OFFSET + 5] = line;
    // Update sequence number
    beacon_rick[SEQNUM_OFFSET] = (seqnum[line] & 0x0f) << 4;
    beacon_rick[SEQNUM_OFFSET + 1] = (seqnum[line] & 0xff0) >> 4;
    seqnum[line]++;
    if (seqnum[line] > 0xfff)
      seqnum[line] = 0;
    esp_wifi_80211_tx(WIFI_IF_AP, beacon_rick, sizeof(beacon_raw) + strlen(my_ssids[line]), false);
    if (++line >= TOTAL_LINES)
      line = 0;
  if(count<0)
    { esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start(); count=30; }
    else count--;
  }
}
void setup(void) {
  Serial.begin(115200);
  nvs_flash_init();
  tcpip_adapter_init();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  // Init dummy AP 
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
  xTaskCreate(&spam_task, "spam_task", 2048, NULL, 5, NULL);
}

void loop(){}
