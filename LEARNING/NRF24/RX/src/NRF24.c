#include "NRF24.h"
#include "stdio.h"
//------------------------------------------------


//------------------------------------------------
#define TX_PLOAD_WIDTH 5

#define TX_ADR_WIDTH 3
uint8_t TX_ADDRESS0[TX_ADR_WIDTH] = {0x00,0xb5,0x00};
uint8_t TX_ADDRESS1[TX_ADR_WIDTH] = {0x01,0xb5,0x00};
uint8_t TX_ADDRESS2[TX_ADR_WIDTH] = {0x02,0xb5,0x00};
uint8_t TX_ADDRESS3[TX_ADR_WIDTH] = {0x03,0xb5,0x00};
uint8_t TX_ADDRESS4[TX_ADR_WIDTH] = {0x04,0xb5,0x00};
uint8_t TX_ADDRESS5[TX_ADR_WIDTH] = {0x05,0xb5,0x00};

uint8_t RX_BUF[TX_PLOAD_WIDTH+1] = {0};
volatile uint8_t rx_flag = 0, tx_flag = 0;
uint32_t TIM1_Count=0;
uint16_t cnt1=0;
extern uint8_t buf1[20];
//extern char buff[50];


uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xb3,0xb4,0x01};//{0xb3,0xb4,0x01};
//uint8_t RX_BUF[TX_PLOAD_WIDTH] = {0};

//------------------------------------------------
void DelayMilliS(uint32_t micros)
{
  micros *= (110);
  /* Wait till done */
  while (micros--) ;
}
//--------------------------------------------------


//-----------------------------------------------------------------------------
uint8_t nRF24_LL_RW(uint8_t data) {
  uint8_t ret;
  
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(data);
  while (SPI_GetFlagStatus(SPI_FLAG_BSY)== SET);
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE)== RESET);
  ret = SPI_ReceiveData();
	
	
  return (uint8_t)ret;
}

//-----------------------------------------------------------------------------
uint8_t NRF24_ReadReg(uint8_t addr)
{
	uint8_t value;

	nRF24_CSN_L();
	nRF24_LL_RW(addr & nRF24_MASK_REG_MAP);
	value = nRF24_LL_RW(nRF24_CMD_NOP);
	nRF24_CSN_H();

	return value;
}
//------------------------------------------------
void NRF24_WriteReg(uint8_t addr, uint8_t dt)
{
	nRF24_CSN_L();
        
	if (addr < nRF24_CMD_W_REGISTER) {
		// This is a register access
		nRF24_LL_RW(nRF24_CMD_W_REGISTER | (addr & nRF24_MASK_REG_MAP));
		nRF24_LL_RW(dt);
	} else {
		// This is a single byte command or future command/register
		nRF24_LL_RW(addr);
		if ((addr != nRF24_CMD_FLUSH_TX) && (addr != nRF24_CMD_FLUSH_RX) && \
				(addr != nRF24_CMD_REUSE_TX_PL) && (addr != nRF24_CMD_NOP)) {
			// Send register value
			nRF24_LL_RW(dt);
		}
	}
	nRF24_CSN_H();
}
//------------------------------------------------
void NRF24_ToggleFeatures(void)
{
  uint8_t dt = ACTIVATE;
  nRF24_CSN_L();
  nRF24_LL_RW(dt);
  DelayMilliS(1);
  dt = 0x73;
  nRF24_LL_RW(dt);
  nRF24_CSN_H();
}
//-----------------------------------------------

