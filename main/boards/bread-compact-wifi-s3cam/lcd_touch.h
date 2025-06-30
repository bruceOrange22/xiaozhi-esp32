#ifndef _LCD_TOUCH_H

#include <lvgl.h>
#include "lcd_display.h"
#include "idle_screen.h"

// SPI LCD 触摸屏
class SpiLcdDisplayEx : public SpiLcdDisplay {
public:
    SpiLcdDisplayEx(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                  int width, int height, int offset_x, int offset_y,
                  bool mirror_x, bool mirror_y, bool swap_xy,
                  DisplayFonts fonts) :
                  SpiLcdDisplay(panel_io, panel,
                  width, height, offset_x, offset_y,
                  mirror_x, mirror_y, swap_xy,
                  fonts) {
        DisplayLockGuard lock(this);
        lv_obj_set_style_pad_left(status_bar_, 20, 0);
        lv_obj_set_style_pad_right(status_bar_, 20, 0);
    }


    #if IDLE_SCREEN_HOOK
    virtual void SetTheme(const std::string& theme_name) override;
    virtual void OnStateChanged() override;
    virtual void OnClockTimer() override;
    void IdleScrSetupUi();
    #endif // IDLE_SCREEN_HOOK

private:
    #if IDLE_SCREEN_HOOK
    IdleScreen _lcdScnIdle;
    #endif // IDLE_SCREEN_HOOK
};

#endif // _LCD_TOUCH_H
