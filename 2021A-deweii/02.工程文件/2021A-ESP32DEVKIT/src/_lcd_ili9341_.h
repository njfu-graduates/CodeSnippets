#ifndef _LCD_H_
#define _LCD_H_

#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include <LCDWIKI_TOUCH.h>
#include <Arduino.h>
#include <sys/time.h>
#include "bt.h"
#include <math.h>

#define MODEL ILI9341
#define CS 15
#define CD 4
#define RST 2
#define MOSI 13
#define MISO 12
#define SCK 14
#define LED 25
// #define Offset 25
//if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V
#define TCS 5
#define TCLK 18
#define TDOUT 19
#define TDIN 23
#define TIRQ 21

#define ADC1 36
#define ADC2 39
#define ADC3 34
#define ADC4 35

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

LCDWIKI_SPI lcd(MODEL, CS, CD, MISO, MOSI, RST, SCK, -1);
LCDWIKI_TOUCH touch(TCS, TCLK, TDOUT, TDIN, TIRQ);

#define LCD_ROTATION 3
#define LCD_WIDTH lcd.Get_Display_Width()
#define LCD_HEIGHT lcd.Get_Display_Height()

#define MENU_MAIN 0
//car
#define MENU_1 1
#define MENU_2 2
#define MENU_3 3 //ADC
#define MENU_4 4
#define MENU_SET 5

#define MENU_NUM 6
#define MENU_INTERVAL 6
#define MENU_WIDTH lcd.Get_Display_Width() / 7
#define MENU_HEIGHT lcd.Get_Display_Height() / 8

#define MENU_FRONT_BEGIN 0
#define MENU_CENTER_BEGIN (LCD_HEIGHT - MENU_NUM * MENU_HEIGHT) / 2
#define MENU_BACK_BEGIN (LCD_HEIGHT - MENU_NUM * MENU_HEIGHT)
#define MENU_BEGIN_Y MENU_CENTER_BEGIN
#define MENU_BEGIN_X MENU_FRONT_BEGIN

uint8_t flag_menu = 0;     //当前菜单
uint8_t flag_old_menu = 0; //上一次菜单（用于界面变化）
//-------------------------//
#define flag_menu_1_name "SAMPLE"
#define flag_menu_1_mode_num 3
// #define flag_menu_1_mode1_num 3
#define flag_menu_1_mode1_name " 100"
// #define flag_menu_1_mode1_1 "XunJi"
// #define flag_menu_1_mode1_2 "BlurT"
// #define flag_menu_1_mode2_num 3
#define flag_menu_1_mode2_name " 10"
// #define flag_menu_1_mode2_1 "X"
// #define flag_menu_1_mode2_2 "B"
// #define flag_menu_1_mode3_num 3
#define flag_menu_1_mode3_name " 1"
// #define flag_menu_1_mode3_1 "T"
// #define flag_menu_1_mode3_2 "B"
// #define flag_menu_1_mode_off "OFF"
uint8_t flag_menu_1_mode = 0;
uint8_t flag_menu_1_mode1 = 0; //百位
uint8_t flag_menu_1_mode2 = 0; //十位
uint8_t flag_menu_1_mode3 = 1; //个位
//-------------------------//
#define flag_menu_2_name "CAM MODE"
//-------------------------//
#define flag_menu_3_name "ADC"
// #define S_ADC_S 160 //adc的测量个数
// uint16_t S_ADC1[S_ADC_S] = {0};
// uint16_t S_ADC2[S_ADC_S] = {0};
// uint16_t S_ADC3[S_ADC_S] = {0};
// uint16_t S_ADC4[S_ADC_S] = {0};
// #define S_ADC_BEGIN_X (2 * MENU_WIDTH + (LCD_WIDTH - MENU_WIDTH - S_ADC_S) / 2)
// #define S_ADC_BEGIN_Y (MENU_HEIGHT * 2 - MENU_HEIGHT / 4 * 3)
// #define S_ADC_BEGIN_Y_I (MENU_HEIGHT / 2 * 3)
// #define ADC_DURATION 333 //ms
// bool flag_adc_print = false;
// bool flag_adc_send = false;
// unsigned long last_ADC_time = 0;
// #define ADC1_COLOR YELLOW
// #define ADC2_COLOR BLUE
// #define ADC3_COLOR GREEN
// #define ADC4_COLOR MAGENTA

