#include "config.h"
#include "application.h"
#include "idle_screen.h"
#include "ui_helpers.h"
#include <esp_log.h>

#if IDLE_SCREEN_HOOK

LV_FONT_DECLARE(font_puhui_20_4);
LV_FONT_DECLARE(ui_font_font48Seg);
LV_IMG_DECLARE(ui_img_xiaozhi_48_png);    // assets/xiaozhi_48.png


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"


static const char *TAG = "IdlScn";

IdleScreen::IdleScreen() {
    ui_anim[0] = NULL;
    ui_anim[1] = NULL;

    ui_scnClock = NULL;
    ui_ctnIdle = NULL;
    ui_grpDateTime = NULL;
    ui_lblDate = NULL;
    ui_lblTime = NULL;
    ui_lblDot = NULL;
    ui_lblWeek = NULL;
    ui_imgRobot = NULL;
    ui_shown = false;
}

IdleScreen::~IdleScreen() {
    ui_scnClock_screen_destroy();
}

void IdleScreen::ui_scnClock_screen_init(ThemeColors *p_current_theme) {
    auto screen = lv_screen_active();

    ESP_LOGD(TAG, "init");
    ui_scnClock = lv_obj_create(screen);
    lv_obj_remove_flag(ui_scnClock, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_size(ui_scnClock, LV_HOR_RES, LV_VER_RES);

    lv_obj_set_style_border_side(ui_scnClock, LV_BORDER_SIDE_FULL, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_ctnIdle = lv_obj_create(ui_scnClock);
    lv_obj_remove_style_all(ui_ctnIdle);
    lv_obj_set_width(ui_ctnIdle, LV_HOR_RES);
    lv_obj_set_height(ui_ctnIdle, LV_VER_RES);
    lv_obj_set_align(ui_ctnIdle, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_ctnIdle, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE));      /// Flags

    ui_grpDateTime = lv_obj_create(ui_ctnIdle);
    lv_obj_set_width(ui_grpDateTime, 132);
    lv_obj_set_height(ui_grpDateTime, 140);
    lv_obj_set_x(ui_grpDateTime, 0);
    lv_obj_set_y(ui_grpDateTime, -9);
    lv_obj_set_align(ui_grpDateTime, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_grpDateTime, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_grpDateTime, p_current_theme->background, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_grpDateTime, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_grpDateTime, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblDate = lv_label_create(ui_grpDateTime);
    lv_obj_set_width(ui_lblDate, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblDate, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblDate, 0);
    lv_obj_set_y(ui_lblDate, -6);
    lv_obj_set_align(ui_lblDate, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_lblDate, "2025/05/23");
    lv_obj_set_style_text_font(ui_lblDate, &font_puhui_20_4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_lblDate, p_current_theme->text, 0);

    ui_lblTime = lv_label_create(ui_grpDateTime);
    lv_obj_set_width(ui_lblTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblTime, "22  56");
    lv_obj_set_style_text_font(ui_lblTime, &ui_font_font48Seg, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_lblTime, p_current_theme->text, 0);

    ui_lblDot = lv_label_create(ui_grpDateTime);
    lv_obj_set_width(ui_lblDot, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblDot, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblDot, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblDot, ":");
    lv_obj_set_style_text_font(ui_lblDot, &ui_font_font48Seg, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_lblDot, p_current_theme->text, 0);

    ui_lblWeek = lv_label_create(ui_grpDateTime);
    lv_obj_set_width(ui_lblWeek, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblWeek, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblWeek, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(ui_lblWeek, "Monday");
    lv_obj_set_style_text_font(ui_lblWeek, &font_puhui_20_4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_lblWeek, p_current_theme->background, 0);

    ui_imgRobot = lv_image_create(ui_ctnIdle);
    lv_image_set_src(ui_imgRobot, &ui_img_xiaozhi_48_png);
    lv_obj_set_width(ui_imgRobot, LV_SIZE_CONTENT);   /// 48
    lv_obj_set_height(ui_imgRobot, LV_SIZE_CONTENT);    /// 48
    lv_obj_set_x(ui_imgRobot, 0);
    lv_obj_set_y(ui_imgRobot, 110);
    lv_obj_set_align(ui_imgRobot, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_imgRobot, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_remove_flag(ui_imgRobot, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_showScreen(false);
    ui_updateTheme(p_current_theme);

    ui_shown = true;
    ui_update(); // update date & time immediately
    ui_shown = false;
}

// theme changed
void IdleScreen::ui_updateTheme(ThemeColors *p_current_theme) {

    // Update the ui_scnClock colors
    lv_obj_set_style_bg_color(ui_scnClock, p_current_theme->background, 0);
    lv_obj_set_style_border_color(ui_scnClock, p_current_theme->border, 0);
    lv_obj_set_style_text_color(ui_scnClock, p_current_theme->text, 0);

    //lv_obj_set_style_bg_color(ui_ctnIdle, p_current_theme->background, 0);
    lv_obj_set_style_border_color(ui_ctnIdle, p_current_theme->border, 0);
    lv_obj_set_style_text_color(ui_ctnIdle, p_current_theme->text, 0);
    
    //lv_obj_set_style_bg_color(ui_grpDateTime, p_current_theme->background, 0);
    lv_obj_set_style_border_color(ui_grpDateTime, p_current_theme->border, 0);
    lv_obj_set_style_text_color(ui_grpDateTime, p_current_theme->text, 0);

    lv_obj_set_style_text_color(ui_lblDate, p_current_theme->text, 0);
    lv_obj_set_style_text_color(ui_lblTime, p_current_theme->text, 0);
    lv_obj_set_style_text_color(ui_lblDot, p_current_theme->text, 0);
    lv_obj_set_style_text_color(ui_lblWeek, p_current_theme->text, 0);
}

// update date & time
void IdleScreen::ui_update() {
    const char *weekDays[] = {"日", "一", "二", "三", "四", "五", "六"};

    time_t now = time(NULL);
    char time_str[32];
    struct tm *local_time = localtime(&now);

    if (!ui_shown) {
        return;
    }

    // "2025/05/23"
    strftime(time_str, sizeof(time_str), "%Y/%m/%d", local_time);
    lv_label_set_text(ui_lblDate, time_str);

    // "22  56"
    strftime(time_str, sizeof(time_str), "%H  %M", local_time);
    lv_label_set_text(ui_lblTime, time_str);

    // 0 表示星期天，1 表示星期一，依此类推
    snprintf(time_str, sizeof(time_str), "星期%s", weekDays[local_time->tm_wday]);
    lv_label_set_text(ui_lblWeek, time_str);
}

void IdleScreen::ui_scnClock_screen_destroy(void) {
    ui_shown = false;

    if(ui_scnClock) lv_obj_del(ui_scnClock);

    // NULL screen variables
    ui_scnClock = NULL;
    ui_ctnIdle = NULL;
    ui_grpDateTime = NULL;
    ui_lblDate = NULL;
    ui_lblTime = NULL;
    ui_lblDot = NULL;
    ui_lblWeek = NULL;
    ui_imgRobot = NULL;
}

void IdleScreen::ui_showScreen(bool showIt) {
    ui_shown = showIt;

    if(ui_scnClock == NULL) {
        ESP_LOGD(TAG, "null");
        return;
    }
    if (showIt) {
        ESP_LOGD(TAG, "-> show");
        if (lv_obj_has_flag(ui_scnClock, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(ui_scnClock, LV_OBJ_FLAG_HIDDEN);
        }
        ui_anim[0] = dots_Animation(ui_lblDot, 0);
        ui_anim[0]->var = ui_anim[0];

        ui_anim[1] = robot_Animation(ui_imgRobot, 0);
        ui_anim[1]->var = ui_anim[1];
    }
    else {
        ESP_LOGD(TAG, "-> hide");
        if (!lv_obj_has_flag(ui_scnClock, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_add_flag(ui_scnClock, LV_OBJ_FLAG_HIDDEN);
        }

        if (ui_anim[0]) {
            lv_anim_del(ui_anim[0], NULL);
            ui_anim[0] = NULL;
        }

        if (ui_anim[1]) {
            lv_anim_del(ui_anim[1], NULL);
            ui_anim[1] = NULL;
        }
    }
}

///////////////////// ANIMATIONS ////////////////////
lv_anim_t * IdleScreen::dots_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = (ui_anim_user_data_t *)lv_malloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 250);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 250);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 250);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 500);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    out_anim = lv_anim_start(&PropertyAnimation_0);

    return out_anim;
}

lv_anim_t * IdleScreen::robot_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t * out_anim;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = (ui_anim_user_data_t *)lv_malloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, 0, -10);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 1000);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y);
    out_anim = lv_anim_start(&PropertyAnimation_0);

    return out_anim;
}

#endif // IDLE_SCREEN_HOOK
