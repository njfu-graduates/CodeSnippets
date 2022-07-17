#include <LCDWIKI_GUI.h>
#include <LCDWIKI_SPI.h>
#include <LCDWIKI_TOUCH.h>
#include <Arduino.h>
#include <sys/time.h>

#define MODEL ILI9341
#define CS 15
#define CD 4
#define RST 2
#define MOSI 13
#define MISO 12
#define SCK 14
#define LED 25
//if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V
#define TCS 5
#define TCLK 18
#define TDOUT 19
#define TDIN 23
#define TIRQ 27

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
#define MENU_3 3
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
#define flag_menu_1_name "CAR MODE"
#define flag_menu_1_mode_num 3
#define flag_menu_1_mode1_num 3
#define flag_menu_1_mode1_name " MODE"
#define flag_menu_1_mode1_1 "XunJi"
#define flag_menu_1_mode1_2 "BlurT"
#define flag_menu_1_mode2_num 3
#define flag_menu_1_mode2_name " MODE2"
#define flag_menu_1_mode2_1 "X"
#define flag_menu_1_mode2_2 "B"
#define flag_menu_1_mode3_num 3
#define flag_menu_1_mode3_name " MODE3"
#define flag_menu_1_mode3_1 "T"
#define flag_menu_1_mode3_2 "B"
#define flag_menu_1_mode_off "OFF"
uint8_t flag_menu_1_mode1 = 0;
uint8_t flag_menu_1_mode2 = 0;
uint8_t flag_menu_1_mode3 = 0;
//-------------------------//
#define flag_menu_2_name "CAM MODE"
//-------------------------//
#define flag_menu_3_name "ADC MODE"
#define S_ADC_S 160 //adc的测量个数
uint16_t S_ADC1[S_ADC_S] = {0};
uint16_t S_ADC2[S_ADC_S] = {0};
uint16_t S_ADC3[S_ADC_S] = {0};
uint16_t S_ADC4[S_ADC_S] = {0};
#define S_ADC_BEGIN_X (2 * MENU_WIDTH + (LCD_WIDTH - MENU_WIDTH - S_ADC_S) / 2)
#define S_ADC_BEGIN_Y (MENU_HEIGHT * 2 - MENU_HEIGHT / 4 * 3)
#define S_ADC_BEGIN_Y_I (MENU_HEIGHT / 2 * 3)

