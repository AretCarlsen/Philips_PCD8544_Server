
#include <AVR_Objects/io.hpp>
#include <AVR_Objects/spi.hpp>
#include <Philips_PCD8544_driver/arch/avr/Philips_PCD8544.hpp>
#include <Philips_PCD8544_driver/arch/avr/Philips_PCD8544_Server.hpp>

typedef SPI LCD_SPI_t;
typedef OutPin_B_0 LCD_DC_Pin_t;
typedef OutPin_B_2 LCD_CE_Pin_t;
typedef OutPin_B_4 LCD_RST_Pin_t;
typedef Philips_PCD8544::Philips_PCD8544<LCD_SPI_t, LCD_DC_Pin_t, LCD_CE_Pin_t, LCD_RST_Pin_t> LCD_t;
typedef Philips_PCD8544::StringLCDServer<LCD_t> LCDServer_t;

