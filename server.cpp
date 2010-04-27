
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "../common/PCD8544/pcd8544.hpp"
#include "images.hpp"

// Actually 83.2us
#define SYSTEM_TIMER_PERIOD_us 83      // Microseconds
#define PRIMARY_MEMORY_POOL_LIMIT 1600

#define OUTGOING_PACKET_BUFFER_CAPACITY 3
#define INPUT_BUFFER_CAPACITY 50
#define OUTPUT_BUFFER_CAPACITY 250
#define UART_INITIAL_BAUD_RATE 115200


#include "../globals.hpp"

// MapOS
#include "../common/MapOS/main.hpp"
//#include "../common/MapOS/templateInstantiations.cpp"
#include "../common/MapOS/uart.hpp"

#include "../common/PCD8544/LCDServer.hpp"

#include "adc.hpp"

// Packet sinks
#define INITIAL_PACKET_SINK_COUNT 2
// Process definitions
#define INITIAL_PROCESS_DEFINITION_COUNT 3

#include "../common/MapOS/verify.hpp"

inline void init_LCD(){
  LcdInit();

// Initial splash screen
  LcdContrast(0x7F);
  LcdClear();
  LcdImage(splashScreen2);
  LcdUpdate();
// Leave up the splash screen for a half second
  sleep_ms(500);

// Clear the LCD
  //LcdContrast(0x00);
  LcdClear();
  LcdUpdate();
}

inline void init(){
  init_UART();
  rprintfInit(uartSendByte);
// DEBUGprint init
  init_DEBUGprint();

  init_ADC();
// SPI is initialized by LcdInit().

  init_LCD();

// Wait for Eeprom to become available
  eeprom_busy_wait();
// Verify Eeprom
  verify_eeprom();
}

int main()
{
  init();

// Give the outgoing sink 50ms to simmer down...
  sleep_ms(50);

/*
while(true){
// sleep test
  DEBUGprint("1");
  sleep_ms(50);
}
*/

#include "../common/MapOS/main.cpp"

#include "StateControlServer.hpp"

// String LCD server
  StringLCDServer process_lcdServer;
  // Routing graph element
  packetSinks.setExpand(5, &process_lcdServer);
  // Millisecond frequency
  scheduler.add_process(&process_lcdServer, 1000);

// State control (save/load)
  // Routing graph element
  packetSinks.setExpand(4, &process_stateControlServer);
  // Half second frequency
  scheduler.add_process(&process_stateControlServer, 500000);

// Make sure outgoing data bus is triggered occasionally.
  // Half millisecond frequency
  scheduler.add_process(&process_triggerOutgoing, 500);

  DEBUGprint_RARE("Sched st\n");

  while(1){
    // Argument is in microseconds.
    scheduler.process(SYSTEM_TIME);
  }

  return 0;
}