//-------------------------//
#define flag_menu_4_name "DAC MODE"
#define flag_menu_4_mode1_name "LED"
#define flag_menu_4_mode1_num 5
uint8_t flag_menu_4_mdoe1 = 5;
#define LED_WIDTH_INTERVAL (2 * MENU_WIDTH - MENU_INTERVAL / 2 * 3) / 5

struct timeval _TV = {0, 0};
int _time_second = 0;
char temp_time[48];
char temp_string[64];

#define pixel_width 262 //(LCD_WIDTH - MENU_WIDTH - 2 * MENU_INTERVAL)
float pixel_pos[pixel_width] = {0};
float pixel_pos_max = 150;
#define pos_times 2 //几个周期
// float THD;
// double ADC_last[pixel_width] = {0};
// double ADC_current[pixel_width] = {0};
// double Um[5] = {0};

uint16_t ii;
#define START_SIGN "==="
bool flag_start_read = false; //用于串口的判断
bool flag_end_read = false;   //用于读取数组里更新的数用于显示
bool flag_send = false;
bool flag_send_blue = false;
uint16_t current_freq = 10;
#define param_size 25
double param[param_size] = {0};
enum
{
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7,
    b1,
    b2,
    b3,
    b4,
    b5,
    b6,
    b7,
    U1,
    U2,
    U3,
    U4,
    U5,
    U6,
    U7,
    thd5,
    thd6,
    thd7,
    freq
};

void Init(void);
void show_string(char *str, int16_t x, int16_t y, uint8_t csize, uint16_t fc, uint16_t bc, boolean mode);
void Screen_Init(void);
void show_main_menu_start(void);
void show_main_menu(void);
void show_deweii(void);
void show_menu_1(void);
void show_menu_1_mode(uint8_t mode);
void show_menu_2(void);
void show_menu_3(void);
void show_menu_3_mode(void);
void show_menu_4(void);
void show_menu_4_mode(uint8_t mode);
void show_menu_set(void);
void show_menu_set_mode(void);
void avoid_touch(void);
void show_deweii(void);

