
#include <ATcommon/arch/avr/avr.hpp>
#include <MapOS/templateInstantiations.cpp>
#include "types.hpp"

// LCD1
template class Philips_PCD8544::Philips_PCD8544<LCD_SPI_t, LCD1_DC_Pin_t, LCD1_CE_Pin_t, LCD1_RST_Pin_t>;
template class Philips_PCD8544::StringServer<LCD1_t>;
template class Philips_PCD8544::CommandServer<LCD1_t>;

// LCD2
template class Philips_PCD8544::Philips_PCD8544<LCD_SPI_t, LCD2_DC_Pin_t, LCD2_CE_Pin_t, LCD2_RST_Pin_t>;
template class Philips_PCD8544::StringServer<LCD2_t>;
template class Philips_PCD8544::CommandServer<LCD2_t>;

