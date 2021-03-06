
#include <AVR_Objects/io.hpp>
#include <AVR_Objects/spi.hpp>
#include <Philips_PCD8544_driver/arch/avr/Philips_PCD8544.hpp>
#include <Philips_PCD8544_driver/arch/avr/Philips_PCD8544_Server.hpp>

typedef SPI LCD_SPI_t;

// Pins and object
typedef OutPin_B_0 LCD1_DC_Pin_t;
typedef OutPin_D_6 LCD1_CE_Pin_t;
typedef OutPin_D_5 LCD1_RST_Pin_t;
typedef Philips_PCD8544::Philips_PCD8544<LCD_SPI_t, LCD1_DC_Pin_t, LCD1_CE_Pin_t, LCD1_RST_Pin_t> LCD1_t;
// String and Command servers
typedef Philips_PCD8544::StringServer<LCD1_t> LCD1StringServer_t;
typedef Philips_PCD8544::CommandServer<LCD1_t> LCD1CommandServer_t;

// Pins and object
typedef OutPin_B_1 LCD2_DC_Pin_t;
typedef OutPin_B_2 LCD2_CE_Pin_t;
typedef OutPin_D_7 LCD2_RST_Pin_t;
typedef Philips_PCD8544::Philips_PCD8544<LCD_SPI_t, LCD2_DC_Pin_t, LCD2_CE_Pin_t, LCD2_RST_Pin_t> LCD2_t;
// String and Command servers
typedef Philips_PCD8544::StringServer<LCD2_t> LCD2StringServer_t;
typedef Philips_PCD8544::CommandServer<LCD2_t> LCD2CommandServer_t;

