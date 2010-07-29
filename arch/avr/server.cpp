// Copyright (C) 2010, Aret N Carlsen (aretcarlsen@autonomoustools.com).
// Philips PCD8544 driver (C++).
// Licensed under GPLv3 and later versions. See license.txt or <http://www.gnu.org/licenses/>.


#include <ATcommon/arch/avr/avr.hpp>
#include <avrlib/rprintf.h>

#include <AVR_Objects/spi.hpp>
#include <Philips_PCD8544_driver/arch/avr/Philips_PCD8544.hpp>
#include "images.hpp"

#include "types.hpp"

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

// Packet sinks: 0-Kernel, [1-port], 2-StateControl, 3-LCD1, 4-LCD2 
#define SCS_PACKET_SINK_INDEX 2
#define LCD1_COMMAND__PACKET_SINK_INDEX 3
#define LCD1_STRING__PACKET_SINK_INDEX 4
#define LCD2_COMMAND__PACKET_SINK_INDEX 5
#define LCD2_STRING__PACKET_SINK_INDEX 6

#define MAX_PACKET_SINK_INDEX LCD2_STRING__PACKET_SINK_INDEX
// Processes: uartComms, LCD1 servers (2), LCD2 server (2), StateControlServer
#define LOCAL_PROCESS_COUNT (1 + 2 + 2 + 1)
// Fetch MapOS process count.
#include <MapOS/arch/avr/count.hpp>
#define INITIAL_PROCESS_COUNT (MAPOS_PROCESS_COUNT + LOCAL_PROCESS_COUNT)

#include <MapOS/arch/avr/main.hpp>

// UART MAP/MEP I/O. Define early on to initialize for debugging during other initializations.
#define UART_OUTGOING_PACKET_BUFFER_CAPACITY 3
#define UART_INPUT_BUFFER_CAPACITY 20
#define UART_OUTPUT_BUFFER_CAPACITY 100
#define UART_INITIAL_BAUD_RATE 115200
#define UART_PACKET_SINK_INDEX 2
#include <MapOS/arch/avr/UartComms.hpp>

#include <Philips_PCD8544_driver/arch/avr/Philips_PCD8544_Server.hpp>

#include <MapOS/arch/avr/adc.hpp>

void DEBUGflush(){
  uartComms.triggerOutgoing();
}

template <typename LCD_t>
inline void init_LCD(LCD_t &lcd){
  lcd.init();

// Initial splash screen
  lcd.contrast(0x7F);
  lcd.clear();
  lcd.image(splashScreen1);
  lcd.update();

/*
// Leave up the splash screen a half second
  sleep_ms(500);
// Clear the LCD
  //LcdContrast(0x00);
  lcd.clear();
  lcd.update();
*/
}

inline void init(){
  rprintfInit(uartSendByte);
//  DEBUGprint("init\n"); uartComms.triggerOutgoing(); while(1) asm("sleep");

  init_ADC();

// Wait for Eeprom to become available
  eeprom_busy_wait();
}

void main() {
  init();

// Give the outgoing sink 50ms to simmer down...
  sleep_ms(50);

// Provides kernel processes and sinks (0-1)
#include <MapOS/arch/avr/main.cpp>
// Provides uartComms process.
#include <MapOS/arch/avr/UartComms.cpp>

// State control (nonvolatile save/load)
#include "StateControlServer.hpp"

// LCD SPI bus. Initializes SPI pins (including SS).
  SPI::init(SPI::FOSC_DIV_8);
// LCD 1
  LCD1_t lcd1;
  init_LCD(lcd1);
  // LCD1 servers
  LCD1CommandServer_t process_lcd1CommandServer(&lcd1);
  LCD1StringServer_t  process_lcd1StringServer(&lcd1);
  // Routing graph elements
  packetSinks.set(LCD1_COMMAND__PACKET_SINK_INDEX, &process_lcd1CommandServer);
  packetSinks.set( LCD1_STRING__PACKET_SINK_INDEX, &process_lcd1StringServer);
  // Scheduled processes
  scheduler.add_process(&process_lcd1CommandServer, 1000);
  scheduler.add_process( &process_lcd1StringServer, 1000);
// LCD 2
  LCD2_t lcd2;
  init_LCD(lcd2);
 // String LCD server
  LCD2CommandServer_t process_lcd2CommandServer(&lcd2);
  LCD2StringServer_t  process_lcd2StringServer(&lcd2);
  // Routing graph element
  packetSinks.set(LCD2_COMMAND__PACKET_SINK_INDEX, &process_lcd2CommandServer);
  if(! (packetSinks.set( LCD2_STRING__PACKET_SINK_INDEX, &process_lcd2StringServer))){
    DEBUGprint_FORCE("sEFLD!\n");
  }
  // Millisecond frequency
  scheduler.add_process(&process_lcd2CommandServer, 1000);
  scheduler.add_process( &process_lcd2StringServer, 1000);

// State control (save/load)
  // Routing graph element
  packetSinks.set(SCS_PACKET_SINK_INDEX, &process_stateControlServer);
  // Half second frequency
  scheduler.add_process(&process_stateControlServer, 500000);

  DEBUGprint_RARE("Sched st\n");

  while(1){
    // Argument is in microseconds.
    scheduler.process(get_systemTime());
  }
}