void draw_black_right()
{
    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_INTERVAL,
                       LCD_WIDTH + MENU_INTERVAL,
                       LCD_HEIGHT - MENU_INTERVAL);
}
//只画屏幕的左半边
void show_main_menu(void)
{ //删除之前的选中图形
    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(MENU_BEGIN_X + MENU_INTERVAL + 1,
                       MENU_BEGIN_Y + MENU_INTERVAL + MENU_HEIGHT * flag_old_menu + 1,
                       MENU_BEGIN_X + MENU_WIDTH - MENU_INTERVAL - 1,
                       MENU_BEGIN_Y + MENU_HEIGHT * (flag_old_menu + 1) - MENU_INTERVAL - 1);
    //绘制新的选中菜单
    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(MENU_BEGIN_X + MENU_INTERVAL + 1,
                       MENU_BEGIN_Y + MENU_INTERVAL + MENU_HEIGHT * flag_menu + 1,
                       MENU_BEGIN_X + MENU_WIDTH - MENU_INTERVAL - 1,
                       MENU_BEGIN_Y + MENU_HEIGHT * (flag_menu + 1) - MENU_INTERVAL - 1);
    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_INTERVAL,
                       LCD_WIDTH + MENU_INTERVAL,
                       LCD_HEIGHT - MENU_INTERVAL);
}
//功能1 （只画右半面）(静态+动态)
void show_menu_1(void)
{
    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_INTERVAL,
                       LCD_WIDTH / 2 - MENU_INTERVAL,
                       MENU_HEIGHT - MENU_INTERVAL);

    show_string(flag_menu_1_name, MENU_WIDTH + 2 * MENU_INTERVAL, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);

    //画出菜单内容的分割线
    lcd.Set_Draw_color(RED);
    for (uint8_t i = 0; i <= MENU_NUM; i++)
    {
        lcd.Draw_Line(MENU_WIDTH + MENU_INTERVAL,
                      MENU_BEGIN_Y + MENU_HEIGHT * i,
                      LCD_WIDTH - MENU_INTERVAL,
                      MENU_BEGIN_Y + MENU_HEIGHT * i);
    }

    show_string(flag_menu_1_mode1_name, MENU_WIDTH + 2 * MENU_INTERVAL, MENU_BEGIN_Y + MENU_HEIGHT * 0 + MENU_INTERVAL * 2, 2, WHITE, BLACK, 1);
    show_string(flag_menu_1_mode2_name, MENU_WIDTH + 2 * MENU_INTERVAL, MENU_BEGIN_Y + MENU_HEIGHT * 1 + MENU_INTERVAL * 2, 2, WHITE, BLACK, 1);
    show_string(flag_menu_1_mode3_name, MENU_WIDTH + 2 * MENU_INTERVAL, MENU_BEGIN_Y + MENU_HEIGHT * 2 + MENU_INTERVAL * 2, 2, WHITE, BLACK, 1);
    show_string(" Current Freq", MENU_WIDTH + 2 * MENU_INTERVAL, MENU_BEGIN_Y + MENU_HEIGHT * 3 + MENU_INTERVAL * 2, 2, WHITE, BLACK, 1);
    //画选中的色块
    lcd.Set_Draw_color(RED);
    for (uint8_t i = 0; i < flag_menu_1_mode_num; i++)
    {
        lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                           MENU_BEGIN_Y + MENU_HEIGHT * i + MENU_INTERVAL,
                           LCD_WIDTH - MENU_INTERVAL,
                           MENU_BEGIN_Y + MENU_HEIGHT * (i + 1) - MENU_INTERVAL);
    }

    current_freq = 100 * flag_menu_1_mode1 + 10 * flag_menu_1_mode2 + flag_menu_1_mode3;
    sprintf(temp_string, "%d", current_freq);
    show_string(temp_string, LCD_WIDTH - 2 * MENU_WIDTH,
                MENU_BEGIN_Y + MENU_HEIGHT * 3 + MENU_INTERVAL, 1, WHITE, BLACK, 1);

    if (flag_menu_1_mode)
        show_menu_1_mode(flag_menu_1_mode);
}
//功能1  (SAMPLE mode) （只画动态）
void show_menu_1_mode(uint8_t mode)
{
    if (flag_menu_1_mode == 0) //不选择功能
    {
        flag_menu_1_mode = mode;
        return;
    }

    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                       MENU_BEGIN_Y + MENU_HEIGHT * (flag_menu_1_mode - 1) + MENU_INTERVAL,
                       LCD_WIDTH - MENU_INTERVAL,
                       MENU_BEGIN_Y + MENU_HEIGHT * flag_menu_1_mode - MENU_INTERVAL);
    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                       MENU_BEGIN_Y + MENU_HEIGHT * flag_menu_1_mode - MENU_INTERVAL + 1,
                       LCD_WIDTH - MENU_INTERVAL,
                       MENU_BEGIN_Y + MENU_HEIGHT * flag_menu_1_mode - 1);

    flag_menu_1_mode = mode;

    lcd.Set_Draw_color(WHITE);
    lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                       MENU_BEGIN_Y + MENU_HEIGHT * (mode - 1) + 2 * MENU_INTERVAL,
                       LCD_WIDTH - MENU_INTERVAL,
                       MENU_BEGIN_Y + MENU_HEIGHT * mode - 2 * MENU_INTERVAL);

    current_freq = 100 * flag_menu_1_mode1 + 10 * flag_menu_1_mode2 + flag_menu_1_mode3;
    sprintf(temp_string, "%d", current_freq);

    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                       MENU_BEGIN_Y + MENU_HEIGHT * 3 + MENU_INTERVAL,
                       LCD_WIDTH - MENU_INTERVAL,
                       MENU_BEGIN_Y + MENU_HEIGHT * 4 - MENU_INTERVAL);
    show_string(temp_string, LCD_WIDTH - 2 * MENU_WIDTH,
                MENU_BEGIN_Y + MENU_HEIGHT * 3 + MENU_INTERVAL, 1, WHITE, BLACK, 1);
}
//      cam mode
void show_menu_2(void)
{
    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_INTERVAL,
                       LCD_WIDTH / 2 - MENU_INTERVAL,
                       MENU_HEIGHT - MENU_INTERVAL);

    show_string(flag_menu_2_name, MENU_WIDTH + 2 * MENU_INTERVAL, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);
}
//      adc mode
void show_menu_3(void)
{
    draw_black_right();

    lcd.Set_Draw_color(RED);
    // lcd.Fill_Rectangle(
    //     MENU_WIDTH + MENU_INTERVAL,
    //     MENU_INTERVAL,
    //     MENU_WIDTH + MENU_INTERVAL + MENU_WIDTH,
    //     MENU_HEIGHT - MENU_INTERVAL);
    // show_string(flag_menu_3_name, MENU_WIDTH + 2 * MENU_INTERVAL, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);

    lcd.Draw_Line((MENU_WIDTH + MENU_INTERVAL) * 2,
                  MENU_INTERVAL,
                  (MENU_WIDTH + MENU_INTERVAL) * 2,
                  LCD_HEIGHT - MENU_INTERVAL);

    lcd.Draw_Line((MENU_WIDTH + MENU_INTERVAL) * 2 + pixel_width / 2,
                  MENU_INTERVAL,
                  (MENU_WIDTH + MENU_INTERVAL) * 2 + pixel_width / 2,
                  LCD_HEIGHT - MENU_INTERVAL);

    // lcd.Fill_Rectangle(
    //     LCD_WIDTH - 2 * MENU_WIDTH,
    //     MENU_INTERVAL,
    //     LCD_WIDTH - 1 * MENU_WIDTH,
    //     MENU_HEIGHT - MENU_INTERVAL);
    // show_string("Set", LCD_WIDTH - 2 * MENU_WIDTH, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);
    // lcd.Fill_Rectangle(
    //     LCD_WIDTH - 4 * MENU_WIDTH,
    //     MENU_INTERVAL,
    //     LCD_WIDTH - 3 * MENU_WIDTH,
    //     MENU_HEIGHT - MENU_INTERVAL);
    // show_string("Send", LCD_WIDTH - 4 * MENU_WIDTH, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);
    // lcd.Set_Draw_color(BLACK);
    // lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL * 2,
    //                    MENU_HEIGHT * 2 + MENU_INTERVAL,
    //                    LCD_WIDTH - MENU_INTERVAL,
    //                    LCD_HEIGHT - MENU_INTERVAL);

    //画波形部分
    //左上角  MENU_WIDTH + MENU_INTERVAL * 2  MENU_HEIGHT * 2 + MENU_INTERVAL
    //右下角  MENU_WIDTH + MENU_INTERVAL * 2+pixel_width  MENU_HEIGHT * 2 + MENU_INTERVAL+pixel_width
    lcd.Set_Draw_color(WHITE);
    for (ii = 0; ii < pixel_width; ii++)
    {
        lcd.Draw_Pixel(MENU_WIDTH + MENU_INTERVAL + ii,
                       MENU_HEIGHT + MENU_INTERVAL + pixel_width / 3 -
                           pixel_width / 4 * pixel_pos[ii] / pixel_pos_max);
    }

    // sprintf(temp_string, "%s", res1);
    // show_string("U1=1.000", MENU_WIDTH + MENU_INTERVAL * 4 + pixel_width,
    //             MENU_HEIGHT * 2 + MENU_INTERVAL * 1, 1, WHITE, BLACK, 0);
    // sprintf(temp_string, "%s", res2);
    // show_string("U2=0.000", MENU_WIDTH + MENU_INTERVAL * 4 + pixel_width,
    //             MENU_HEIGHT * 2 + MENU_INTERVAL * 3, 1, WHITE, BLACK, 0);
    // sprintf(temp_string, "%s", res3);
    // show_string("U3=0.501", MENU_WIDTH + MENU_INTERVAL * 4 + pixel_width,
    //             MENU_HEIGHT * 2 + MENU_INTERVAL * 5, 1, WHITE, BLACK, 0);
    // sprintf(temp_string, "%s", res4);
    // show_string("U4=0.000", MENU_WIDTH + MENU_INTERVAL * 4 + pixel_width,
    //             MENU_HEIGHT * 2 + MENU_INTERVAL * 7, 1, WHITE, BLACK, 0);
    // sprintf(temp_string, "%s", res5);
    // show_string("U5=0.000", MENU_WIDTH + MENU_INTERVAL * 4 + pixel_width,
    //             MENU_HEIGHT * 2 + MENU_INTERVAL * 9, 1, WHITE, BLACK, 0);
    // sprintf(temp_string, "%s", res6);
    // show_string("THD=49.890%", MENU_WIDTH + MENU_INTERVAL * 4 + pixel_width,
    //             MENU_HEIGHT * 2 + MENU_INTERVAL * 11, 1, WHITE, BLACK, 0);
}
//      波形设置
//   用于显示最终结果

