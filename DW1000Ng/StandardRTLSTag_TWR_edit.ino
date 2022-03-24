/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/* 
 * StandardRTLSTag_TWR.ino
 * 
 * This is an example tag in a RTLS using two way ranging ISO/IEC 24730-62_2013 messages
 */

#include <SPI.h>
#include <DW1000Ng.hpp>
#include <DW1000NgUtils.hpp>
#include <DW1000NgTime.hpp>
#include <DW1000NgConstants.hpp>
#include <DW1000NgRanging.hpp>
#include <DW1000NgRTLS.hpp>

// connection pins
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4; // spi select 
//#if defined(ESP8266)
//const uint8_t PIN_SS = 15;
//#else
//const uint8_t PIN_SS = SS; // spi select pin
//const uint8_t PIN_RST = 9;
//#endif

// Extended Unique Identifier register. 64-bit device identifier. Register file: 0x01
char EUI[] = "BB:CC:DD:EE:FF:00:00:01";

volatile uint32_t blink_rate = 20;

device_configuration_t DEFAULT_CONFIG = {
    false,
    true,
    false,
    true,
    false,
    SFDMode::STANDARD_SFD,
    Channel::CHANNEL_3,
    DataRate::RATE_6800KBPS,
    PulseFrequency::FREQ_16MHZ,
    PreambleLength::LEN_64,
    PreambleCode::CODE_5
};

frame_filtering_configuration_t TAG_FRAME_FILTER_CONFIG = {
    false,
    false,
    true,
    false,
    false,
    false,
    false,
    false
};

sleep_configuration_t SLEEP_CONFIG = {
    false,  // onWakeUpRunADC   reg 0x2C:00
    false,  // onWakeUpReceive
    false,  // onWakeUpLoadEUI
    true,   // onWakeUpLoadL64Param
    false,   // preserveSleep
    false,   // enableSLP    reg 0x2C:06
    false,  // enableWakePIN
    false    // enableWakeSPI
};

void setup() {
    // DEBUG monitoring
    Serial.begin(115200);
    Serial.println(F("### DW1000Ng-arduino-ranging-tag ###"));
    // initialize the driver
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ng::initialize(PIN_SS, PIN_IRQ, PIN_RST);
//    #if defined(ESP8266)
//    DW1000Ng::initializeNoInterrupt(PIN_SS);
//    #else
//    DW1000Ng::initializeNoInterrupt(PIN_SS, PIN_RST);
//    #endif
    Serial.println("DW1000Ng initialized ...");
    // general configuration
    DW1000Ng::applyConfiguration(DEFAULT_CONFIG);
    DW1000Ng::enableFrameFiltering(TAG_FRAME_FILTER_CONFIG);
    
    DW1000Ng::setEUI(EUI);

    DW1000Ng::setNetworkId(RTLS_APP_ID);

    DW1000Ng::setAntennaDelay(16436);

    DW1000Ng::applySleepConfiguration(SLEEP_CONFIG);

    DW1000Ng::setPreambleDetectionTimeout(15);
    DW1000Ng::setSfdDetectionTimeout(273);
    DW1000Ng::setReceiveFrameWaitTimeoutPeriod(1000);
    
    Serial.println(F("Committed configuration ..."));
    // DEBUG chip info and registers pretty printed
    char msg[128];
    DW1000Ng::getPrintableDeviceIdentifier(msg);
    Serial.print("Device ID: "); Serial.println(msg);
    DW1000Ng::getPrintableExtendedUniqueIdentifier(msg);
    Serial.print("Unique ID: "); Serial.println(msg);
    DW1000Ng::getPrintableNetworkIdAndShortAddress(msg);
    Serial.print("Network ID & Device Address: "); Serial.println(msg);
    DW1000Ng::getPrintableDeviceMode(msg);
    Serial.print("Device mode: "); Serial.println(msg);    
}

void loop() {
//    DW1000Ng::deepSleep();
    delay(blink_rate);
    DW1000Ng::spiWakeup();
    DW1000Ng::setEUI(EUI);

    RangeInfrastructureResult res = DW1000NgRTLS::tagTwrLocalize(1500);
    if(res.success)
        blink_rate = res.new_blink_rate;
}
