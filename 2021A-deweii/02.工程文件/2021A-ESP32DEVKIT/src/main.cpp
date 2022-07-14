#include "_lcd_ili9341_.h"

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial BS;

#define test

void setup(void)
{
  Serial.begin(115200);
  Serial2.begin(115200);

  Screen_Init();
  //-------------------------//
  settimeofday(&_TV, NULL);
  //-------------------------//
#ifdef test
  BS.begin("devkit32_program_a");
  // Serial.println("---begin---");
#endif
  show_main_menu_start();
}

void loop()
{
  // again:
  touch.TP_Scan(0);
  if (touch.TP_Get_State() & TP_PRES_DOWN)
  {
    //按键检测在菜单栏
    if (touch.x < MENU_WIDTH + MENU_INTERVAL)
    {
      //
      if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT)
      {
        flag_menu = MENU_MAIN;
      }
      else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 2)
      {
        flag_menu = MENU_1;
      }
      else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 3)
      {
        flag_menu = MENU_2;
      }
      else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 4)
      {
        flag_menu = MENU_3;
      }
      else if (touch.y < MENU_BEGIN_Y + MENU_HEIGHT * 5)
      {
        flag_menu = MENU_4;
      }
      else
      {
        flag_menu = MENU_SET;
      }
      //
      if (flag_old_menu != flag_menu)
      {
        show_main_menu();
        flag_old_menu = flag_menu;

        if (flag_menu == MENU_MAIN)
        {
          show_deweii();
        }

        if (flag_menu == MENU_1) //car mode
        {
          show_menu_1();
        }

        if (flag_menu == MENU_2) //cam mode
        {
          show_menu_2();
        }

        if (flag_menu == MENU_3)
        {
          show_menu_3();
          flag_end_read = false;
        }

        if (flag_menu == MENU_4)
        {
          show_menu_4();
        }

        if (flag_menu == MENU_SET)
        {
          show_menu_set();
        }
      }
    }
    //
    else
    {
      //menu_1
      if (flag_menu == MENU_1)
      {
        //select
        if (touch.x > LCD_WIDTH - 2 * MENU_WIDTH)
        {
          //mode
          if (touch.y < MENU_HEIGHT)
          {
            show_menu_1_mode(0);
          }
          else if (touch.y < MENU_HEIGHT * 2)
          {
            if (flag_menu_1_mode1 >= 2)
            {
              flag_menu_1_mode1 = 0;
            }
            else
            {
              flag_menu_1_mode1++;
            }

            show_menu_1_mode(1);
          }
          else if (touch.y < MENU_HEIGHT * 3)
          {
            if (flag_menu_1_mode2 >= 9)
            {
              flag_menu_1_mode2 = 0;
            }
            else
            {
              flag_menu_1_mode2++;
            }
            show_menu_1_mode(2);
          }
          else if (touch.y < MENU_HEIGHT * 4)
          {

            if (flag_menu_1_mode3 >= 9)
            {
              flag_menu_1_mode3 = 0;
            }
            else
            {
              flag_menu_1_mode3++;
            }

            show_menu_1_mode(3);
          }
          else if (touch.y < MENU_HEIGHT * 5)
          {
          }
          else if (touch.y < MENU_HEIGHT * 6)
          {
          }
          else if (touch.y < MENU_HEIGHT * 7)
          {
          }
          else
          {
          }

          delay(200);
        }
      }
      //touch
      if (flag_menu == MENU_2)
      {
      }
      //设定参数 重要
      if (flag_menu == MENU_3)
      {
        // //上半部分 设置 下半部分 发送
        // if (touch.y > LCD_HEIGHT / 2) //刷新
        // {
        //   show_menu_3();
        // }
        // else //send
        // {
        //   if (touch.x > LCD_WIDTH / 2 + MENU_WIDTH / 2)
        //   {
        //     avoid_touch();
        //     Serial2.write(0xff);
        //     Serial2.write(current_freq);
        //     show_string("/", LCD_WIDTH - MENU_INTERVAL * 5, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);
        //   }
        //   else if (!flag_adc_send)
        //   {
        //     flag_adc_send = true;
        //     Serial2.write(0x00);
        //     Serial2.write(0x00);
        //     show_string("!", LCD_WIDTH - 3 * MENU_WIDTH, 2 * MENU_INTERVAL, 2, WHITE, BLACK, 1);
        //   }
        //   avoid_touch();
        //   //显示结果
        // }
      }
      //touch dac mode
      if (flag_menu == MENU_4)
      {
        if (touch.y < MENU_HEIGHT)
        {
        }
        else if (touch.y < MENU_HEIGHT * 2)
        {
          if (touch.x < LCD_WIDTH - 2 * MENU_WIDTH + MENU_INTERVAL / 2)
          {
            show_menu_4_mode(--flag_menu_4_mdoe1);
          }
          else
          {
            show_menu_4_mode(++flag_menu_4_mdoe1);
          }
        }
        else
        {
        }
      }

      if (flag_menu == MENU_SET)
      {
        //发送信息

        if (touch.y < LCD_WIDTH / 2 && !flag_send && current_freq >= 5)
        {
          Serial2.write(0xff);
          Serial2.write(current_freq);

          Serial2.write(0x00);
          Serial2.write(0x01); //默认 用原数据

          flag_send = true;
        }
        // if (touch.y > LCD_WIDTH / 2 && !flag_send_blue) //比较被动
        // {
        //   for (ii = 0; ii < pixel_width; ii++)
        //   {
        //     BS.println(pixel_pos[ii] * 100);
        //   }
        //   BS.println("---");
        //   flag_send_blue = true;
        // }
      }
    }

    // avoid_touch(); //bug? IRQ接错了
  }

  if (flag_menu == MENU_SET)
  {
    show_menu_set_mode(); //变化的部分
  }
  //主要显示界面的ADC
  if (flag_menu == MENU_3 && flag_end_read)
  {
    show_menu_3();
    flag_end_read = false;
  }