//这个界面主要来显示我们的结果
void show_menu_3_mode(void)
{
}

//      dac mode
void show_menu_4(void)
{
    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_INTERVAL,
                       LCD_WIDTH / 2 - MENU_INTERVAL * 0,
                       MENU_HEIGHT - MENU_INTERVAL);

    show_string(flag_menu_4_name, MENU_WIDTH + 2 * MENU_INTERVAL, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);

    show_string(flag_menu_4_mode1_name, MENU_WIDTH + 2 * MENU_INTERVAL, MENU_BEGIN_Y + MENU_HEIGHT * 0 + MENU_INTERVAL * 2, 2, WHITE, BLACK, 1);
    show_string("<        >", LCD_WIDTH - 3 * MENU_WIDTH - 17, MENU_BEGIN_Y + MENU_HEIGHT * 0 + MENU_INTERVAL * 2, 2, RED, BLACK, 1);

    //LED _
    lcd.Set_Draw_color(RED);
    lcd.Draw_Rectangle(LCD_WIDTH - 3 * MENU_WIDTH,
                       MENU_BEGIN_Y + MENU_HEIGHT * 0 + MENU_INTERVAL,
                       LCD_WIDTH - 1 * MENU_WIDTH - MENU_INTERVAL,
                       MENU_BEGIN_Y + MENU_HEIGHT * 1 - MENU_INTERVAL * 0);

    show_menu_4_mode(flag_menu_4_mdoe1);
    //
}

