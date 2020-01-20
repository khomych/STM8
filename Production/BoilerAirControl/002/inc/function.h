/**

  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTION_H
#define __FUNCTION_H

//*****************************************************************************
//              BEEPER DEFINES
//*****************************************************************************
#define BEEP_DELAY 30

//                      BEEP_FREQUENCY_1KHZ
//                      BEEP_FREQUENCY_2KHZ
//                      BEEP_FREQUENCY_4KHZ

#define BEEP_FREQUENCY  BEEP_FREQUENCY_2KHZ


//*****************************************************************************
//              SERVO DEFINES
//*****************************************************************************    
#define SERVO_MIN_POS (uint8_t)102
#define SERVO_MAX_POS (uint8_t)204


uint32_t Millis(void);
void Beep(uint8_t status);




#endif /*__FUNCTION_H*/
