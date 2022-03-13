// rf22_reliable_datagram_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging client
// with the RHReliableDatagram class, using the RH_RF22 driver to control a RF22 radio.
// It is designed to work with the other example rf22_reliable_datagram_server
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

#include <RHReliableDatagram.h>
#include <RH_RF22.h>
#include <SPI.h>
#include <RHSoftwareSPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 5

// Singleton instance of the radio driver
// RH_RF22 driver;
RHSoftwareSPI spi;
// ss_pin nSEL GPIO15
// irq_pin nIRQ GPIO5
RH_RF22 driver(22, 23, spi);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

void setup()
{
  Serial.begin(115200);

  spi.setPins(12, 13, 14);

  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
}

uint8_t data[] = {0x11, 0x0E, 0x10, 0xE0, 0x50, 0x00, 0x00,
                  0xAD, 0x00, 0x10, 0x02, 0xBF, 0x00, 0x0D,
                  0x01, 0x01, 0x00, 0x40, 0x00, 0x41, 0x00,
                  0x44, 0x4F};
// Dont put this on the stack:
uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];

void loop()
{
  Serial.println("Sending data");
  driver.send(data, sizeof(data));
  // Send a message to manager_server

  driver.waitPacketSent();
  delay(1000);
}