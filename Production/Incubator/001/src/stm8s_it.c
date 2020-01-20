#include "stm8s_conf.h"
#include "stm8s_it.h"

#include "main.h" //??????????????????????wtf!!

#include "stdio.h"
#include "string.h"


uint16_t uiCounter_1S;
uint16_t uiCounter_100mS;

extern volatile bool b_1S_Flag;
extern volatile bool b_100mS_Flag;

extern volatile uint32_t u32Millis; //
extern volatile bool b_Key_Scan_Flag;

extern uint8_t u8RxCounter;
extern uint8_t u8TxCounter;

extern bool bPacketReceive;

extern uint8_t GoodStartMarker[4];


uint8_t u8TmpCounter = 0;


__IO uint8_t Tx_Idx = 0;
__IO uint16_t Event = 0x00;

uint8_t TxBuff[2] = {0x00, 0x0f};
uint32_t temp = 366;




#ifndef TRAP_IRQ
//TRAP Interrupt routine
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
	while (1){};  
}
#endif

#ifndef TLI_IRQ
//Top Level Interrupt routine.
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
	while (1){};
}
#endif

#ifndef AWU_IRQ 
//Auto Wake Up Interrupt routine.
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
	while (1){};
}
#endif

#ifndef CLK_IRQ 
//Clock Controller Interrupt routine.
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
	while (1){};
}
#endif

#ifndef EXTI_PORTA_IRQ 
//External Interrupt PORTA Interrupt routine.
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
	while (1){};
}
#endif

#ifndef EXTI_PORTB_IRQ 
//External Interrupt PORTB Interrupt routine.
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
	while (1){};
}
#endif

#ifndef EXTI_PORTC_IRQ 
//External Interrupt PORTC Interrupt routine.
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
	while (1){};
}
#endif

#ifndef EXTI_PORTD_IRQ 
//External Interrupt PORTD Interrupt routine.
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
	while (1){};
}
#endif

#ifndef EXTI_PORTE_IRQ 
//External Interrupt PORTE Interrupt routine.
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
	while (1){};
}
#endif


#ifndef SPI_IRQ
//SPI Interrupt routine.
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
	while (1){};
}
#endif

#ifndef TIM1_UPD_OVF_TRG_BRK_IRQ 
//Timer1 Update/Overflow/Trigger/Break Interrupt routine.
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
	while (1){};
}
#endif

#ifndef TIM1_CAP_COM_IRQ 
//Timer1 Capture/Compare Interrupt routine.
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
	while (1){};
}
#endif

#ifndef TIM2_UPD_OVF_BRK_IRQ
//Timer2 Update/Overflow/Break Interrupt routine.
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
	while (1){};
}
#endif

#ifndef TIM2_CAP_COM_IRQ 
//Timer2 Capture/Compare Interrupt routine.
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
	while (1){};
}
#endif

#ifndef UART1_TX_IRQ 
//UART1 TX Interrupt routine.
INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
	while (1){};

}
#endif

#ifndef UART1_RX_IRQ 
//UART1 RX Interrupt routine.
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
	while (1){};

}
#endif

#ifndef I2C_IRQ
//I2C Interrupt routine.
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  uint8_t u8RxData = 0;
  uint16_t res;
  temp = 4680;
  
  TxBuff[0] = (uint8_t)(temp / 1600);
  TxBuff[1] = ((temp % 1600)*16)/100;
  
  //TxBuff[1] = TxBuff[1] & 0xFC; // clear b1 and b0
  

  
  //TxBuff[0] = 5;
  //TxBuff[1] = 0xA0;
  
	/* Read SR2 register to get I2C error */
  if ((I2C->SR2) != 0)
  {
    /* Clears SR2 register */
    I2C->SR2 = 0;

  }
  Event = I2C_GetLastEvent();
  switch (Event)
  {
      /******* Slave transmitter ******/
      /* check on EV1 */
    case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
      //Tx_Idx = 0;
      
      break;

      /* check on EV3 */
    case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
      /* Transmit data */
      I2C_SendData(TxBuff[Tx_Idx]);
      Tx_Idx++;
      
      break;
      /******* Slave receiver **********/
      /* check on EV1*/
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
      break;

      /* Check on EV2*/
    case I2C_EVENT_SLAVE_BYTE_RECEIVED:
      u8RxData = I2C_ReceiveData();
      if(u8RxData == 0x05)
      {
        Tx_Idx = 0;
      }
     
      break;

      /* Check on EV4 */
    case (I2C_EVENT_SLAVE_STOP_DETECTED):
            /* write to CR2 to clear STOPF flag */
            I2C->CR2 |= I2C_CR2_ACK;
            
      break;

    default:
      break;
  }
}
#endif


#ifndef ADC1_IRQ 
//ADC1 interrupt routine.
INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
	while (1){};
}
#endif

#ifndef TIM4_UPD_OVF_IRQ
//Timer4 Update/Overflow Interrupt routine.
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{

  u32Millis++;
  uiCounter_1S++;
  uiCounter_100mS++;
  
  if(uiCounter_1S >= 1000)
  {
    uiCounter_1S = 0;
    b_1S_Flag = TRUE;
  }

  if(uiCounter_100mS >=100)
  {
    uiCounter_100mS = 0;
    b_100mS_Flag = TRUE;
  }



  TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
  
  //while (1){};
}
#endif


#ifndef EEPROM_EEC_IRQ 
//Eeprom EEC Interrupt routine.
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
	while (1){};
}
#endif