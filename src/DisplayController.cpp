#ifndef _DISPLAY_CONTROLLER_C_
#define _DISPLAY_CONTROLLER_C_
#include <Arduino.h>
#include <SSD1306Wire.h>
#include "Logger.cpp"
using namespace std;

// Define I2C pins (optional, default pins are often used)
#define OLED_SDA_PIN 21 // 5 - For Onboard LED kit
#define OLED_SCL_PIN 22 // 4 - For Onboard LED kit
// Define the I2C address of your SSD1306 display (default is 0x3c)
#define OLED_ADDRESS 0x3c

class DisplayController
{
public:
    // Constructor
    DisplayController() : display(OLED_ADDRESS, OLED_SDA_PIN, OLED_SCL_PIN)
    {
    }

    // Initialize the display here
    void initialize()
    {
        display.init();
        display.clear();
        display.flipScreenVertically();
        display.setFont(ArialMT_Plain_10);
        void showProgress(int progress1, String message1, int progress2, String message2);
        void showProgress2(int progress);
        display.display(); // Show the initial cleared display
    }

    // https://forums.adafruit.com/viewtopic.php?t=194853
    void show(String &message, OLEDDISPLAY_COLOR color = WHITE)
    {
        display.clear();

        display.setFont(ArialMT_Plain_16);
        display.setColor(color);
        int textWidth = display.getStringWidth(message);
        // Logger::logWarn("TXT:WIDTH=" + String(textWidth));
        int textHeight = 24;
        int centerX = (display.getWidth() / 2 - textWidth / 2) * 2;
        // int centerX = (display.getWidth() / 2 - textWidth) * 2;
        // Logger::logWarn("TXT:POS=" + String(centerX));
        int centerY = display.getHeight() / 2 - textHeight / 2;
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(centerX, centerY, message);

        // display.print(message);
        display.display();
    };

    void showInfo(String message)
    {
        show(message, WHITE);
    };

    void showWarn(String message)
    {
        show(message, WHITE);
    };

    void showError(String message)
    {
        show(message, WHITE);
    };

    void showDateTime(String &dateTime)
    {
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.setFont(ArialMT_Plain_10);
        display.drawString(60, 0, dateTime);
        display.display();
    };

    void showProgress(String &dateTime, int progress1, String message1, int progress2, String message2)
    {
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.setFont(ArialMT_Plain_10);
        display.drawString(64, 0, dateTime);
        display.drawProgressBar(0, 20, 120, 10, progress1);
        display.drawString(64, 10, message1);
        display.drawProgressBar(0, 40, 120, 10, progress2);
        display.drawString(64, 30, message2);
        display.drawString(64, 50, "ON::OFF::OFF::ON");
        display.display();
    };

    void showProgress(int progress)
    {
        display.clear();
        display.drawProgressBar(0, 32, 120, 10, progress);
        // draw the percentage as String
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.setFont(ArialMT_Plain_10);
        display.drawString(64, 15, String(progress) + "%");
        display.display();
    };

    void showDemo(String message)
    {
        for (int i = 1; i < 5; i++)
        {
            display.clear();
            switch (i)
            {
            case 1: // Rectangle
                // Draw a pixel at given position
                for (int r = 0; r < 10; r++)
                {
                    display.setPixel(r, r);
                    display.setPixel(10 - r, r);
                }
                display.drawRect(12, 12, 20, 20);
                display.fillRect(14, 14, 17, 17);
                display.drawHorizontalLine(0, 40, 20);
                display.drawVerticalLine(40, 0, 20);
                break;
            case 2: // Circle
                for (int c = 1; c < 8; c++)
                {
                    display.setColor(WHITE);
                    display.drawCircle(32, 32, c * 3);
                    if (c % 2 == 0)
                    {
                        display.setColor(BLACK);
                    }
                    display.fillCircle(96, 32, 32 - c * 3);
                }
                break;
            case 3: // Progress Bar
                for (int p = 1; p <= 5; p++)
                {
                    display.clear();
                    int progress = p * 20;
                    display.drawProgressBar(0, 32, 120, 10, progress);
                    // draw the percentage as String
                    display.setTextAlignment(TEXT_ALIGN_CENTER);
                    display.setFont(ArialMT_Plain_10);
                    display.drawString(64, 15, String(progress) + "%");
                    display.display();
                    delay(1000);
                }

                break;
                // case 4://Image
                // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
                // on how to create xbm files

                // display.drawXbm(display.width() + 34, display.height() + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
                // display.drawFastImage(display.width() + 34, display.height() + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
                // display.display();
                // delay(1000);
                // display.drawFastImage((display.width()  - LOGO_WIDTH ) / 2,
                //   (display.height() - LOGO_HEIGHT) / 2,
                //   logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1)
                // display.drawBitmap(
                //   (display.width()  - LOGO_WIDTH ) / 2,
                //   (display.height() - LOGO_HEIGHT) / 2,
                //   logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
                // display.display();
                // delay(1000);
                // break;
            default:
                // create more fonts at http://oleddisplay.squix.ch/
                display.setTextAlignment(TEXT_ALIGN_CENTER);
                display.setFont(ArialMT_Plain_10);   // Can change (Font type, style, font size)
                display.drawString(50, 0, "Hello "); // Specify the text to display (horizontal margin from left, margin from top, desired sentence).
                display.setTextAlignment(TEXT_ALIGN_LEFT);
                display.setFont(ArialMT_Plain_24);
                display.drawString(0, 18, message + String(i));
                display.setTextAlignment(TEXT_ALIGN_RIGHT);
                display.setFont(ArialMT_Plain_10);
                display.drawString(20, 50, "Water Level Monitor");
                break;
            }
            display.display();
            delay(3000);
        }
    };

    SSD1306Wire display;

private:
    // SSD1306Wire object
};
#endif