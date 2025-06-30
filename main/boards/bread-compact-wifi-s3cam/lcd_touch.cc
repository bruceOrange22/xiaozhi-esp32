#include "config.h"
#include "application.h"
#include "lcd_touch.h"
// #include "esp_lcd_touch_cst816s.h"
#include "esp_lvgl_port.h"
#include "esp_lcd_touch.h"
#include "idle_screen.h"

#if DISPLAY_TOUCH_AS_LISTEN_BUTTON
#include <esp_log.h>

#define TOUCH_PRESSED           LV_INDEV_STATE_PRESSED
#define TOUCH_RELEASED          LV_INDEV_STATE_RELEASED

static const char *TAG = "TOUCH";
// static esp_lcd_touch_handle_t touch_handle;
// static lv_indev_t *lvgl_touch_indev = NULL;
static int8_t lcd_touch_status;

// void lvgl_touch_cb(lv_indev_t *indev_drv, lv_indev_data_t *data);

#if IDLE_SCREEN_HOOK
void SpiLcdDisplayEx::IdleScrSetupUi() {
    ESP_LOGI(TAG, "IdleScrSetupUi()");

    _lcdScnIdle.ui_scnClock_screen_init(&current_theme_);
}

void SpiLcdDisplayEx::OnClockTimer() {
    _lcdScnIdle.ui_update(); // update screen every 1s, called by Application::OnClockTimer()
}

void SpiLcdDisplayEx::SetTheme(const std::string& theme_name) {
    LcdDisplay::SetTheme(theme_name);

    _lcdScnIdle.ui_updateTheme(&current_theme_);
}

void SpiLcdDisplayEx::OnStateChanged() {
    auto& app = Application::GetInstance();
    auto device_state = app.GetDeviceState();
    switch (device_state) {
        case kDeviceStateIdle:
            ESP_LOGI(TAG, "hide xiaozhi");
            if (!lv_obj_has_flag(container_, LV_OBJ_FLAG_HIDDEN)) {
                lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
            }
            _lcdScnIdle.ui_showScreen(true);
            break;

        default:
            ESP_LOGI(TAG, "show xiaozhi");
            if (lv_obj_has_flag(container_, LV_OBJ_FLAG_HIDDEN)) {
                lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
            }
            _lcdScnIdle.ui_showScreen(false);
            break;
    }
}
#endif // IDLE_SCREEN_HOOK

// void SimuOnPressDown() {
//     if (lcd_touch_status == TOUCH_PRESSED) {
//         return;
//     }

//     ESP_LOGI(TAG, "PressDown");

//     lcd_touch_status = TOUCH_PRESSED;
// }

// void SimuOnPressUp() {
//     if (lcd_touch_status == TOUCH_RELEASED) {
//         return;
//     }
//     ESP_LOGI(TAG, "PressUp");
//     auto& app = Application::GetInstance();
//     app.ToggleChatState();
//     // if (app.GetDeviceState() == kDeviceStateSpeaking) {
//     //     app.AbortSpeaking(kAbortReasonNone);
//     // }
//     lcd_touch_status = TOUCH_RELEASED;
// }

// take from 'esp_lvgl_port_touch.c'
typedef struct {
    esp_lcd_touch_handle_t  handle;     /* LCD touch IO handle */
    lv_indev_t              *indev;     /* LVGL input device driver */
} lvgl_port_touch_ctx_t;

// void lvgl_touch_cb(lv_indev_t *indev_drv, lv_indev_data_t *data)
// {
//     assert(indev_drv);
//     lvgl_port_touch_ctx_t *touch_ctx = (lvgl_port_touch_ctx_t *)lv_indev_get_driver_data(indev_drv);
//     assert(touch_ctx);
//     assert(touch_ctx->handle);

//     uint16_t touchpad_x[1] = {0};
//     uint16_t touchpad_y[1] = {0};
//     uint8_t touchpad_cnt = 0;

//     /* Read data from touch controller into memory */
//     esp_lcd_touch_read_data(touch_ctx->handle);

//     /* Read data from touch controller */
//     bool touchpad_pressed = esp_lcd_touch_get_coordinates(touch_ctx->handle, touchpad_x, touchpad_y, NULL, &touchpad_cnt, 1);

//     if (touchpad_pressed && touchpad_cnt > 0) {
//         data->point.x = touchpad_x[0];
//         data->point.y = touchpad_y[0];
//         data->state = LV_INDEV_STATE_PRESSED;
//         SimuOnPressDown();
//     } else {
//         data->state = LV_INDEV_STATE_RELEASED;
//         SimuOnPressUp();
//     }
// }

#else // DISPLAY_TOUCH_AS_LISTEN_BUTTON ==0
void InitializeTouch() {
}
#endif // DISPLAY_TOUCH_AS_LISTEN_BUTTON