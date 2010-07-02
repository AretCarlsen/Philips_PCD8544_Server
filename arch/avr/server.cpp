
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

// MapOS
#define INITIAL_PACKET_SINK_COUNT 2
#define INITIAL_PROCESS_DEFINITION_COUNT 2
#include <MapOS/arch/avr/main.hpp>

// UART MAP/MEP I/O
#define UART_OUTGOING_PACKET_BUFFER_CAPACITY 3
#define UART_INPUT_BUFFER_CAPACITY 50
#define UART_OUTPUT_BUFFER_CAPACITY 250
#define UART_INITIAL_BAUD_RATE 115200
#define UART_PACKET_SINK_INDEX 2
#include <MapOS/arch/avr/UartComms.hpp>

#include <Philips_PCD8544_driver/arch/avr/Philips_PCD8544_Server.hpp>

#include <MapOS/arch/avr/adc.hpp>

inline void init_LCD(LCD_t &lcd){
  SPI::init(SPI::FOSC_DIV_8);

  lcd.init();

// Initial splash screen
  lcd.contrast(0x7F);
  lcd.clear();
  lcd.image(splashScreen1);
  lcd.update();
// Leave up the splash screen for a half second
  sleep_ms(500);

// Clear the LCD
  //LcdContrast(0x00);
  lcd.clear();
  lcd.update();
}

inline void init(){
  uartComms.init();
  rprintfInit(uartSendByte);

  init_ADC();

// Wait for Eeprom to become available
  eeprom_busy_wait();
}

void main() {
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

  // Provides kernel sinks (0-1)
#include <MapOS/arch/avr/main.cpp>
#include <MapOS/arch/avr/UartComms.cpp>

// State control (nonvolatile save/load)
#include "StateControlServer.hpp"

// LCD
  LCD_t lcd;
  init_LCD(lcd);
// String LCD server
  LCDServer_t process_lcdServer(&lcd);
  // Routing graph element
  packetSinks.setExpand(5, &process_lcdServer);
  // Millisecond frequency
  scheduler.add_process(&process_lcdServer, 1000);

// State control (save/load)
  // Routing graph element
  packetSinks.setExpand(4, &process_stateControlServer);
  // Half second frequency
  scheduler.add_process(&process_stateControlServer, 500000);

  DEBUGprint_RARE("Sched st\n");

  while(1){
    // Argument is in microseconds.
    scheduler.process(get_systemTime());
  }
}

