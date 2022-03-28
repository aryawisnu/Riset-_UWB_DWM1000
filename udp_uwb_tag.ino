#include <SPI.h>
#include <DW1000Ranging.h>
#include "link.h"

// SPI pins
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
#define RX2 16
#define TX2 17
// Control pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin
//i2c pins
#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

struct MyLink *uwb_data;
int index_num = 0;
long runtime = 0;
String all_json = "";

void setup()
{
    Serial.begin(115200);
    Serial2.begin(115200,SERIAL_8N1, RX2, TX2);
    delay(1000);
//    WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR); //mulai i2c
    //init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);

    //we start the module as a tag
    DW1000Ranging.startAsTag("04:00:22:EA:82:60:E2:9C", DW1000.MODE_LONGDATA_FAST_ACCURACY);
//    WireSlave.onRequest(send_i2c);
    uwb_data = init_link();
}

void loop()
{
    DW1000Ranging.loop();
    if ((millis() - runtime) > 100){   
        make_link_json(uwb_data, &all_json);
//        Serial.print("ini data sesuatu = ");
//        Serial.println(all_json);
        Serial2.println(all_json);
        runtime = millis();
    }
    
}
//void send_i2c(){
//  WireSlave.print(all_json);
////  Serial.print("ini data sesuatu = ");
////  Serial.println(all_json);
//}

void newRange()
{
    char c[30];
    fresh_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
}

void newDevice(DW1000Device *device)
{
    add_link(uwb_data, device->getShortAddress());
}

void inactiveDevice(DW1000Device *device)
{
    delete_link(uwb_data, device->getShortAddress());
}
