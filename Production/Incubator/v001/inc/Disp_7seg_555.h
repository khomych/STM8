#ifndef __DISP_7SEG_555_H
#define __DISP_7SEG_555_H

#ifndef REGISTER_DS_PORT
#define REGISTER_DS_PORT        GPIOC
#endif

#ifndef REGISTER_ST_PORT
#define REGISTER_ST_PORT        GPIOC
#endif

#ifndef REGISTER_SH_PORT
#define REGISTER_SH_PORT        GPIOC
#endif

#ifndef REGISTER_DS_PIN
#define REGISTER_DS_PIN        GPIO_PIN_5
#endif

#ifndef REGISTER_ST_PIN
#define REGISTER_ST_PIN        GPIO_PIN_6
#endif

#ifndef REGISTER_SH_PIN
#define REGISTER_SH_PIN        GPIO_PIN_7
#endif


#define REGISTER_DS_LOW REGISTER_DS_PORT->ODR &=  (uint16_t)(~REGISTER_DS_PIN)
#define REGISTER_DS_HIGH REGISTER_DS_PORT->ODR |=(uint16_t)(REGISTER_DS_PIN)

#define REGISTER_ST_LOW REGISTER_ST_PORT->ODR &=  (uint16_t)(~REGISTER_ST_PIN)
#define REGISTER_ST_HIGH REGISTER_ST_PORT->ODR |=(uint16_t)(REGISTER_ST_PIN)

#define REGISTER_SH_LOW REGISTER_SH_PORT->ODR &=  (uint16_t)(~REGISTER_SH_PIN)
#define REGISTER_SH_HIGH REGISTER_SH_PORT->ODR |=(uint16_t)(REGISTER_SH_PIN)

#define DISP_LED_0 13
#define DISP_LED_1 14
#define DISP_LED_2 12

#define DISP_AN0 10
#define DISP_AN1 9
#define DISP_AN2 8

#define DISP_DOT 5

typedef enum {OFF = 0, ON = !OFF} LedStatus;
typedef enum {Led0 = DISP_LED_0, Led1 = DISP_LED_1, Led2 = DISP_LED_2}Leds;


void DisplayInit(void);
void Register(uint16_t u16Data);
void DisplayLed(Leds LedN, LedStatus lStatus);
void DisplayTick(void);
void DiaplayCalculate(void);
void DisplaySetDots(uint8_t Dots);

#endif //__DISP_7SEG_555_H