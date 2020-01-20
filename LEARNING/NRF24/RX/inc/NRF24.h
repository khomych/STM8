#ifndef NRF24_H_
#define NRF24_H_
//------------------------------------------------
#include "stm8s_conf.h"
//#include "max7219.h"
#include <main.h>
//------------------------------------------------
/*
#define CS_GPIO_PORT GPIOA
#define CS_PIN GPIO_PIN_4
#define CS_ON HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET)
#define CS_OFF HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET)
#define CE_GPIO_PORT GPIOA
#define CE_PIN GPIO_PIN_3
#define CE_RESET HAL_GPIO_WritePin(CE_GPIO_PORT, CE_PIN, GPIO_PIN_RESET)
#define CE_SET HAL_GPIO_WritePin(CE_GPIO_PORT, CE_PIN, GPIO_PIN_SET)
#define LED_GPIO_PORT GPIOC
#define LED_PIN GPIO_PIN_13
#define LED_ON HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET)
#define LED_TGL HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN)
*/
// Register masks definitions
#define nRF24_MASK_REG_MAP         (uint8_t)0x1F // Mask bits[4:0] for CMD_RREG and CMD_WREG commands
#define nRF24_MASK_CRC             (uint8_t)0x0C // Mask for CRC bits [3:2] in CONFIG register
#define nRF24_MASK_STATUS_IRQ      (uint8_t)0x70 // Mask for all IRQ bits in STATUS register
#define nRF24_MASK_RF_PWR          (uint8_t)0x06 // Mask RF_PWR[2:1] bits in RF_SETUP register
#define nRF24_MASK_RX_P_NO         (uint8_t)0x0E // Mask RX_P_NO[3:1] bits in STATUS register
#define nRF24_MASK_DATARATE        (uint8_t)0x28 // Mask RD_DR_[5,3] bits in RF_SETUP register
#define nRF24_MASK_EN_RX           (uint8_t)0x3F // Mask ERX_P[5:0] bits in EN_RXADDR register
#define nRF24_MASK_RX_PW           (uint8_t)0x3F // Mask [5:0] bits in RX_PW_Px register
#define nRF24_MASK_RETR_ARD        (uint8_t)0xF0 // Mask for ARD[7:4] bits in SETUP_RETR register
#define nRF24_MASK_RETR_ARC        (uint8_t)0x0F // Mask for ARC[3:0] bits in SETUP_RETR register
#define nRF24_MASK_RXFIFO          (uint8_t)0x03 // Mask for RX FIFO status bits [1:0] in FIFO_STATUS register
#define nRF24_MASK_TXFIFO          (uint8_t)0x30 // Mask for TX FIFO status bits [5:4] in FIFO_STATUS register
#define nRF24_MASK_PLOS_CNT        (uint8_t)0xF0 // Mask for PLOS_CNT[7:4] bits in OBSERVE_TX register
#define nRF24_MASK_ARC_CNT         (uint8_t)0x0F // Mask for ARC_CNT[3:0] bits in OBSERVE_TX register

#define nRF24_CMD_R_REGISTER       (uint8_t)0x00 // Register read
#define nRF24_CMD_W_REGISTER       (uint8_t)0x20 // Register write
#define nRF24_CMD_R_RX_PAYLOAD     (uint8_t)0x61 // Read RX payload
#define nRF24_CMD_W_TX_PAYLOAD     (uint8_t)0xA0 // Write TX payload
#define nRF24_CMD_FLUSH_TX         (uint8_t)0xE1 // Flush TX FIFO
#define nRF24_CMD_FLUSH_RX         (uint8_t)0xE2 // Flush RX FIFO
#define nRF24_CMD_REUSE_TX_PL      (uint8_t)0xE3 // Reuse TX payload
#define nRF24_CMD_LOCK_UNLOCK      (uint8_t)0x50 // Lock/unlock exclusive features
#define nRF24_CMD_NOP              (uint8_t)0xFF // No operation (used for reading status register)

