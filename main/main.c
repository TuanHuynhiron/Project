#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "driver/ledc.h"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

static const char *TAG = "PWM_WEB";
static int pwm_value = 0;   // giá trị 0–255

// ======= HTML Trang web =======
static const char *HTML_PAGE =
"<!DOCTYPE html>"
"<html>"
"<head><meta charset='utf-8'><title>ESP32 PWM</title></head>"
"<body style='font-family:Arial; font-size:18px;'>"
"<h2>Điều khiển PWM từ ESP32</h2>"
"<p>Giá trị PWM hiện tại: <b id='pwm_val'>%d</b></p>"

"<form action='/sethex' method='post'>"
"Nhập mã HEX (0x00–0xFF): <input type='text' name='hex'/>"
"<input type='submit' value='Gửi HEX'/>"
"</form><br>"

"<form action='/setpercent' method='post'>"
"Nhập phần trăm (0–100): <input type='text' name='percent'/>"
"<input type='submit' value='Gửi pre'/>"
"</form>"

"<script>"
"setInterval(()=>{fetch('/val').then(r=>r.text()).then(t=>{document.getElementById('pwm_val').innerText=t;});},1000);"
"</script>"
"</body>"
"</html>";

// ======= PWM Setup =======
static void pwm_init(void) {
    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_8_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 5000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num   = 2, // GPIO xuất PWM
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&channel);
}

static void pwm_set(int duty) {
    pwm_value = duty;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, pwm_value);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}

// ======= HTTP Handlers =======
static esp_err_t root_get_handler(httpd_req_t *req) {
    char buf[1024];
    int len = snprintf(buf, sizeof(buf), HTML_PAGE, pwm_value);
    httpd_resp_send(req, buf, len);
    return ESP_OK;
}

static esp_err_t sethex_post_handler(httpd_req_t *req) {
    char buf[100];
    int ret = httpd_req_recv(req, buf, MIN(req->content_len, sizeof(buf)-1));
    if (ret > 0) {
        buf[ret] = '\0';
        char hexstr[10];
        if (sscanf(buf, "hex=%9s", hexstr) == 1) {
            int val = (int)strtol(hexstr, NULL, 16);
            if (val < 0) val = 0;
            if (val > 255) val = 255;
            pwm_set(val);
            ESP_LOGI(TAG, "Set PWM HEX = %d", val);
        }
    }
    httpd_resp_set_status(req, "303 See Other");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t setpercent_post_handler(httpd_req_t *req) {
    char buf[100];
    int ret = httpd_req_recv(req, buf, MIN(req->content_len, sizeof(buf)-1));
    if (ret > 0) {
        buf[ret] = '\0';
        int percent = 0;
        if (sscanf(buf, "percent=%d", &percent) == 1) {
            if (percent < 0) percent = 0;
            if (percent > 100) percent = 100;
            int val = percent * 255 / 100;
            pwm_set(val);
            ESP_LOGI(TAG, "Set PWM %% = %d -> duty %d", percent, val);
        }
    }
    httpd_resp_set_status(req, "303 See Other");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t val_get_handler(httpd_req_t *req) {
    char buf[16];
    int len = snprintf(buf, sizeof(buf), "%d", pwm_value);
    httpd_resp_send(req, buf, len);
    return ESP_OK;
}

// ======= Start Web Server =======
static httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_get_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &root_uri);

        httpd_uri_t sethex_uri = {
            .uri = "/sethex",
            .method = HTTP_POST,
            .handler = sethex_post_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &sethex_uri);

        httpd_uri_t setpercent_uri = {
            .uri = "/setpercent",
            .method = HTTP_POST,
            .handler = setpercent_post_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &setpercent_uri);

        httpd_uri_t val_uri = {
            .uri = "/val",
            .method = HTTP_GET,
            .handler = val_get_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &val_uri);
    }
    return server;
}

// ======= WiFi AP =======
static void wifi_init_softap(void) {
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t ap_config = {
        .ap = {
            .ssid = "ESP32_PWM",
            .ssid_len = strlen("ESP32_PWM"),
            .channel = 1,
            .max_connection = 4,
            .authmode = WIFI_AUTH_OPEN
        },
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "WiFi AP started. SSID: ESP32_PWM");
}

// ======= Main =======
void app_main(void) {
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();

    wifi_init_softap();
    pwm_init();
    start_webserver();
}
