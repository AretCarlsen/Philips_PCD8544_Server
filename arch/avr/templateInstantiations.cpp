
#include <ATcommon/arch/avr/avr.hpp>
#include <MapOS/templateInstantiations.cpp>
#include "types.hpp"

template class Philips_PCD8544::Philips_PCD8544<LCD_SPI_t, LCD_DC_Pin_t, LCD_CE_Pin_t, LCD_RST_Pin_t>;
template class Philips_PCD8544::StringLCDServer<LCD_t>;

