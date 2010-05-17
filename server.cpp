
#include "../globals.hpp"
/*
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
*/

#include "../drivers/SPI/SPI.hpp"
#include "../drivers/Philips_PCD8544/pcd8544.hpp"
#include "images.hpp"

// Actually 83.2us
#define SYSTEM_TIMER_PERIOD_us 83      // Microseconds

// ADC channel count
#define ADC_CHANNEL_COUNT 2
// Number of raw readings that are averaged before sinking into the input buffers.
// About 12,019 readings occur per second (20MHz, /128 prescaler, 13 cycles per measurement),
// or one every 83.2us. Averaging 120 cuts down to 100 measurements per second (one every 10ms).
#define ADC_AVERAGED_READINGS 120
#define ADC_READING_PERIOD (ADC_AVERAGED_READINGS * SYSTEM_TIMER_PERIOD_us)

#define PRIMARY_MEMORY_POOL_LIMIT 1600

#define OUTGOING_PACKET_BUFFER_CAPACITY 3
#define INPUT_BUFFER_CAPACITY 50
#define OUTPUT_BUFFER_CAPACITY 250
#define UART_INITIAL_BAUD_RATE 115200


#include "../globals.hpp"

// MapOS
#include "../common/MapOS/main.hpp"
#include "../common/MapOS/uart.hpp"

#include "../drivers/Philips_PCD8544/LCDServer.hpp"

#include "../common/MapOS/adc.hpp"

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
  LcdImage(splashScreen1);
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

  SPI::init(SPI::FOSC_DIV_8);
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
    scheduler.process(get_systemTime());
  }

  return 0;
}