void show_menu_4_mode(uint8_t mode)
{
    if (mode > 5)
        flag_menu_4_mdoe1 = 5;
    else if (mode < 1)
        flag_menu_4_mdoe1 = 1;

    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(LCD_WIDTH - 3 * MENU_WIDTH + MENU_INTERVAL / 2,
                       MENU_BEGIN_Y + MENU_HEIGHT * 0 + MENU_INTERVAL + MENU_INTERVAL / 2,
                       LCD_WIDTH - 3 * MENU_WIDTH + MENU_INTERVAL / 2 + LED_WIDTH_INTERVAL * flag_menu_4_mdoe1,
                       MENU_BEGIN_Y + MENU_HEIGHT * 1 - MENU_INTERVAL / 2);
    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(LCD_WIDTH - 3 * MENU_WIDTH + MENU_INTERVAL / 2 + LED_WIDTH_INTERVAL * flag_menu_4_mdoe1 + 1,
                       MENU_BEGIN_Y + MENU_HEIGHT * 0 + MENU_INTERVAL + MENU_INTERVAL / 2,
                       LCD_WIDTH - 1 * MENU_WIDTH - MENU_INTERVAL - MENU_INTERVAL / 2 + 1,
                       MENU_BEGIN_Y + MENU_HEIGHT * 1 - MENU_INTERVAL / 2);

    dacWrite(LED, 255 / 5 * flag_menu_4_mdoe1);
}
//观察界面 -时间 -数据
void show_menu_set(void)
{
    lcd.Set_Draw_color(RED);
    lcd.Draw_Line(MENU_WIDTH + MENU_INTERVAL, MENU_HEIGHT / 2,
                  LCD_WIDTH - MENU_INTERVAL, MENU_HEIGHT / 2);
    lcd.Draw_Line((LCD_WIDTH + MENU_WIDTH) / 2, MENU_HEIGHT / 2 + MENU_INTERVAL,
                  (LCD_WIDTH + MENU_WIDTH) / 2, LCD_HEIGHT - MENU_INTERVAL);

    // lcd.Set_Draw_color(RED);
    // lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
    //                    MENU_HEIGHT / 2 + MENU_INTERVAL,
    //                    (LCD_WIDTH + MENU_WIDTH) / 2 - 2 * MENU_INTERVAL,
    //                    MENU_HEIGHT / 2 * 3 - MENU_INTERVAL);
    // lcd.Set_Draw_color(BLACK);
    // lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
    //                    MENU_HEIGHT / 2 * 3 + MENU_INTERVAL + 1,
    //                    (LCD_WIDTH + MENU_WIDTH) / 2 - 2 * MENU_INTERVAL,
    //                    MENU_HEIGHT * 2 - 1);
}

