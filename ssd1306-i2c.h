
#ifndef __SSD1306_I2C_H__
#define __SSD1306_I2C_H__

void ssd1306_init(int i2c_dev);
void ssd1306_string(uint8_t x, uint8_t y, const char *text);

#endif	/* __SSD1306_I2C_H__ */

