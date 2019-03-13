/**
 *  @filename   :   epd2in9-demo.ino
 *  @brief      :   2.9inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include <SPI.h>
#include <epd2in9.h>
#include <epdpaint.h>
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

// 2.9" epaper display: 296x128

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */

//unsigned char image[1024];
unsigned char image[EPD_HEIGHT*EPD_WIDTH];
Paint paint(image, EPD_WIDTH, EPD_HEIGHT);    // width should be the multiple of 8
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;

void drawImages()
{

  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  paint.SetWidth(EPD_WIDTH);
  paint.SetHeight(EPD_HEIGHT);

  paint.SetRotate(ROTATE_90);


  /* For simplicity, the arguments are explicit numerical coordinates */
  paint.Clear(UNCOLORED);
  paint.DrawStringAt((EPD_HEIGHT/2-11*5)/2, 8, "Temperature", &Font8, COLORED);
  paint.DrawStringAt( (EPD_HEIGHT/2) + (EPD_HEIGHT/2-8*5)/2, 8, "Humidity", &Font8, COLORED);

  paint.DrawStringAt( (EPD_HEIGHT/2) + (EPD_HEIGHT/2-8*5)/2, EPD_WIDTH/2+8, "Pressure", &Font8, COLORED);
  paint.DrawStringAt( (EPD_HEIGHT/2-5*5)/2, EPD_WIDTH/2+8, "Light", &Font8, COLORED);

  paint.DrawHorizontalLine(0, EPD_WIDTH/2, EPD_HEIGHT, COLORED);
  paint.DrawVerticalLine(EPD_HEIGHT/2,0, EPD_WIDTH, COLORED);

  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();

  delay(500);

  if (epd.Init(lut_partial_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
   /*
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  */

  time_start_ms = millis();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  drawImages();
  //drawImages();

}

void loop() {
  // put your main code here, to run repeatedly:
  time_now_s = (millis() - time_start_ms) / 1000;
  char time_string[] = {'0', '0', ':', '0', '0', '\0'};
  time_string[0] = time_now_s / 60 / 10 + '0';
  time_string[1] = time_now_s / 60 % 10 + '0';
  time_string[3] = time_now_s % 60 / 10 + '0';
  time_string[4] = time_now_s % 60 % 10 + '0';

  paint.SetWidth(18);
  paint.SetHeight(56);//5*11=55,next multiple of 8 is 56
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 2, time_string, &Font16, COLORED);


  //Temperature
  epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH-paint.GetWidth()-16-8,  (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());

  //Humidity
  epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH-paint.GetWidth()-16-8, EPD_HEIGHT/2 + (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());

  //Pressure
  epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH/2-paint.GetWidth()-16-8, EPD_HEIGHT/2 + (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());

  //Light
  epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH/2-paint.GetWidth()-16-8,  (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());


  epd.DisplayFrame();



    //Temperature
    epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH-paint.GetWidth()-16-8,  (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());

    //Humidity
    epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH-paint.GetWidth()-16-8, EPD_HEIGHT/2 + (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());

    //Pressure
    epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH/2-paint.GetWidth()-16-8, EPD_HEIGHT/2 + (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());

    //Light
    epd.SetFrameMemory(paint.GetImage(), EPD_WIDTH/2-paint.GetWidth()-16-8,  (EPD_HEIGHT/2-5*12)/2, paint.GetWidth(), paint.GetHeight());



  epd.DisplayFrame();

  delay(500);
  Serial.print(".");
}
