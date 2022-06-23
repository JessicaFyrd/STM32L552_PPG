/*
 * Acquisition.h
 *
 *  Created on: Jun 22, 2022
 *      Author: Jessica Fayard
 */

#ifndef INC_ACQUISITION_H_
#define INC_ACQUISITION_H_


//Includes ============================================================================================================================================
#include "main.h"
#include "MAX86916_ppg.h"
#include "Board.h"


//Variables ===========================================================================================================================================
#define BLOCK_SIZE            		20					//Must be the same as the watermark level for interruption
#define LENGTH_DATA 				200					//Must be equal to the sample rate/number of samples for average to obtain 1 seconde of data


//Enumeration types  ==================================================================================================================================
typedef enum
{
   ACQUI_OK = 0,
   ACQUI_ERROR = -1

} acqui_return_value_t;



/*==================================================================================================================================================*/
/*=================================================================Acquisition=======================================================================*/
/*==================================================================================================================================================*/
//Functions  ==========================================================================================================================================
acqui_return_value_t ACQUISITION_BY_BLOCKSIZE(void);


#endif /* INC_ACQUISITION_H_ */