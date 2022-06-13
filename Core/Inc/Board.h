/*
 * Board.h
 *
 *  Created on: 13 Jun 2022
 *      Author: Jessica Fayard
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_



//Includes ============================================================================================================================================
#include "main.h"
#include "MAX86916_ppg.h"


//Enumeration types  ==================================================================================================================================
typedef enum
{
   BOARD_OK = 0,
   BOARD_ERROR = -1

} board_return_value_t;




/*==================================================================================================================================================*/
/*======================================================================Board=======================================================================*/
/*==================================================================================================================================================*/


//Functions  ==========================================================================================================================================
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_ICACHE_Init(void);
static void MX_RTC_Init(void);
board_return_value_t STM_INIT(void);
board_return_value_t SENSOR_2LED_INIT(void);
board_return_value_t SENSOR_4LED_INIT(void);



#endif /* INC_BOARD_H_ */
