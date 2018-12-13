#include"stm32f10x.h"
const unsigned char Num12_18[10][36]={//12*18
	{
		0x00,0x00,0x0E,0x00,0x1F,0x80,0x39,0xC0,0x30,0xC0,0x60,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,
		0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xC0,0x30,0xC0,0x39,0xC0,0x1F,0x80,0x06,0x00
	},
	{
		0x00,0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x0F,0x00,0x1B,0x00,0x13,0x00,0x03,0x00,0x03,0x00,
		0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00
	},
	{
		0x00,0x00,0x0F,0x00,0x3F,0x80,0x31,0xC0,0x60,0xC0,0x60,0xC0,0x00,0xC0,0x00,0xC0,0x01,0x80,
		0x01,0x80,0x03,0x00,0x06,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x30,0x00,0x7F,0xE0,0x00,0x00
	},
	{
		0x00,0x00,0x0F,0x00,0x1F,0x80,0x30,0xC0,0x60,0xC0,0x60,0xC0,0x00,0xC0,0x00,0xC0,0x07,0x80,
		0x07,0x80,0x01,0xC0,0x00,0xC0,0x00,0x60,0x60,0x60,0x60,0xC0,0x31,0xC0,0x1F,0x80,0x0E,0x00,
	},
	{
		0x00,0x00,0x00,0x80,0x01,0x80,0x03,0x80,0x03,0x80,0x07,0x80,0x0D,0x80,0x0D,0x80,0x19,0x80,
		0x31,0x80,0x31,0x80,0x61,0x80,0xFF,0xF0,0xFF,0xF0,0x01,0x80,0x01,0x80,0x01,0x80,0x00,0x00,
	},
	{
		0x00,0x00,0x3F,0xC0,0x30,0x00,0x30,0x00,0x30,0x00,0x20,0x00,0x7F,0x80,0x73,0xC0,0x60,0xC0,
		0x00,0xC0,0x00,0x60,0x00,0x60,0xC0,0xC0,0x60,0xC0,0x61,0xC0,0x3F,0x80,0x0E,0x00,0x00,0x00,
	},
	{
		0x00,0x00,0x03,0x80,0x03,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x1F,0x00,0x3F,0xC0,
		0x30,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x30,0xC0,0x1F,0xC0,0x0F,0x00,
	},
	{
		0x00,0x00,0x00,0x00,0x7F,0xE0,0x00,0x60,0x00,0xC0,0x00,0xC0,0x01,0x80,0x01,0x80,0x03,0x00,
		0x03,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x18,0x00,0x00,0x00,
	},
	{
		0x00,0x00,0x0E,0x00,0x3F,0x80,0x30,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x31,0xC0,0x3F,0x80,
		0x3F,0x80,0x71,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0xC0,0x3F,0x80,0x0F,0x00,
	},
	{
		0x00,0x00,0x0E,0x00,0x3F,0x80,0x71,0xC0,0x60,0xC0,0x60,0xE0,0xC0,0xE0,0x60,0xC0,0x60,0xC0,
		0x71,0xC0,0x3F,0x80,0x0F,0x80,0x03,0x00,0x07,0x00,0x06,0x00,0x0C,0x00,0x0C,0x00,0x18,0x00,
	}
};
