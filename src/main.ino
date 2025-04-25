#include <Arduino.h>
#include "mcp2515.h"
#include "esp_system.h"
#include "driver/spi_master.h"

#define MISO  19
#define MOSI  23
#define SCK   18
#define SS    5

__u32 filterSpeedVal = 0x0d1;
__u32 filterTorqueVal = 0x048;
__u32 filterBattery = 0x111;

struct can_frame frame;
spi_device_handle_t spi;

MCP2515 canbus(&spi);

void setup(void){
  Serial.begin(115200);
  while(!Serial){};

  canbus.setBitrate(CAN_500KBPS, MCP_8MHZ);
  canbus.setNormalMode();
  // canbus.setFilter(41,true, NULL);
  Serial.println("CANBus Configurated");
}


void loop(void){
  // Serial.println(canbus.readMessage(&frame));
  if (canbus.readMessage(&frame) == MCP2515::ERROR_OK) {
    if(frame.can_id == filterSpeedVal){
      Serial.print("\n Speed: ");
      for (int i = 0; i < 8; i++) {
        Serial.print(frame.data[i]); 
      }
    }
    if(frame.can_id == filterTorqueVal){
      Serial.print("\n Torque: ");
      for (int i = 0; i < 8; i++) {
        Serial.print(frame.data[i]); 
      }
    }
    if(frame.can_id == filterBattery){
      // D0-D1: const 00 00
      // D2-D3 ushort: battery discharge limit indicator: normally 4e 20 = 20000, less when battery runs low
      // D4: const 00
      // D5 uchar: last full battery charge [Ah]; 8a, 89, 87, 86 changed after charging completely empty battery, OWL active and Gene performance 72 (400Wh battery)
      // D6 uchar: battery state of charge in % (0: battery empty, 100: full)
  
      Serial.print("\n BatteryInfo: ");
      for (int i = 0; i < 8; i++) {
        Serial.print(frame.data[i]); 
      }
    }
  }
  // delay(10);
}