// nRF24L0 register definitions
#define nRF24_REG_CONFIG           (uint8_t)0x00 // Configuration register
#define nRF24_REG_EN_AA            (uint8_t)0x01 // Enable "Auto acknowledgment"
#define nRF24_REG_EN_RXADDR        (uint8_t)0x02 // Enable RX addresses
#define nRF24_REG_SETUP_AW         (uint8_t)0x03 // Setup of address widths
#define nRF24_REG_SETUP_RETR       (uint8_t)0x04 // Setup of automatic retransmit
#define nRF24_REG_RF_CH            (uint8_t)0x05 // RF channel
#define nRF24_REG_RF_SETUP         (uint8_t)0x06 // RF setup register
#define nRF24_REG_STATUS           (uint8_t)0x07 // Status register
#define nRF24_REG_OBSERVE_TX       (uint8_t)0x08 // Transmit observe register
#define nRF24_REG_RPD              (uint8_t)0x09 // Received power detector
#define nRF24_REG_RX_ADDR_P0       (uint8_t)0x0A // Receive address data pipe 0
#define nRF24_REG_RX_ADDR_P1       (uint8_t)0x0B // Receive address data pipe 1
#define nRF24_REG_RX_ADDR_P2       (uint8_t)0x0C // Receive address data pipe 2
#define nRF24_REG_RX_ADDR_P3       (uint8_t)0x0D // Receive address data pipe 3
#define nRF24_REG_RX_ADDR_P4       (uint8_t)0x0E // Receive address data pipe 4
#define nRF24_REG_RX_ADDR_P5       (uint8_t)0x0F // Receive address data pipe 5
#define nRF24_REG_TX_ADDR          (uint8_t)0x10 // Transmit address
#define nRF24_REG_RX_PW_P0         (uint8_t)0x11 // Number of bytes in RX payload in data pipe 0
#define nRF24_REG_RX_PW_P1         (uint8_t)0x12 // Number of bytes in RX payload in data pipe 1
#define nRF24_REG_RX_PW_P2         (uint8_t)0x13 // Number of bytes in RX payload in data pipe 2
#define nRF24_REG_RX_PW_P3         (uint8_t)0x14 // Number of bytes in RX payload in data pipe 3
#define nRF24_REG_RX_PW_P4         (uint8_t)0x15 // Number of bytes in RX payload in data pipe 4
#define nRF24_REG_RX_PW_P5         (uint8_t)0x16 // Number of bytes in RX payload in data pipe 5
#define nRF24_REG_FIFO_STATUS      (uint8_t)0x17 // FIFO status register
#define nRF24_REG_DYNPD            (uint8_t)0x1C // Enable dynamic payload length
#define nRF24_REG_FEATURE          (uint8_t)0x1D // Feature register

// Register bits definitions
#define nRF24_CONFIG_PRIM_RX       (uint8_t)0x01 // PRIM_RX bit in CONFIG register
#define nRF24_CONFIG_PWR_UP        (uint8_t)0x02 // PWR_UP bit in CONFIG register
#define nRF24_FLAG_RX_DR           (uint8_t)0x40 // RX_DR bit (data ready RX FIFO interrupt)
#define nRF24_FLAG_TX_DS           (uint8_t)0x20 // TX_DS bit (data sent TX FIFO interrupt)
#define nRF24_FLAG_MAX_RT          (uint8_t)0x10 // MAX_RT bit (maximum number of TX retransmits interrupt)

//------------------------------------------------
#define ACTIVATE 0x50 //
#define RD_RX_PLOAD 0x61 // Define RX payload register address
#define WR_TX_PLOAD 0xA0 // Define TX payload register address
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
//------------------------------------------------
#define CONFIG 0x00 //'Config' register address
#define EN_AA 0x01 //'Enable Auto Acknowledgment' register address
#define EN_RXADDR 0x02 //'Enabled RX addresses' register address
#define SETUP_AW 0x03 //'Setup address width' register address
#define SETUP_RETR 0x04 //'Setup Auto. Retrans' register address
#define RF_CH 0x05 //'RF channel' register address
#define RF_SETUP 0x06 //'RF setup' register address
#define STATUS 0x07 //'Status' register address
#define OBSERVE_TX 0x08 //'Transmit observe' register
#define RX_ADDR_P0 0x0A //'RX address pipe0' register address
#define RX_ADDR_P1 0x0B //'RX address pipe1' register address
#define RX_ADDR_P2 0x0C //'RX address pipe2' register address
#define RX_ADDR_P3 0x0D //'RX address pipe3' register address
#define RX_ADDR_P4 0x0E //'RX address pipe4' register address
#define RX_ADDR_P5 0x0F //'RX address pipe5' register address
#define TX_ADDR 0x10 //'TX address' register address
#define RX_PW_P0 0x11 //'RX payload width, pipe0' register address
#define RX_PW_P1 0x12 //'RX payload width, pipe1' register address
#define RX_PW_P2 0x13 //'RX payload width, pipe2' register address
#define RX_PW_P3 0x14 //'RX payload width, pipe3' register address
#define RX_PW_P4 0x15 //'RX payload width, pipe4' register address
#define RX_PW_P5 0x16 //'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17 //'FIFO Status Register' register address
#define DYNPD 0x1C
#define FEATURE 0x1D
//------------------------------------------------
#define PRIM_RX 0x00 //RX/TX control (1: PRX, 0: PTX)
#define PWR_UP 0x01 //1: POWER UP, 0:POWER DOWN
#define RX_DR 0x40 //Data Ready RX FIFO interrupt
#define TX_DS 0x20 //Data Sent TX FIFO interrupt
#define MAX_RT 0x10 //Maximum number of TX retransmits interrupt
//------------------------------------------------
#define W_REGISTER 0x20 //запись в регистр
//------------------------------------------------


void DelayMilliS(uint32_t micros);
void NRF24_ini(void);
uint8_t NRF24_ReadReg(uint8_t addr);
void NRF24_Read_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes);
uint8_t NRF24L01_Send(uint8_t *pBuf);
void NRF24L01_Receive(void);
void IRQ_Callback(void);
void TIM1_Callback(void);
void NRF24L01_RX_Mode(void);

//------------------------------------------------
#endif /* NRF24_H_ */
