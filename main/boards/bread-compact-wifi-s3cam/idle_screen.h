/**
 * @file idle_screen.h
 * @author dotnfc (dotnfc@gmail.com)
 * @brief a simple class for idle screen of xiaozhi
 * @version 0.1
 * @date 2025-05-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include <lvgl.h>
#include "lcd_display.h"

#include "config.h"
#if IDLE_SCREEN_HOOK

class IdleScreen
{
public:
    IdleScreen();
    ~IdleScreen();

public:
    void ui_scnClock_screen_init(ThemeColors *p_current_theme);
    void ui_scnClock_screen_destroy(void);

    lv_anim_t * dots_Animation(lv_obj_t * TargetObject, int delay);
    lv_anim_t * robot_Animation(lv_obj_t * TargetObject, int delay);

    void ui_showScreen(bool showIt);
    void ui_update();
    void ui_updateTheme(ThemeColors *p_current_theme);
    
public:
    bool       ui_shown;        // ui shown or not (in xiaozhi ui)

    lv_obj_t * ui_scnClock;
    lv_obj_t * ui_ctnIdle;
    lv_obj_t * ui_grpDateTime;
    lv_obj_t * ui_lblDate;
    lv_obj_t * ui_lblTime;
    lv_obj_t * ui_lblDot;
    lv_obj_t * ui_lblWeek;
    lv_obj_t * ui_imgRobot;

    lv_anim_t * ui_anim[2];     // we have only two animations
};
#endif // IDLE_SCREEN_HOOK