void show_menu_set_mode(void)
{
    gettimeofday(&_TV, NULL);
    struct tm *TP = localtime(&_TV.tv_sec);
    if (TP->tm_sec != _time_second)
    {
        _time_second = TP->tm_sec;
        strftime(temp_time, sizeof(temp_time), "Time:%T", TP);
        show_string(temp_time, LCD_WIDTH - MENU_WIDTH * 2, MENU_INTERVAL, 1, WHITE, BLACK, 0);

        sprintf(temp_string, "freq = %.fkhz", param[freq]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 4, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "U1   = %.3f", param[U1]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 6, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "U2   = %.3f", param[U2]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 8, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "U3   = %.3f", param[U3]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 10, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "U4   = %.3f", param[U4]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 12, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "U5   = %.3f", param[U5]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 14, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "U6   = %.3f", param[U6]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 16, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "U7   = %.3f", param[U7]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 18, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "THD5 = %.3f%", param[thd5]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 20, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "THD6 = %.3f%", param[thd6]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 22, 1, WHITE, BLACK, 0);
        sprintf(temp_string, "THD7 = %.3f%", param[thd7]);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 24, 1, WHITE, BLACK, 0);
        //第二栏
        sprintf(temp_string, "freq_to_set:%dkhz", current_freq);
        show_string(temp_string, (LCD_WIDTH + MENU_WIDTH) / 2 + MENU_INTERVAL * 3,
                    MENU_HEIGHT / 2 + MENU_INTERVAL * 4, 1, WHITE, BLACK, 0);

        sprintf(temp_string, "-=SEND=-");
        if (!flag_start_read && !flag_send)
            show_string(temp_string, (LCD_WIDTH + MENU_WIDTH) / 2 + MENU_INTERVAL * 3,
                        MENU_HEIGHT / 2 + MENU_INTERVAL * 14, 2, BLACK, RED, 0);
        else
            show_string(temp_string, (LCD_WIDTH + MENU_WIDTH) / 2 + MENU_INTERVAL * 3,
                        MENU_HEIGHT / 2 + MENU_INTERVAL * 14, 2, RED, WHITE, 0);

        // sprintf(temp_string, "-=SEND=-");
        // if (!flag_start_read && !flag_send_blue)
        //     show_string(temp_string, (LCD_WIDTH + MENU_WIDTH) / 2 + MENU_INTERVAL * 3,
        //                 MENU_HEIGHT / 2 + MENU_INTERVAL * 24, 2, BLUE, RED, 0);
        // else
        //     show_string(temp_string, (LCD_WIDTH + MENU_WIDTH) / 2 + MENU_INTERVAL * 3,
        //                 MENU_HEIGHT / 2 + MENU_INTERVAL * 24, 2, RED, WHITE, 0);

        //这个函数一秒执行一
    }
}
//最开始的界面
void show_main_menu_start(void)
{
    lcd.Fill_Screen(BLACK);

    //逐一绘制新的菜单
    lcd.Set_Draw_color(RED);
    for (uint8_t i = 0; i < MENU_NUM; i++)
    {
        lcd.Draw_Rectangle(MENU_BEGIN_X + MENU_INTERVAL,
                           MENU_BEGIN_Y + MENU_INTERVAL + MENU_HEIGHT * i,
                           MENU_BEGIN_X + MENU_WIDTH - MENU_INTERVAL,
                           MENU_BEGIN_Y + MENU_HEIGHT * (i + 1) - MENU_INTERVAL);
    }
    lcd.Fill_Rectangle(MENU_BEGIN_X + MENU_INTERVAL + 1,
                       MENU_BEGIN_Y + MENU_INTERVAL + MENU_HEIGHT * flag_menu + 1,
                       MENU_BEGIN_X + MENU_WIDTH - MENU_INTERVAL - 1,
                       MENU_BEGIN_Y + MENU_HEIGHT * (flag_menu + 1) - MENU_INTERVAL - 1);
    lcd.Draw_Line(MENU_WIDTH, MENU_INTERVAL, MENU_WIDTH, LCD_HEIGHT - MENU_INTERVAL);

    //开始界面的绘制
    show_deweii();

    //做一些参数的归零
}