#ifdef test
  //数据接收部分 全部放在一个double 的数组里
  if (Serial2.available() && !flag_start_read)
  {
    String linedata;
    while (Serial2.available())
    {
      linedata = Serial2.readStringUntil('\n');
      if (linedata == START_SIGN)
      {
        flag_start_read = true;
        break;
      }
    }
  }
  if (Serial2.available() && flag_start_read)
  {
    for (ii = 0; ii < param_size; ii++)
    {
      param[ii] = Serial2.readStringUntil('\n').toDouble();
      // Serial.printf("param[%d]=%.3f\n", ii, param[ii]);
    }
    flag_start_read = false;
    flag_end_read = true;
    flag_send = false;

    for (ii = 0; ii < pixel_width; ii++)
    {
      pixel_pos[ii] = (param[a1] * cos(2 * PI / pixel_width * ii * pos_times) +
                       param[b1] * sin(2 * PI / pixel_width * ii * pos_times) +
                       param[a2] * cos(4 * PI / pixel_width * ii * pos_times) +
                       param[b2] * sin(4 * PI / pixel_width * ii * pos_times) +
                       param[a3] * cos(6 * PI / pixel_width * ii * pos_times) +
                       param[b3] * sin(6 * PI / pixel_width * ii * pos_times) +
                       param[a4] * cos(8 * PI / pixel_width * ii * pos_times) +
                       param[b4] * sin(8 * PI / pixel_width * ii * pos_times) +
                       param[a5] * cos(10 * PI / pixel_width * ii * pos_times) +
                       param[b5] * sin(10 * PI / pixel_width * ii * pos_times) +
                       param[a6] * cos(12 * PI / pixel_width * ii * pos_times) +
                       param[b6] * sin(12 * PI / pixel_width * ii * pos_times) +
                       param[a7] * cos(12 * PI / pixel_width * ii * pos_times) +
                       param[b7] * sin(12 * PI / pixel_width * ii * pos_times)) /
                      300000;

      // Serial.println(pixel_pos[ii]);
    }
    pixel_pos_max = pixel_pos[0]; //找出最大 以确定显示的系数
    for (ii = 1; ii < pixel_width; ii++)
    {
      if (pixel_pos[ii] > pixel_pos_max)
      {
        pixel_pos_max = pixel_pos[ii];
      }
    }
  }

  if (BS.available())
  {
    // char temp = BS.read();
    // if (temp == 0xff) //读数据
    // {
    //   for (ii = 0; ii < pixel_width; ii++)
    //   {
    //     BS.println(pixel_pos[ii] * 100);
    //   }
    //   BS.println();
    // }
    // if (temp == 0x00)
    // {
    //   current_freq = 10;
    // }
    // Serial.println(BS.read())

    while (BS.available())
    {
      if (BS.read() == 0xa5) //start
      {
        char cmd = BS.read();
        while (BS.available())
        {
          if (BS.read() == 0x5a)
          {
            break;
          }
        }
        if (cmd == 0x00)
        {
          for (ii = 0; ii < pixel_width; ii++)
          {
            char temp = (char)(pixel_pos[pixel_width - ii - 1] / pixel_pos_max * 100);
            BS.write(0xa5);
            BS.write(temp);
            BS.write(temp);
            BS.write(0x5a);
          }
        }
        if (cmd == 0x01)
        {
          BS.write(0xa5);
          BS.write((char)(param[U1] * 100));
          BS.write((char)(param[U2] * 100));
          BS.write((char)(param[U3] * 100));
          BS.write((char)(param[U4] * 100));
          BS.write((char)(param[U5] * 100));
          BS.write((char)(param[U6] * 100));
          BS.write((char)(param[U7] * 100));
          BS.write((char)(param[thd5]));
          BS.write((char)(param[thd6]));
          BS.write((char)(param[thd7]));
          BS.write((((char)param[thd7] + (char)param[thd6] + (char)param[thd5] +
                     (char)(param[U7] * 100) + (char)(param[U6] * 100) + (char)(param[U5] * 100) +
                     (char)(param[U4] * 100) + (char)(param[U3] * 100) + (char)(param[U2] * 100) + (char)(param[U1] * 100))) &
                   0xff);
          BS.write(0x5a);
        }
      }
    }
  }

#endif
}
