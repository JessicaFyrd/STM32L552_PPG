/*
****************************************************************************
** Copyright (C) 2020 MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
**  USE OR OTHER DEALINGS IN THE SOFTWARE.
****************************************************************************
*
* Modification:
*
*  MAX86916_ppg.c
*
*  Created on: 28 avr. 2022
*      Author: Jessica Fayard
*/

//Includes ===========================================================================================================================================
#include "MAX86916_ppg.h"


//Variables ==========================================================================================================================================
I2C_HandleTypeDef I2C;
HAL_StatusTypeDef fifo_read;
uint8_t buf_r=0; 				// Read value
uint8_t buf[32]={0}; 			// Write value


//Functions ==========================================================================================================================================
heartrate10_return_value_t HEARTRATE10_DEFAULT_4LEDS_CFG(I2C_HandleTypeDef i2c)
{
	I2C=i2c;

    //Shutdown device
	HEARTRATE10_SHUTDOWN_DEVICE();
    READ(HEARTRATE10_REG_MODE_CFG1, &buf_r);

    //Reset device
    HEARTRATE10_RESET_DEVICE();

    //Part ID
    READ(HEARTRATE10_REG_PART_ID, &buf_r);
    if (HEARTRATE10_PART_ID != buf_r)
    {
        return HEARTRATE10_ERROR;
    }

    //Set the led mode
    HEARTRATE10_SET_MODE(MAX86916_MODE_FLEX);

    //Set led sequences [ IR, RED, GREEN, BLUE ]
    	//RED-IR
    HEARTRATE10_SET_LED_SEQUENCE_1(MAX86916_LED_SEQ_LED1);
    HEARTRATE10_SET_LED_SEQUENCE_2(MAX86916_LED_SEQ_LED2);
    READ(HEARTRATE10_REG_LED_SEQ1, &buf_r);
    	//BLUE-GREEN
    HEARTRATE10_SET_LED_SEQUENCE_3(MAX86916_LED_SEQ_LED3);
    HEARTRATE10_SET_LED_SEQUENCE_4(MAX86916_LED_SEQ_LED4);
    READ(HEARTRATE10_REG_LED_SEQ2, &buf_r);

    //Set scale range, sample per second and led pulse widths
    HEARTRATE10_ADC_RANGE(MAX86916_ADC_RANGE_32768);
    HEARTRATE10_SR(MAX86916_SR_100);
    HEARTRATE10_LED_PULSE_WIDTH(MAX86916_LED_PW_70);
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);

    //Set led range
    HEARTRATE10_LED_RANGE_1(MAX86916_LED_RANGE_50); 	//Current range 0 to 50 mA for the 4 LEDs
    HEARTRATE10_LED_RANGE_2(MAX86916_LED_RANGE_50);
    HEARTRATE10_LED_RANGE_3(MAX86916_LED_RANGE_50);
    HEARTRATE10_LED_RANGE_4(MAX86916_LED_RANGE_50);

    //Set led power
    buf[0]=0xFF; 										//nominal current pulse amplitude of 50.4 mA for all the LEDs
    HEARTRATE10_LED_POWER_1(buf[0]); 					//IR
    HEARTRATE10_LED_POWER_2(buf[0]); 					//RED
    HEARTRATE10_LED_POWER_3(buf[0]); 					//GREEN
    HEARTRATE10_LED_POWER_4(buf[0]); 					//BLUE

    //Sample average
    heartrate10_FIFO_SAMPLE_AVERAGE(MAX86916_FIFO_SAMPLE_AVERAGE_1);

    //Enable fifo overflow
    heartrate10_FIFO_RO(MAX86916_FIFO_RO_EN);

    //Number of unread data for interruption
    heartrate10_FIFO_A_FULL(MAX86916_FIFO_A_FULL_20);

    //Enable Int on data read
    heartrate10_A_FULL_EN(MAX86916_A_FULL_DIS);
    heartrate10_SMP_RDY_EN(MAX86916_SMP_RDY_EN);
    heartrate10_ALC_OVF_EN(MAX86916_ALC_OVF_DIS);
    heartrate10_PROX_INT_EN(MAX86916_PROX_INT_DIS);
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);

	READ(HEARTRATE10_REG_INT_STATUS, &buf_r);			//Clean interrupts

    return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_DEFAULT_2LEDS_CFG(I2C_HandleTypeDef i2c)
{
	I2C=i2c;

    //Shutdown device
	HEARTRATE10_SHUTDOWN_DEVICE();
    READ(HEARTRATE10_REG_MODE_CFG1, &buf_r);

    //Reset device
    HEARTRATE10_RESET_DEVICE();

    //Part ID
    READ(HEARTRATE10_REG_PART_ID, &buf_r);
    if (HEARTRATE10_PART_ID != buf_r)
    {
        return HEARTRATE10_ERROR;
    }

    //Set the led mode
    HEARTRATE10_SET_MODE(MAX86916_MODE_LED1_LED2);

    //Set led sequences [ IR, RED ]
    	//RED-IR
    HEARTRATE10_SET_LED_SEQUENCE_1(MAX86916_LED_SEQ_LED1);
    HEARTRATE10_SET_LED_SEQUENCE_2(MAX86916_LED_SEQ_LED2);
    READ(HEARTRATE10_REG_LED_SEQ1, &buf_r);
    	//BLUE-GREEN
    HEARTRATE10_SET_LED_SEQUENCE_3(MAX86916_LED_SEQ_OFF);
    HEARTRATE10_SET_LED_SEQUENCE_4(MAX86916_LED_SEQ_OFF);
    READ(HEARTRATE10_REG_LED_SEQ2, &buf_r);

    //Set scale range, sample per second and led pulse widths
    HEARTRATE10_ADC_RANGE(MAX86916_ADC_RANGE_32768);
    HEARTRATE10_SR(MAX86916_SR_200);
    HEARTRATE10_LED_PULSE_WIDTH(MAX86916_LED_PW_70);
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);

    //Set led range
    HEARTRATE10_LED_RANGE_1(MAX86916_LED_RANGE_50); 		//Current range 0 to 50 mA for the 4 LEDs
    HEARTRATE10_LED_RANGE_2(MAX86916_LED_RANGE_50);
    HEARTRATE10_LED_RANGE_3(MAX86916_LED_RANGE_50);
    HEARTRATE10_LED_RANGE_4(MAX86916_LED_RANGE_50);

    //Set led power
    buf[0]=0xFF; 											//Nominal current pulse amplitude of 50.4 mA for the IR and red LEDs
    HEARTRATE10_LED_POWER_1(buf[0]); 						//IR
    HEARTRATE10_LED_POWER_2(buf[0]); 						//RED
    buf[0]=0x00; 											//Turn off the green and blue LEDs
    HEARTRATE10_LED_POWER_3(buf[0]); 						//GREEN
    HEARTRATE10_LED_POWER_4(buf[0]); 						//BLUE

    //Sample average
    heartrate10_FIFO_SAMPLE_AVERAGE(MAX86916_FIFO_SAMPLE_AVERAGE_1);

    //Enable fifo overflow
    heartrate10_FIFO_RO(MAX86916_FIFO_RO_EN);

    //Number of unread data for interruption
    heartrate10_FIFO_A_FULL(MAX86916_FIFO_A_FULL_20);

    //Enable Int on data read
    heartrate10_SMP_RDY_EN(MAX86916_SMP_RDY_DIS);			//Disable interruption when a new sample is in FIFO
    heartrate10_A_FULL_EN(MAX86916_A_FULL_EN);				//Enable interruption when a watermark level of samples is in FIFO
    heartrate10_ALC_OVF_EN(MAX86916_ALC_OVF_DIS);
    heartrate10_PROX_INT_EN(MAX86916_PROX_INT_DIS);
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);

	READ(HEARTRATE10_REG_INT_STATUS, &buf_r);				//Clean interrupts

    return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_SHUTDOWN_DEVICE(void)
{
	buf[0]=0x80; //0b1000 0000
	SEND(HEARTRATE10_REG_MODE_CFG1, buf);
	HAL_Delay(1000);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_RESET_DEVICE(void)
{
	buf[0]=0x40; //0b0100 0000
	SEND(HEARTRATE10_REG_MODE_CFG1, buf);
	uint8_t reg_data;
	do {
		READ(HEARTRATE10_REG_INT_STATUS, &reg_data);
		reg_data &= 0x01;
	} while ( reg_data );

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_SET_MODE(MAX86916_MODE_TypeDef MODE)
{
    READ(HEARTRATE10_REG_MODE_CFG1, &buf_r);
    buf[0]=MODE|(buf_r&0b00111000);
    SEND(HEARTRATE10_REG_MODE_CFG1, buf);
    READ(HEARTRATE10_REG_MODE_CFG1, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_SET_LED_SEQUENCE_1(MAX86916_LED_SEQ_PILOT_TypeDef SEQ)
{
	READ(HEARTRATE10_REG_LED_SEQ1, &buf_r);
	buf[0]=SEQ|(buf_r&0b11110000);
	SEND(HEARTRATE10_REG_LED_SEQ1, buf);
	READ(HEARTRATE10_REG_LED_SEQ1, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_SET_LED_SEQUENCE_2(MAX86916_LED_SEQ_PILOT_TypeDef SEQ)
{
	READ(HEARTRATE10_REG_LED_SEQ1, &buf_r);
	buf[0]=(SEQ<<4)|(buf_r&0b00001111);
	SEND(HEARTRATE10_REG_LED_SEQ1, buf);
	READ(HEARTRATE10_REG_LED_SEQ1, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_SET_LED_SEQUENCE_3(MAX86916_LED_SEQ_PILOT_TypeDef SEQ)
{
	READ(HEARTRATE10_REG_LED_SEQ2, &buf_r);
	buf[0]=SEQ|(buf_r&0b11110000);
	SEND(HEARTRATE10_REG_LED_SEQ2, buf);
	READ(HEARTRATE10_REG_LED_SEQ2, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_SET_LED_SEQUENCE_4(MAX86916_LED_SEQ_PILOT_TypeDef SEQ)
{
	READ(HEARTRATE10_REG_LED_SEQ2, &buf_r);
	buf[0]=(SEQ<<4)|(buf_r&0b00001111);
	SEND(HEARTRATE10_REG_LED_SEQ2, buf);
	READ(HEARTRATE10_REG_LED_SEQ2, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_ADC_RANGE(MAX86916_ADC_range_TypeDef ADC_RANGE)
{
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);
    buf[0]=(ADC_RANGE<<5)|(buf_r&0b10011111);
    SEND(HEARTRATE10_REG_MODE_CFG2, buf);
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_SR(MAX86916_SR_TypeDef SR)
{
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);
    buf[0]=(SR<<2)|(buf_r&0b11100011);
    SEND(HEARTRATE10_REG_MODE_CFG2, buf);
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_PULSE_WIDTH(MAX86916_LED_PW_TypeDef LED_PW)
{
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);
    buf[0]=LED_PW|(buf_r&0b11111100);
    SEND(HEARTRATE10_REG_MODE_CFG2, buf);
    READ(HEARTRATE10_REG_MODE_CFG2, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_RANGE_1(MAX86916_LED_RANGE_TypeDef LED_RANGE)
{
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);
    buf[0]=LED_RANGE|(buf_r&0b11111100);
    SEND(HEARTRATE10_REG_LED_RANGE, buf);
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_RANGE_2(MAX86916_LED_RANGE_TypeDef LED_RANGE)
{
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);
    buf[0]=(LED_RANGE<<2)|(buf_r&0b11110011);
    SEND(HEARTRATE10_REG_LED_RANGE, buf);
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_RANGE_3(MAX86916_LED_RANGE_TypeDef LED_RANGE)
{
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);
    buf[0]=(LED_RANGE<<4)|(buf_r&0b11001111);
    SEND(HEARTRATE10_REG_LED_RANGE, buf);
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_RANGE_4(MAX86916_LED_RANGE_TypeDef LED_RANGE)
{
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);
    buf[0]=(LED_RANGE<<6)|(buf_r&0b00111111);
    SEND(HEARTRATE10_REG_LED_RANGE, buf);
    READ(HEARTRATE10_REG_LED_RANGE, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_POWER_1(uint8_t LED_PA_1)
{
    buf[0]=LED_PA_1;
    SEND(HEARTRATE10_REG_LED1_PA, buf);
    READ(HEARTRATE10_REG_LED1_PA, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_POWER_2(uint8_t LED_PA_2)
{
    buf[0]=LED_PA_2;
    SEND(HEARTRATE10_REG_LED2_PA, buf);
    READ(HEARTRATE10_REG_LED2_PA, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_POWER_3(uint8_t LED_PA_3)
{
    buf[0]=LED_PA_3;
    SEND(HEARTRATE10_REG_LED3_PA, buf);
    READ(HEARTRATE10_REG_LED3_PA, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t HEARTRATE10_LED_POWER_4(uint8_t LED_PA_4)
{
    buf[0]=LED_PA_4;
    SEND(HEARTRATE10_REG_LED4_PA, buf);
    READ(HEARTRATE10_REG_LED4_PA, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t heartrate10_FIFO_SAMPLE_AVERAGE(MAX86916_FIFO_SAMPLE_AVERAGE_TypeDef SMP_AVE)
{
    READ(HEARTRATE10_REG_FIFO_CFG, &buf_r);
    buf[0]=(SMP_AVE<<5)|(buf_r&0b00011111);
    SEND(HEARTRATE10_REG_FIFO_CFG, buf);
    READ(HEARTRATE10_REG_FIFO_CFG, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t heartrate10_FIFO_RO(MAX86916_FIFO_RO_TypeDef FIFO_RO_EN)
{
    READ(HEARTRATE10_REG_FIFO_CFG, &buf_r);
    buf[0]=(FIFO_RO_EN<<4)|(buf_r&0b11110111);
    SEND(HEARTRATE10_REG_FIFO_CFG, buf);
    READ(HEARTRATE10_REG_FIFO_CFG, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t heartrate10_FIFO_A_FULL(MAX86916_FIFO_A_FULL_TypeDef FIFO_A_FULL)
{
    READ(HEARTRATE10_REG_FIFO_CFG, &buf_r);
    buf[0]=FIFO_A_FULL|(buf_r&0b11110000);
    SEND(HEARTRATE10_REG_FIFO_CFG, buf);
    READ(HEARTRATE10_REG_FIFO_CFG, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t heartrate10_A_FULL_EN(MAX86916_A_FULL_EN_TypeDef A_FULL_EN)
{
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);
    buf[0]=(A_FULL_EN<<7)|(buf_r&0b01111111);
    SEND(HEARTRATE10_REG_INT_ENABLE, buf);
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t heartrate10_SMP_RDY_EN(MAX86916_SMP_RDY_EN_TypeDef SMP_RDY_EN)
{
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);
    buf[0]=(SMP_RDY_EN<<6)|(buf_r&0b10111111);
    SEND(HEARTRATE10_REG_INT_ENABLE, buf);
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t heartrate10_ALC_OVF_EN(MAX86916_ALC_OVF_EN_TypeDef ALC_OVF_EN)
{
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);
    buf[0]=(ALC_OVF_EN<<5)|(buf_r&0b11011111);
    SEND(HEARTRATE10_REG_INT_ENABLE, buf);
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);

	return HEARTRATE10_OK;
}

heartrate10_return_value_t heartrate10_PROX_INT_EN(MAX86916_PROX_INT_EN_TypeDef PROX_INT_EN)
{
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);
    buf[0]=(PROX_INT_EN<<4)|(buf_r&0b11101111);
    SEND(HEARTRATE10_REG_INT_ENABLE, buf);
    READ(HEARTRATE10_REG_INT_ENABLE, &buf_r);

	return HEARTRATE10_OK;
}

HAL_StatusTypeDef HEARTRATE10_FIFO_READ(uint8_t *pData, uint8_t RX_LEN)
{
    return (HAL_I2C_Mem_Read(&I2C, (uint16_t)HEARTRATE10_SET_DEV_ADDR_R, (uint16_t)HEARTRATE10_REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT, pData, RX_LEN, 1000));
}

uint32_t HEARTRATE10_READ_FIFO_SAMPLE(void)
{
    uint32_t sample;
    uint8_t sample_parts[ 3 ] = { 0 };
    HEARTRATE10_FIFO_READ(sample_parts, 3);
    sample = ( uint32_t )sample_parts[ 2 ] | ( ( uint32_t )sample_parts[ 1 ] << 8 ) | ( ( uint32_t )sample_parts[ 0 ] << 16 );
    sample &= 0x0007FFFF; //last 19-bits taken

    return sample;
}

uint8_t HEARTRATE10_NUMBER_AVAILABLE_SAMPLES(void)
{
	uint8_t nb_available_samples;
	READ(HEARTRATE10_REG_FIFO_OVF_CNT,&buf[0]);
	READ(HEARTRATE10_REG_FIFO_WR_PTR,&buf[1]);
	READ(HEARTRATE10_REG_FIFO_RD_PTR,&buf[2]);
	if (buf[0] == 0) //no overflow
	{
		if (buf[1]>buf[2])
		{
			nb_available_samples=buf[1]-buf[2];
		}
		else
		{
			nb_available_samples=buf[1]+32-buf[2];
		}
	}
	else
	{
		nb_available_samples = 32; // // overflow occurred and data has been lost
	}

	return nb_available_samples;
}

HAL_StatusTypeDef HEARTRATE10_READ_COMPLETE_FIFO_DATA(data_4leds_TypeDef *pData)
{
    uint8_t sample_parts[ 12 ] = { 0 };
    fifo_read = HEARTRATE10_FIFO_READ(sample_parts, 12 );
    pData->ir = sample_parts[ 2 ] | ( ( uint32_t )sample_parts[ 1 ] << 8 ) | ( ( uint32_t )sample_parts[ 0 ] << 16 );
    pData->ir &= 0x0007FFFF;
    pData->red = sample_parts[ 5 ] | ( ( uint32_t )sample_parts[ 4 ] << 8 ) | ( ( uint32_t )sample_parts[ 3 ] << 16 );
    pData->red &= 0x0007FFFF;
    pData->green = sample_parts[ 8 ] | ( ( uint32_t )sample_parts[ 7 ] << 8 ) | ( ( uint32_t )sample_parts[ 6 ] << 16 );
    pData->green &= 0x0007FFFF;
    pData->blue = sample_parts[ 11 ] | ( ( uint32_t )sample_parts[ 10 ] << 8 ) | ( ( uint32_t )sample_parts[ 9 ] << 16 );
    pData->blue &= 0x0007FFFF;

    return fifo_read;
}

HAL_StatusTypeDef HEARTRATE10_READ_2LEDS_FIFO_DATA(data_2leds_TypeDef *pData)
{
    uint8_t sample_parts[ 6 ] = { 0 };
    fifo_read = HEARTRATE10_FIFO_READ(sample_parts, 6 );
    pData->ir = sample_parts[ 2 ] | ( ( uint32_t )sample_parts[ 1 ] << 8 ) | ( ( uint32_t )sample_parts[ 0 ] << 16 );
    pData->ir &= 0x0007FFFF;
    pData->red = sample_parts[ 5 ] | ( ( uint32_t )sample_parts[ 4 ] << 8 ) | ( ( uint32_t )sample_parts[ 3 ] << 16 );
    pData->red &= 0x0007FFFF;

    return fifo_read;
}

HAL_StatusTypeDef READ(uint16_t Address, uint8_t *pData){
	return (HAL_I2C_Mem_Read(&I2C, (uint16_t)HEARTRATE10_SET_DEV_ADDR_R, (uint16_t)Address, I2C_MEMADD_SIZE_8BIT, pData, 1, 1000));
}

HAL_StatusTypeDef SEND(uint16_t Address, uint8_t *pData){
	return (HAL_I2C_Mem_Write(&I2C, (uint16_t)HEARTRATE10_SET_DEV_ADDR_W, (uint16_t)Address, I2C_MEMADD_SIZE_8BIT, pData, 1, 1000));
}