void Screen_Init(void)
{
    lcd.Init_LCD();
    lcd.Set_Rotation(LCD_ROTATION);
    touch.TP_Set_Rotation(LCD_ROTATION);
    touch.TP_Init(lcd.Get_Rotation(), lcd.Get_Display_Width(), lcd.Get_Display_Height());
    lcd.Fill_Screen(BLACK);
    pinMode(LED, OUTPUT);
    // pinMode(Offset, OUTPUT);

    dacWrite(LED, 255 / 5 * flag_menu_4_mdoe1);
    // dacWrite(Offset, 118);//1.5v
}

void show_string(char *str, int16_t x, int16_t y, uint8_t csize, uint16_t fc, uint16_t bc, boolean mode)
{
    lcd.Set_Text_Mode(mode);
    lcd.Set_Text_Size(csize);
    lcd.Set_Text_colour(fc);
    lcd.Set_Text_Back_colour(bc);
    lcd.Print_String(str, x, y);
}
//防误触
void avoid_touch(void)
{
    delay(50);
    while (touch.TP_Get_State() & TP_PRES_DOWN)
        touch.TP_Scan(0);
}
//右半边
void show_deweii(void)
{
    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle((LCD_WIDTH + MENU_WIDTH) / 2 - 96 - MENU_INTERVAL,
                       LCD_HEIGHT / 2 - 10 - MENU_INTERVAL,
                       (LCD_WIDTH + MENU_WIDTH) / 2 - 96 - MENU_INTERVAL + 208,
                       LCD_HEIGHT / 2 - 10 - MENU_INTERVAL + 38);
    show_string("PROGRAM A",
                (LCD_WIDTH + MENU_WIDTH) / 2 - 96,
                LCD_HEIGHT / 2 - 10,
                2, WHITE, RED, 1);
    show_string("Version -- v0.0.1",
                (LCD_WIDTH + MENU_WIDTH) / 2 - 96,
                LCD_HEIGHT / 2 + 10,
                2, WHITE, RED, 1);
}

#endif
