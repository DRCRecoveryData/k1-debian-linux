#include "panel-creality-lcd.h"
#include <linux/types.h>
#include <linux/delay.h>

int lcd_st7701_tjc_lc_detect(void)
{
	uint8_t buffer[5];
	SPI_ReadData(0xa1, buffer, 5);
	if((buffer[0] == 0x77 && buffer[1] == 0x01) || (buffer[0] == 0x88 && buffer[1] == 0x02) || (buffer[0] == 0x99 && buffer[1] == 0x03)){
		SPI_ReadData(0x04, buffer, 3);
		if(buffer[0] == 0x54 && buffer[1] == 0x01 && buffer[2] == 0x02){
			return 1;
		}
	}
	
	return 0;
}

void lcd_st7701_tjc_lc_init(void)
{
	/* 1. Вход в Command2, Bank 0 */
	SPI_SendCommand(0xFF);
	SPI_SendData(0x77);
	SPI_SendData(0x01);
	SPI_SendData(0x00);
	SPI_SendData(0x00);
	SPI_SendData(0x10);

	/* 2. Основные настройки экрана (60Hz) */
	SPI_SendCommand(0xC0); // Line Control
	SPI_SendData(0x3B);
	SPI_SendData(0x00);

	SPI_SendCommand(0xC1); // Porch Control (ВАЖНО для 60Гц)
	SPI_SendData(0x0D);    // VBP
	SPI_SendData(0x02);    // VFP

	SPI_SendCommand(0xC2); // PCLK Control
	SPI_SendData(0x31);
	SPI_SendData(0x05);

	SPI_SendCommand(0xCC);
	SPI_SendData(0x10);

	/* Gamma settings (Bank 0) */
	SPI_SendCommand(0xB0);
	SPI_SendData(0x00); SPI_SendData(0x11); SPI_SendData(0x18); SPI_SendData(0x0E);
	SPI_SendData(0x11); SPI_SendData(0x06); SPI_SendData(0x07); SPI_SendData(0x08);
	SPI_SendData(0x07); SPI_SendData(0x22); SPI_SendData(0x04); SPI_SendData(0x12);
	SPI_SendData(0x0F); SPI_SendData(0xAA); SPI_SendData(0x31); SPI_SendData(0x18);

	SPI_SendCommand(0xB1);
	SPI_SendData(0x00); SPI_SendData(0x11); SPI_SendData(0x19); SPI_SendData(0x0E);
	SPI_SendData(0x12); SPI_SendData(0x07); SPI_SendData(0x08); SPI_SendData(0x08);
	SPI_SendData(0x08); SPI_SendData(0x22); SPI_SendData(0x04); SPI_SendData(0x11);
	SPI_SendData(0x11); SPI_SendData(0xA9); SPI_SendData(0x32); SPI_SendData(0x18);

	/* 3. Настройки питания (Bank 1) */
	SPI_SendCommand(0xFF);
	SPI_SendData(0x77);
	SPI_SendData(0x01);
	SPI_SendData(0x00);
	SPI_SendData(0x00);
	SPI_SendData(0x11);

	SPI_SendCommand(0xB0); SPI_SendData(0x60); // Vop
	SPI_SendCommand(0xB1); SPI_SendData(0x32); // Vcom
	SPI_SendCommand(0xB2); SPI_SendData(0x07); // VGH
	SPI_SendCommand(0xB3); SPI_SendData(0x80);
	SPI_SendCommand(0xB5); SPI_SendData(0x49); // VGL
	SPI_SendCommand(0xB7); SPI_SendData(0x85);
	SPI_SendCommand(0xB8); SPI_SendData(0x21);
	SPI_SendCommand(0xC1); SPI_SendData(0x78);
	SPI_SendCommand(0xC2); SPI_SendData(0x78);

	/* 4. Настройки GIP (Gate In Panel) - адаптированы для скорости 60Гц */
	SPI_SendCommand(0xE0); SPI_SendData(0x00); SPI_SendData(0x1B); SPI_SendData(0x02);

	SPI_SendCommand(0xE1);
	SPI_SendData(0x08); SPI_SendData(0xA0); SPI_SendData(0x00); SPI_SendData(0x00);
	SPI_SendData(0x07); SPI_SendData(0xA0); SPI_SendData(0x00); SPI_SendData(0x00);
	SPI_SendData(0x00); SPI_SendData(0x44); SPI_SendData(0x44);

	SPI_SendCommand(0xE2);
	SPI_SendData(0x11); SPI_SendData(0x11); SPI_SendData(0x44); SPI_SendData(0x44);
	SPI_SendData(0xED); SPI_SendData(0xA0); SPI_SendData(0x00); SPI_SendData(0x00);
	SPI_SendData(0xEC); SPI_SendData(0xA0); SPI_SendData(0x00); SPI_SendData(0x00);

	SPI_SendCommand(0xE3); SPI_SendData(0x00); SPI_SendData(0x00); SPI_SendData(0x11); SPI_SendData(0x11);
	SPI_SendCommand(0xE4); SPI_SendData(0x44); SPI_SendData(0x44);

	SPI_SendCommand(0xE5);
	SPI_SendData(0x0A); SPI_SendData(0xE9); SPI_SendData(0xD8); SPI_SendData(0xA0);
	SPI_SendData(0x0C); SPI_SendData(0xEB); SPI_SendData(0xD8); SPI_SendData(0xA0);
	SPI_SendData(0x0E); SPI_SendData(0xED); SPI_SendData(0xD8); SPI_SendData(0xA0);
	SPI_SendData(0x10); SPI_SendData(0xEF); SPI_SendData(0xD8); SPI_SendData(0xA0);

	SPI_SendCommand(0xE6); SPI_SendData(0x00); SPI_SendData(0x00); SPI_SendData(0x11); SPI_SendData(0x11);
	SPI_SendCommand(0xE7); SPI_SendData(0x44); SPI_SendData(0x44);

	SPI_SendCommand(0xE8);
	SPI_SendData(0x09); SPI_SendData(0xE8); SPI_SendData(0xD8); SPI_SendData(0xA0);
	SPI_SendData(0x0B); SPI_SendData(0xEA); SPI_SendData(0xD8); SPI_SendData(0xA0);
	SPI_SendData(0x0D); SPI_SendData(0xEC); SPI_SendData(0xD8); SPI_SendData(0xA0);
	SPI_SendData(0x0F); SPI_SendData(0xEE); SPI_SendData(0xD8); SPI_SendData(0xA0);

	SPI_SendCommand(0xEB);
	SPI_SendData(0x02); SPI_SendData(0x00); SPI_SendData(0xE4); SPI_SendData(0xE4);
	SPI_SendData(0x88); SPI_SendData(0x00); SPI_SendData(0x40);

	SPI_SendCommand(0xEC); SPI_SendData(0x3C); SPI_SendData(0x00);

	SPI_SendCommand(0xED);
	SPI_SendData(0xAB); SPI_SendData(0x89); SPI_SendData(0x76); SPI_SendData(0x54);
	SPI_SendData(0x02); SPI_SendData(0xFF); SPI_SendData(0xFF); SPI_SendData(0xFF);
	SPI_SendData(0xFF); SPI_SendData(0xFF); SPI_SendData(0xFF); SPI_SendData(0x20);
	SPI_SendData(0x45); SPI_SendData(0x67); SPI_SendData(0x98); SPI_SendData(0xBA);

	/* 5. Выход и включение */
	SPI_SendCommand(0xFF);
	SPI_SendData(0x77);
	SPI_SendData(0x01);
	SPI_SendData(0x00);
	SPI_SendData(0x00);
	SPI_SendData(0x00);

	SPI_SendCommand(0x11); // Sleep Out
	udelay(120000);        // Ждем 120мс (стандарт для ST7701)

	SPI_SendCommand(0x29); // Display On
	udelay(20000);
}
