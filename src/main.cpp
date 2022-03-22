// rf22_reliable_datagram_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging client
// with the RHReliableDatagram class, using the RH_RF22 driver to control a RF22 radio.
// It is designed to work with the other example rf22_reliable_datagram_server
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

// #include <RHReliableDatagram.h>
#include <RH_RF22.h>
#include <SPI.h>
#include <RHSoftwareSPI.h>

// #define CLIENT_ADDRESS 0x21
// #define SERVER_ADDRESS 5

// Singleton instance of the radio driver
// RH_RF22 driver;
RHSoftwareSPI spi;
// ss_pin nSEL GPIO15
// irq_pin nIRQ GPIO5
RH_RF22 driver(22, 23, spi);

// Class to manage message delivery and receipt, using the driver declared above
// RHReliableDatagram manager(driver, CLIENT_ADDRESS);

//{D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,
// D12,D13,D14,D15,D16,D17,D18,D19,D20,D21,D22}

// uint8_t cmd2[] = {0x11, 0x0E, 0x10, 0xE0, 0x50, 0x00, 0x00, 0xAD, 0x00, 0x03, 0x02, 0x54, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x41, 0x00, 0x39, 0x00, 0x40, 0x01};
// uint8_t msg1[] = {0x11, 0x0E, 0x10, 0xE0, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x50,
// 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x40, 0x3B };
// uint8_t msg2[] = {0x11, 0x0E, 0x10, 0xE0, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x50,
// 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x55, 0x3B };
// uint8_t msg3[] = {0x11, 0x0E, 0x10, 0xE0, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x50,
// 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x6A, 0x3B };
// uint8_t msg4[] = {0x11, 0x0E, 0x10, 0xE0, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x50,
// 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x7F, 0x3B };
uint8_t cmd1[] = {0x11, 0x04, 0x03, 0xe9};
//  uint8_t cmd1[] = {0x11, 0x0E, 0x10, 0xE0, 0x50, 0x00, 0x00, 0xAD, 0x00,
// 0x03, 0x02, 0x54, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x41, 0x00, 0x39, 0x00, 0x40, 0x01};

uint8_t cmd2[] = {0x11, 0x04, 0x04, 0xe9};
//uint8_t data[] = {0x11, 0x08, 0x50, 0x50, 0x00, 0x00, 0xAD, 0xFF};
// uint8_t data[] = {0x11, 0x04, 0x01, 0x4F};

// Dont put this on the stack:
uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];

int counter = 0;

void sendiaqdata(void *pvParam);

void setup()
{
      Serial.begin(115200);
      Serial.println("set pin");
      spi.setPins(12, 13, 14);
      Serial.println("driver init");
      if (!driver.init())
            Serial.println("init failed");
      // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
      xTaskCreate(sendiaqdata, "sendiaqdata", 2048, NULL, 5, NULL);
      Serial.println("setup done");
}

void sendiaqdata(void *pvParam)
{
uint8_t data[] = {0x11, 0x08, 0x50, 0x50, 0x00, 0x00, 0xAD, 0xFF};
      Serial.println("sendiaqdata Task start");
      while (1)
      {
            // if (counter < 4)
            //{
             driver.setHeaderFrom(0x21);
             driver.setHeaderTo(0x21);
             driver.setHeaderId(0x21);
             driver.setHeaderFlags(0x21);

            driver.send(data, sizeof(data));

            driver.waitPacketSent();
            Serial.println("data send");
            counter++;
            //}
            vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
}

void loop()
{
      // Serial.print("counter: ");
      // Serial.println(counter);

      // // if (counter < 4)
      // // {
      // //       Serial.println("Sending data");
      // //       driver.setHeaderFrom(0x21);
      // //       driver.setHeaderTo(0x21);
      // //       driver.setHeaderId(0x21);
      // //       driver.setHeaderFlags(0x21);
      // //       driver.send(data, sizeof(data));
      // //       driver.waitPacketSent();
      // //       delay(1000);
      // //       counter++;
      // // }

      // if (counter < 4)
      // {
      //       // uint8_t to    = driver.headerTo();
      //       // uint8_t from  = driver.headerFrom();
      //       // uint8_t id    = driver.headerId();
      //       // uint8_t flags = driver.headerFlags();
      //       // Serial.print("RF22 Message To: 0x");
      //       // Serial.print(to, HEX);
      //       // Serial.print(" From: 0x");
      //       // Serial.print(from, HEX);
      //       // Serial.print(" Id: 0x");
      //       // Serial.print(id, DEC);
      //       // Serial.print(" Flags: 0x");
      //       // Serial.println(flags, HEX);
      //       Serial.println("Sending data");
      //       driver.setHeaderFrom(0x21);
      //       driver.setHeaderTo(0x21);
      //       driver.setHeaderId(0x21);
      //       driver.setHeaderFlags(0x21);
      //       driver.send(data, sizeof(data));
      // }
      // else
      // {
      //       driver.setHeaderFrom(0x21);
      //       driver.setHeaderTo(0x21);
      //       driver.setHeaderId(0x21);
      //       driver.setHeaderFlags(0x21);

      //       // driver.setHeaderFrom(0x50);
      //       // driver.setHeaderTo(0x00);
      //       // driver.setHeaderId(0x00);
      //       // driver.setHeaderFlags(0xAD);

      //       // if (counter == 4)
      //       // {
      //       //       // Serial.println("msg1");
      //       //       // driver.send(msg1, sizeof(msg1));
      //       //       // }else if (counter == 5){
      //       //       //   Serial.println("msg1");
      //       //       //   driver.send(msg1, sizeof(msg1));
      //       //       // }else if (counter == 6){
      //       //       //       Serial.println("msg2");
      //       //       //       driver.send(msg2, sizeof(msg2));
      //       //       // }else if (counter == 7){
      //       //       //       Serial.println("msg2");
      //       //       //       driver.send(msg2, sizeof(msg2));
      //       //       // }else if (counter == 8){
      //       //       //       Serial.println("msg3");
      //       //       //       driver.send(msg3, sizeof(msg3));
      //       //       // }else if (counter == 9){
      //       //       //       Serial.println("msg3");
      //       //       //       driver.send(msg3, sizeof(msg3));
      //       //       // }else if (counter == 10){
      //       //       //       Serial.println("msg4");
      //       //       //       driver.send(msg4, sizeof(msg4));
      //       //       // }else if (counter == 11){
      //       //       // Serial.println("msg4");
      //       //       // driver.send(msg4, sizeof(msg4));

      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 5)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 6)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 7)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 8)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 9)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 10)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 11)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 12)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 13)
      //       // {
      //       //       Serial.println("cmd1");
      //       //       driver.send(cmd1, sizeof(cmd1));
      //       // }
      //       // else if (counter == 14)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 15)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 16)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 17)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 18)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 19)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 20)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 21)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 22)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else if (counter == 23)
      //       // {
      //       //       Serial.println("cmd2");
      //       //       driver.send(cmd2, sizeof(cmd2));
      //       // }
      //       // else
      //       // {
      //       //       counter = 3;
      //       // }

      //       // counter = 3;
      // }
      // driver.waitPacketSent();
      delay(1000);
      // counter++;
}