// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: PhysicsLab

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_AQI_Meter;
lv_obj_t * ui_ColorPanel1;
lv_obj_t * ui_IMAGE1;
lv_obj_t * ui_KNOB1;
lv_obj_t * ui_AQI_TEXT;
lv_obj_t * ui_AQI_VALUE;
lv_obj_t * ui_CO2_Meter;
lv_obj_t * ui_ColorPanel2;
lv_obj_t * ui_IMAGE2;
lv_obj_t * ui_KNOB2;
lv_obj_t * ui_CO2_TEXT;
lv_obj_t * ui_CO2_VALUE;
lv_obj_t * ui_TEMPERATURE_METER;
lv_obj_t * ui_ColorPanel3;
lv_obj_t * ui_IMAGE3;
lv_obj_t * ui_KNOB3;
lv_obj_t * ui_TEMPERATURE_TEXT;
lv_obj_t * ui_Temp_Number;
lv_obj_t * ui_LETTER_C;
lv_obj_t * ui_DOT;
lv_obj_t * ui_TEMPERATURE_VALUE;
lv_obj_t * ui_HUMIDITY_METER;
lv_obj_t * ui_ColorPanel4;
lv_obj_t * ui_IMAGE4;
lv_obj_t * ui_KNOB4;
lv_obj_t * ui_HUMIDITY_TEXT;
lv_obj_t * ui_HUMIDITY_VALUE;
lv_obj_t * ui_Label;
lv_obj_t * ui_Panel1;
lv_obj_t * ui_Label1;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
const lv_img_dsc_t * ui_imgset_part[2] = {&ui_img_part1_png, &ui_img_part2_png};
const lv_img_dsc_t * ui_imgset_test[3] = {&ui_img_test22_png, &ui_img_test3_png, &ui_img_test99_png};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}