#define ADC_DURATION 333 //ms
unsigned long last_ADC_time = 0;
bool flag_adc_rolling = true; //是否滚动出现
#define ADC1_COLOR YELLOW
#define ADC2_COLOR BLUE
#define ADC3_COLOR GREEN
#define ADC4_COLOR MAGENTA
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
//功能1(car mode) （只画右半面）(静态+动态)
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

    //画选中的色块
    lcd.Set_Draw_color(RED);
    for (uint8_t i = 0; i < flag_menu_1_mode_num; i++)
    {
        lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                           MENU_BEGIN_Y + MENU_HEIGHT * i + MENU_INTERVAL,
                           LCD_WIDTH - MENU_INTERVAL,
                           MENU_BEGIN_Y + MENU_HEIGHT * (i + 1) - MENU_INTERVAL);
    }
    for (uint8_t i = 1; i <= flag_menu_1_mode_num; i++)
    {
        show_menu_1_mode(i);
    }
}
//功能1(car mode) （只画动态）
void show_menu_1_mode(uint8_t mode)
{
    char *str = "";
    if (mode == 0)
    {
        flag_menu_1_mode1 = 0;
        flag_menu_1_mode2 = 0;
        flag_menu_1_mode3 = 0;

        for (uint8_t i = 1; i <= flag_menu_1_mode_num; i++)
        {
            show_menu_1_mode(i);
        }
        return;
    }
    else if (mode == 1)
    {
        switch (flag_menu_1_mode1)
        {
        case 1:
            str = flag_menu_1_mode1_1;
            break;

        case 2:
            str = flag_menu_1_mode1_2;
            break;

        default:
            str = flag_menu_1_mode_off;
            break;
        }
    }
    else if (mode == 2)
    {
        switch (flag_menu_1_mode2)
        {
        case 1:
            str = flag_menu_1_mode2_1;
            break;

        case 2:
            str = flag_menu_1_mode2_2;
            break;

        default:
            str = flag_menu_1_mode_off;
            break;
        }
    }
    else if (mode == 3)
    {
        switch (flag_menu_1_mode3)
        {
        case 1:
            str = flag_menu_1_mode3_1;
            break;

        case 2:
            str = flag_menu_1_mode3_2;
            break;

        default:
            str = flag_menu_1_mode_off;
            break;
        }
    }

    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                       MENU_BEGIN_Y + MENU_HEIGHT * (mode - 1) + MENU_INTERVAL,
                       LCD_WIDTH - MENU_INTERVAL,
                       MENU_BEGIN_Y + MENU_HEIGHT * mode - MENU_INTERVAL);
    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(LCD_WIDTH - 2 * MENU_WIDTH,
                       MENU_BEGIN_Y + MENU_HEIGHT * mode - MENU_INTERVAL + 1,
                       LCD_WIDTH - MENU_INTERVAL,
                       MENU_BEGIN_Y + MENU_HEIGHT * mode - 1);

    show_string(str, LCD_WIDTH - 2 * MENU_WIDTH + MENU_INTERVAL,
                MENU_BEGIN_Y + MENU_HEIGHT * (mode - 1) + 2 * MENU_INTERVAL,
                2, WHITE, BLACK, 1);
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
    lcd.Set_Draw_color(RED);
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_INTERVAL,
                       LCD_WIDTH / 2 - MENU_INTERVAL,
                       MENU_HEIGHT - MENU_INTERVAL);

    show_string(flag_menu_3_name, MENU_WIDTH + 2 * MENU_INTERVAL, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);

    //4路ADC
    sprintf(temp_string, "ADC1:D%d", ADC1);
    show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                MENU_HEIGHT * 2 - MENU_INTERVAL / 2, 1, WHITE, BLACK, 1);
    sprintf(temp_string, "ADC2:D%d", ADC2);
    show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                MENU_HEIGHT / 2 * 7 - MENU_INTERVAL / 2, 1, WHITE, BLACK, 1);
    sprintf(temp_string, "ADC3:D%d", ADC3);
    show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                MENU_HEIGHT * 5 - MENU_INTERVAL / 2, 1, WHITE, BLACK, 1);
    sprintf(temp_string, "ADC4:D%d", ADC4);
    show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                MENU_HEIGHT / 2 * 13 - MENU_INTERVAL / 2, 1, WHITE, BLACK, 1);
    //绘制记录的波形
    for (uint8_t i = 0; i < S_ADC_S; i++)
    {
        lcd.Set_Draw_color(ADC1_COLOR);
        lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                       S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I - (uint16_t)S_ADC1[(last_ADC_time / ADC_DURATION % S_ADC_S - i + S_ADC_S + 2) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
        lcd.Set_Draw_color(ADC2_COLOR);
        lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                       S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 2 - (uint16_t)S_ADC2[(last_ADC_time / ADC_DURATION % S_ADC_S - i + S_ADC_S + 2) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
        lcd.Set_Draw_color(ADC3_COLOR);
        lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                       S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 3 - (uint16_t)S_ADC3[(last_ADC_time / ADC_DURATION % S_ADC_S - i + S_ADC_S + 2) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
        lcd.Set_Draw_color(ADC4_COLOR);
        lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                       S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 4 - (uint16_t)S_ADC4[(last_ADC_time / ADC_DURATION % S_ADC_S - i + S_ADC_S + 2) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
    }
}
//      波形设置
void show_menu_3_mode(void)
{
    unsigned long current_time = millis();
    if (current_time - last_ADC_time > ADC_DURATION)
    {
        last_ADC_time = current_time;
        uint8_t current_num = last_ADC_time / ADC_DURATION % S_ADC_S;

        S_ADC1[current_num] = analogRead(ADC1);
        S_ADC2[current_num] = analogRead(ADC2);
        S_ADC3[current_num] = analogRead(ADC3);
        S_ADC4[current_num] = analogRead(ADC4);

        sprintf(temp_string, "   %.2f V", S_ADC1[current_num] * 3.3 / 4095);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                    MENU_HEIGHT * 2 + MENU_INTERVAL, 1, RED, BLACK, 0);
        sprintf(temp_string, "   %.2f V", S_ADC2[current_num] * 3.3 / 4095);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                    MENU_HEIGHT / 2 * 7 + MENU_INTERVAL, 1, RED, BLACK, 0);
        sprintf(temp_string, "   %.2f V", S_ADC3[current_num] * 3.3 / 4095);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                    MENU_HEIGHT * 5 + MENU_INTERVAL, 1, RED, BLACK, 0);
        sprintf(temp_string, "   %.2f V", S_ADC4[current_num] * 3.3 / 4095);
        show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                    MENU_HEIGHT / 2 * 13 + MENU_INTERVAL, 1, RED, BLACK, 0);

        if (flag_adc_rolling)
        {
            for (uint8_t i = 0; i < S_ADC_S; i++)
            {
                if (S_ADC1[(current_num - i + S_ADC_S) % S_ADC_S] != S_ADC1[(current_num - i + S_ADC_S - 1) % S_ADC_S])
                {
                    lcd.Set_Draw_color(BLACK);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I - (uint16_t)S_ADC1[(current_num - i + S_ADC_S - 1) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                    lcd.Set_Draw_color(ADC1_COLOR);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I - (uint16_t)S_ADC1[(current_num - i + S_ADC_S) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                }
                if (S_ADC2[(current_num - i + S_ADC_S) % S_ADC_S] != S_ADC2[(current_num - i + S_ADC_S - 1) % S_ADC_S])
                {
                    lcd.Set_Draw_color(BLACK);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 2 - (uint16_t)S_ADC2[(current_num - i + S_ADC_S - 1) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                    lcd.Set_Draw_color(ADC2_COLOR);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 2 - (uint16_t)S_ADC2[(current_num - i + S_ADC_S) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                }
                if (S_ADC3[(current_num - i + S_ADC_S) % S_ADC_S] != S_ADC3[(current_num - i + S_ADC_S - 1) % S_ADC_S])
                {
                    lcd.Set_Draw_color(BLACK);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 3 - (uint16_t)S_ADC3[(current_num - i + S_ADC_S - 1) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                    lcd.Set_Draw_color(ADC3_COLOR);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 3 - (uint16_t)S_ADC3[(current_num - i + S_ADC_S) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                }
                if (S_ADC4[(current_num - i + S_ADC_S) % S_ADC_S] != S_ADC4[(current_num - i + S_ADC_S - 1) % S_ADC_S])
                {
                    lcd.Set_Draw_color(BLACK);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 4 - (uint16_t)S_ADC4[(current_num - i + S_ADC_S - 1) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                    lcd.Set_Draw_color(ADC4_COLOR);
                    lcd.Draw_Pixel(S_ADC_BEGIN_X + S_ADC_S - i,
                                   S_ADC_BEGIN_Y + S_ADC_BEGIN_Y_I * 4 - (uint16_t)S_ADC4[(current_num - i + S_ADC_S) % S_ADC_S] * (S_ADC_BEGIN_Y_I - MENU_INTERVAL) / 4095);
                }
            }
        }
    }
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
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_HEIGHT / 2 + MENU_INTERVAL,
                       (LCD_WIDTH + MENU_WIDTH) / 2 - 2 * MENU_INTERVAL,
                       MENU_HEIGHT / 2 * 3 - MENU_INTERVAL);

    lcd.Set_Draw_color(BLACK);
    lcd.Fill_Rectangle(MENU_WIDTH + MENU_INTERVAL,
                       MENU_HEIGHT / 2 * 3 + MENU_INTERVAL + 1,
                       (LCD_WIDTH + MENU_WIDTH) / 2 - 2 * MENU_INTERVAL,
                       MENU_HEIGHT * 2 - 1);

    show_string(flag_menu_1_name, MENU_WIDTH + MENU_INTERVAL * 2,
                MENU_HEIGHT / 2 + MENU_INTERVAL * 2, 2, WHITE, BLACK, 1);
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

        for (uint8_t i = 0; i < flag_menu_1_mode1_num; i++)
        {
            sprintf(temp_string, "%s:%d", flag_menu_1_mode1_name, flag_menu_1_mode1);
            show_string(temp_string, MENU_WIDTH + MENU_INTERVAL * 2,
                        MENU_HEIGHT * 2 + MENU_INTERVAL * 2 * i, 1, WHITE, BLACK, 0);
        }
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

void Init()
{
    //-------------------------//
    settimeofday(&_TV, NULL);
    //-------------------------//
    adcAttachPin(ADC1);
    adcAttachPin(ADC2);
    adcAttachPin(ADC3);
    adcAttachPin(ADC4);
}

void Screen_Init(void)
{
    lcd.Init_LCD();
    lcd.Set_Rotation(LCD_ROTATION);
    touch.TP_Set_Rotation(LCD_ROTATION);
    touch.TP_Init(lcd.Get_Rotation(), lcd.Get_Display_Width(), lcd.Get_Display_Height());
    lcd.Fill_Screen(BLACK);
    pinMode(LED, OUTPUT);
    dacWrite(LED, 255 / 5 * flag_menu_4_mdoe1);
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
    delay(20);
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
    show_string("hello form deweii",
                (LCD_WIDTH + MENU_WIDTH) / 2 - 96,
                LCD_HEIGHT / 2 - 10,
                2, WHITE, RED, 1);
    show_string("Version -- v0.0.1",
                (LCD_WIDTH + MENU_WIDTH) / 2 - 96,
                LCD_HEIGHT / 2 + 10,
                2, WHITE, RED, 1);
}