void NRF24_Read_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  nRF24_CSN_L();
  nRF24_LL_RW(addr);
  while (bytes--) {
		*pBuf++ = nRF24_LL_RW(nRF24_CMD_NOP);
	}
  
  nRF24_CSN_H();
}
//------------------------------------------------
void NRF24_Write_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  addr |= W_REGISTER;//
  nRF24_CSN_L();
  nRF24_LL_RW(addr);//
  DelayMilliS(1);
  while (bytes--)
    {
      nRF24_LL_RW(*pBuf++);
    }
  
  nRF24_CSN_H();
}
//------------------------------------------------
void NRF24_FlushRX(void)
{
  uint8_t dt = FLUSH_RX;
  nRF24_CSN_L();
  nRF24_LL_RW(dt);
  DelayMilliS(1);
  nRF24_CSN_H();
}
//------------------------------------------------
void NRF24_FlushTX(void)
{
  uint8_t dt = FLUSH_TX;
  nRF24_CSN_L();
  nRF24_LL_RW(dt);
  DelayMilliS(1);
  nRF24_CSN_H();
}
//------------------------------------------------
void NRF24L01_RX_Mode(void)
{
  uint8_t regval=0x00;
  regval = NRF24_ReadReg(CONFIG);
  
  regval |= (1<<PWR_UP)|(1<<PRIM_RX);
  NRF24_WriteReg(CONFIG,regval);
  
  NRF24_Write_Buf(TX_ADDR, TX_ADDRESS1, TX_ADR_WIDTH);
  NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS0, TX_ADR_WIDTH);
  NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS1, TX_ADR_WIDTH);
  NRF24_Write_Buf(RX_ADDR_P2, &TX_ADDRESS2[0], 1); //для каналов Pipe2 — Pipe5 мы заносим
  NRF24_Write_Buf(RX_ADDR_P3, &TX_ADDRESS3[0], 1); //               только младший байт 
  NRF24_Write_Buf(RX_ADDR_P4, &TX_ADDRESS4[0], 1); //               регистра, а старшие берутся из 
  NRF24_Write_Buf(RX_ADDR_P5, &TX_ADDRESS5[0], 1); //               адреса канала pipe1.
  

  nRF24_CE_H();
  DelayMilliS(1); 
  // Flush buffers
  NRF24_FlushRX();
  NRF24_FlushTX();
}
//------------------------------------------------
void NRF24L01_TX_Mode()
{
  
  NRF24_Write_Buf(TX_ADDR, TX_ADDRESS1, TX_ADR_WIDTH);
  NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS1, TX_ADR_WIDTH);
  
  //в режиме передатчика TX_ADDR и RX_ADDR_P0 должны содержать в себе адрес передатчика
  nRF24_CE_L();
  // Flush buffers
  NRF24_FlushRX();
  NRF24_FlushTX();
}
//------------------------------------------------
void NRF24_Transmit(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  nRF24_CE_L();
  nRF24_CSN_L();
  NRF24_Write_Buf(addr, pBuf, bytes);
  nRF24_CSN_H();
  nRF24_CE_H();
}
//------------------------------------------------
uint8_t NRF24L01_Send(uint8_t *pBuf)
{
  uint8_t regval=0x00;
  NRF24L01_TX_Mode();
  regval = NRF24_ReadReg(CONFIG);
 
  regval |= (1<<PWR_UP);
  regval &= ~(1<<PRIM_RX);
  NRF24_WriteReg(CONFIG,regval);
  DelayMilliS(1); 
  
  NRF24_Transmit(WR_TX_PLOAD, pBuf, TX_PLOAD_WIDTH);
  nRF24_CE_H();
  DelayMilliS(1); //minimum 10us high pulse (Page 21)
  nRF24_CE_L();
  return 0;
}
//------------------------------------------------
void NRF24L01_Receive(void)
{
  if(rx_flag==1)
  {
		printf("\n\r");
                printf((char *)RX_BUF);
                printf("\n\r");
    rx_flag = 0;
  }
}
//------------------------------------------------

void NRF24_ini(void)
{
  nRF24_CE_L();
  DelayMilliS(5);//5ms
  NRF24_WriteReg(CONFIG, 0x0a); // Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
  DelayMilliS(5);//5ms
  NRF24_WriteReg(EN_AA, 0x3F);     //для передатчика нужно активировать Pipe0
  NRF24_WriteReg(EN_RXADDR, 0x3F); //для передатчика нужно активировать Pipe0
  NRF24_WriteReg(SETUP_AW, 0x01); // Setup address width=3 bytes
  NRF24_WriteReg(SETUP_RETR, 0x5F); //5F // 1500us, 15 retrans
  //NRF24_ToggleFeatures();
  NRF24_WriteReg(FEATURE, 0);
  NRF24_WriteReg(DYNPD, 0);
  NRF24_WriteReg(STATUS, 0x70); //Reset flags for IRQ
  NRF24_WriteReg(RF_CH, 76); // частота 2476 MHz
  NRF24_WriteReg(RF_SETUP, 0x06); //TX_PWR:0dBm, Datarate:1Mbps
  NRF24_WriteReg(RX_PW_P0, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 0
  NRF24_WriteReg(RX_PW_P1, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 1
  NRF24_WriteReg(RX_PW_P2, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 2
  NRF24_WriteReg(RX_PW_P3, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 3
  NRF24_WriteReg(RX_PW_P4, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 4
  NRF24_WriteReg(RX_PW_P5, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 5
  //пока уходим в режим приёмника
  NRF24L01_RX_Mode();

  
}

// Clear any pending IRQ flags
void NRF24_ClearIRQFlags(void) {
  uint8_t reg;

  // Clear RX_DR, TX_DS and MAX_RT bits of the STATUS register
  reg  = NRF24_ReadReg(nRF24_REG_STATUS);
  reg |= nRF24_MASK_STATUS_IRQ;
  NRF24_WriteReg(nRF24_REG_STATUS, reg);
}
//--------------------------------------------------
void IRQ_Callback(void)
{
  uint8_t status=0x01;
  uint8_t pipe;
  DelayMilliS(1);
  status = NRF24_ReadReg(STATUS);//STATUS
  


  if(status & RX_DR)
  {
    //LED_TGL;
    pipe = (status>>1)&0x07;
    printf("\n\rPipe: %d\n\r", pipe);
    NRF24_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);
    
    *(RX_BUF+TX_PLOAD_WIDTH) = pipe;
    NRF24_ClearIRQFlags();
        
    rx_flag = 1;
    Led(1);
    
    
  }
  if((status&TX_DS)) //tx_ds == 0x20
  {
    NRF24_ClearIRQFlags();
    
    printf("\n\rTX_DS\n\r");
    NRF24_FlushTX();
    NRF24L01_RX_Mode();
    tx_flag = 1;
    
    }
  else if((status&MAX_RT))
  {
    NRF24_ClearIRQFlags();

    printf("\n\rMAX_RT\n\r");
    NRF24_FlushTX();
    NRF24L01_RX_Mode();
  }
}
//--------------------------------------------------
void TIM1_Callback(void)
{

    cnt1++;
    //memcpy(buf1,(uint8_t*)&cnt1,2);
    NRF24L01_Send(buf1);
    //ClearR_7219();
    //Number_7219(*(uint16_t*)buf1);
    if(cnt1>=9999) cnt1=0;
    //HAL_Delay(1);


}
//--------------------------------------------------



