//******************************************************************************
// Драйвер модуля I2C микроконтроллера STM8S003F
//
// Автор: Осипов Андрей
// Дата:  17 июля 2014
// URL:   http://hamlab.net/
//******************************************************************************

/* // example
  unsigned int i = 0;
  //Инициализация I2C в режиме мастер
  i2c_master_init(F_MASTER_HZ, F_I2C_HZ);
  
  char * data = "0123456789ABCDEF";
   
  i2c_wr_reg(0xA0, 0, data, 16);
     
  i=0x05FF;     //~11ms pause for write eeprom page (16 bytes for AT24C04)
  while(i--){}

  i2c_rd_reg(0xA0, 0, data, 255);
*/

#include "i2c_drv.h"
//#include <intrinsics.h>
//#include <iostm8.h>

//Таймаут ожидания события I2C
static unsigned long int i2c_timeout;

//Задать таймаут в микросекундах
//#define set_tmo_us(time)  i2c_timeout = (unsigned long int)(F_MASTER_MHZ * time)

//Задать таймаут в миллисекундах
#define set_tmo_ms(time)  i2c_timeout = (unsigned long int)(F_MASTER_MHZ * time * 1000)

#define tmo               i2c_timeout--

//Ожидание наступления события event
//в течении времени timeout в мс
#define wait_event(event, timeout) set_tmo_ms(timeout);\
                                   while((event) && (--i2c_timeout));\
                                   if(!i2c_timeout) return I2C_TIMEOUT;
                                   
                                   
//******************************************************************************
// Инициализация I2C интерфейса      
//      f_master_hz - частота тактирования периферии Fmaster                                   
//      f_i2c_hz - скорость передачи данных по I2C                                   
//******************************************************************************                                   
void i2c_master_init(unsigned long f_master_hz, unsigned long f_i2c_hz){
  unsigned long int ccr;
  
  GPIOB->DDR &= (uint8_t)(~GPIO_PIN_4);
  GPIOB->DDR &= (uint8_t)(~GPIO_PIN_5);
  
  GPIOB->ODR |= (uint8_t)(GPIO_PIN_4);
  GPIOB->ODR |= (uint8_t)(GPIO_PIN_5);
  
  GPIOB->CR1 &= (uint8_t)(~GPIO_PIN_4);
  GPIOB->CR1 &= (uint8_t)(~GPIO_PIN_5);
  
  GPIOB->CR2 &= (uint8_t)(~GPIO_PIN_4);
  GPIOB->CR2 &= (uint8_t)(~GPIO_PIN_5);
  
  //PB_DDR_bit.DDR4 = 0;
  //PB_DDR_bit.DDR5 = 0;
  //PB_ODR_bit.ODR5 = 1;  //SDA
  //PB_ODR_bit.ODR4 = 1;  //SCL
  
  //PB_CR1_bit.C14 = 0;
  //PB_CR1_bit.C15 = 0;
  
  //PB_CR2_bit.C24 = 0;
  //PB_CR2_bit.C25 = 0;
  

  //Частота тактирования периферии MHz
  //I2C_FREQR_FREQ = 16;
  I2C->FREQR = 16;
  //Отключаем I2C
  //I2C_CR1_PE = 0;
  I2C->CR1 &= (uint8_t)(~I2C_CR1_PE);
  //В стандартном режиме скорость I2C max = 100 кбит/с
  //Выбираем стандартный режим 
  //I2C_CCRH_F_S = 0;
  I2C->CCRH &= (uint8_t)(~I2C_CCRH_FS);
  //I2C->CCRL &= (uint8_t)(~I2C_CCRL_CCR);
  //CCR = Fmaster/2*Fiic= 12MHz/2*100kHz
  ccr = f_master_hz/(2*f_i2c_hz);
  //Set Maximum Rise Time: 1000ns max in Standard Mode
  //= [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
  //= InputClockFrequencyMHz+1
  //I2C_TRISER_TRISE = 12+1;
  I2C->TRISER = 12 + 1;
  //I2C_CCRL = ccr & 0xFF;
  I2C->CCRL = ccr & 0xFF; 
  //I2C_CCRH_CCR = (ccr >> 8) & 0x0F;
  I2C->CCRH = (ccr >> 8) & 0x0F;
  //Включаем I2C
  //I2C_CR1_PE = 1;
  I2C->CR1 |= (uint8_t)(I2C_CR1_PE);
  //Разрешаем подтверждение в конце посылки
  //I2C_CR2_ACK = 1; //I2C_CR2_bit.ACK = 1
  I2C->CR2 |= I2C_CR2_ACK;
}

//******************************************************************************
// Запись регистра slave-устройства
//******************************************************************************                                   
t_i2c_status i2c_wr_reg(unsigned char address, unsigned char reg_addr,
                              char * data, unsigned char length){                                  
                                
  //Ждем освобождения шины I2C
  //wait_event(I2C_SR3_BUSY, 10); //I2C_SR3_bit.BUSY
  wait_event((I2C->SR3&I2C_SR3_BUSY), 10); //I2C_SR3_bit.BUSY
  
  //Генерация СТАРТ-посылки
  //I2C_CR2_START = 1;
  I2C->CR2 |= I2C_CR2_START;
  //Ждем установки бита SB
  //wait_event(!I2C_SR1_SB, 1); //I2C_SR1_bit.SB
  wait_event(!(I2C->SR1&I2C_SR1_SB), 1);

 
  //Записываем в регистр данных адрес ведомого устройства
  //I2C_DR = address & 0xFE;
  I2C->DR = address & 0xFE;
  //Ждем подтверждения передачи адреса
  //wait_event(!I2C_SR1_ADDR, 1);
  wait_event((!(I2C->SR1&I2C_SR1_ADDR)), 1);
  
  //Очистка бита ADDR чтением регистра SR3
  //I2C_SR3;
  I2C->SR3;
  
  
  //Ждем освобождения регистра данных
  //wait_event(!I2C_SR1_TXE, 1);
  wait_event(!(I2C->SR1&I2C_SR1_TXE), 1);
  //Отправляем адрес регистра
  //I2C_DR = reg_addr;
  I2C->DR = reg_addr;
  
  
  //Отправка данных
  while(length--){
    //Ждем освобождения регистра данных
    //wait_event(!I2C_SR1_TXE, 1);
    wait_event(!(I2C->SR1&I2C_SR1_TXE), 1);
    //Отправляем адрес регистра
    //I2C_DR = *data++;
    I2C->DR = *data++;
  }

 
  //Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
  //wait_event(!(I2C_SR1_TXE && I2C_SR1_BTF), 1);
  wait_event(!((I2C->SR1&I2C_SR1_TXE) && (I2C->SR1&I2C_SR1_BTF)), 1);
  
  //Посылаем СТОП-посылку
  //I2C_CR2_STOP = 1;
  I2C->CR2 |= I2C_CR2_STOP;
  //Ждем выполнения условия СТОП
  //wait_event(I2C_CR2_STOP, 1);
  wait_event(I2C->CR2&I2C_CR2_STOP, 1);
  
  return I2C_SUCCESS;
}

//******************************************************************************
// Чтение регистра slave-устройства
// Start -> Slave Addr -> Reg. addr -> Restart -> Slave Addr <- data ... -> Stop 
//******************************************************************************                                   
t_i2c_status i2c_rd_reg(unsigned char address, unsigned char reg_addr,
                              char * data, unsigned char length){
  
  //Ждем освобождения шины I2C
  //wait_event(I2C_SR3_BUSY, 10);
  wait_event(I2C->SR3&I2C_SR3_BUSY, 10);
    
  //Разрешаем подтверждение в конце посылки
  //I2C_CR2_ACK = 1;
  I2C->CR2 |= I2C_CR2_ACK;
  
  //Генерация СТАРТ-посылки
  //I2C_CR2_START = 1;
  I2C->CR2 |= I2C_CR2_START;
  //Ждем установки бита SB
  //wait_event(!I2C_SR1_SB, 1);
  wait_event(!(I2C->SR1&I2C_SR1_SB), 1);
  
  //Записываем в регистр данных адрес ведомого устройства
  //I2C_DR = address & 0xFE;
  I2C->DR = address & 0xFE;
  //Ждем подтверждения передачи адреса
  //wait_event(!I2C_SR1_ADDR, 1);
  wait_event(!(I2C->SR1&I2C_SR1_ADDR), 1);
  //Очистка бита ADDR чтением регистра SR3
  //I2C_SR3;
  (void)I2C->SR3;
  
  //Ждем освобождения регистра данных RD
  //wait_event(!I2C_SR1_TXE, 1);
  wait_event(!(I2C->SR1 & I2C_SR1_TXE), 1);
  
  //Передаем адрес регистра slave-устройства, который хотим прочитать
  //I2C_DR = reg_addr;
  I2C->DR = reg_addr;
  //Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
  //wait_event(!(I2C_SR1_TXE && I2C_SR1_BTF), 1);
  wait_event(!((I2C->SR1 & I2C_SR1_TXE) && (I2C->SR1 & I2C_SR1_BTF)), 1);
  
  //Генерация СТАРТ-посылки (рестарт)
  //I2C_CR2_START = 1;
  I2C->CR2 |= (uint8_t)I2C_CR2_START;
  //Ждем установки бита SB
  //wait_event(!I2C_SR1_SB, 1);
  wait_event(!(I2C->SR1 & I2C_SR1_SB), 1);
  
  //Записываем в регистр данных адрес ведомого устройства и переходим
  //в режим чтения (установкой младшего бита в 1)
  //I2C_DR = address | 0x01;
  I2C->DR = address | 0x01;
  
  //Дальше алгоритм зависит от количества принимаемых байт
  //N=1
  if(length == 1){
    //Запрещаем подтверждение в конце посылки
    //I2C_CR2_ACK = 0;
    I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);
    //Ждем подтверждения передачи адреса
    //wait_event(!I2C_SR1_ADDR, 1);
    wait_event(!(I2C->SR1 & I2C_SR1_ADDR), 1);
    
    //Заплатка из Errata
    __disable_interrupt();
    //Очистка бита ADDR чтением регистра SR3
    //I2C_SR3;
    (void)I2C->SR3;
    
    //Устанавлием бит STOP
    //I2C_CR2_STOP = 1;
    I2C->CR2 |= (uint8_t)I2C_CR2_STOP;
    //Заплатка из Errata
    __enable_interrupt();
    
    //Ждем прихода данных в RD
    //wait_event(!I2C_SR1_RXNE, 1);
    wait_event(!(I2C->SR1 & I2C_SR1_RXNE), 1);
    
    //Читаем принятый байт
    //*data = I2C_DR;
    *data = I2C->DR;
  } 
  //N=2
  else if(length == 2){
    //Бит который разрешает NACK на следующем принятом байте
    //I2C_CR2_POS = 1;
    I2C->CR2 |= I2C_CR2_POS;
    //Ждем подтверждения передачи адреса
    //wait_event(!I2C_SR1_ADDR, 1);
    wait_event(!(I2C->SR1 & I2C_SR1_ADDR), 1);
    //Заплатка из Errata
    __disable_interrupt();
    //Очистка бита ADDR чтением регистра SR3
    //I2C_SR3;
    (void)I2C->SR3;
    //Запрещаем подтверждение в конце посылки
    //I2C_CR2_ACK = 0;
    I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);
    //Заплатка из Errata
    __enable_interrupt();
    //Ждем момента, когда первый байт окажется в DR,
    //а второй в сдвиговом регистре
    //wait_event(!I2C_SR1_BTF, 1);
    wait_event(!(I2C->SR1 &  I2C_SR1_BTF), 1);
    
    //Заплатка из Errata
    __disable_interrupt();
    //Устанавлием бит STOP
    //I2C_CR2_STOP = 1;
    I2C->CR2 |= I2C_CR2_STOP;
    //Читаем принятые байты
    //*data++ = I2C_DR;
    *data = I2C->DR;
    //Заплатка из Errata
    __enable_interrupt();
    //*data = I2C_DR;
    *data = I2C->DR;
  } 
  //N>2
  else if(length > 2){
    //Ждем подтверждения передачи адреса
    //wait_event(!I2C_SR1_ADDR, 1);
    wait_event(!(I2C->SR1&I2C_SR1_ADDR), 1);
    
    //Заплатка из Errata
    __disable_interrupt();
    
    //Очистка бита ADDR чтением регистра SR3
    //I2C_SR3;
    (void)I2C->SR3;
    
    //Заплатка из Errata
    __enable_interrupt();
    
    while((length--) > (3 && tmo)){
      //Ожидаем появления данных в DR и сдвиговом регистре
      //wait_event(!I2C_SR1_BTF, 1);
      wait_event(!(I2C->SR1&I2C_SR1_BTF), 1);
      //Читаем принятый байт из DR
      //*data++ = I2C_DR;
      *data++ = I2C->DR;
    }
    //Время таймаута вышло
    if(!tmo) return I2C_TIMEOUT;
    
    //Осталось принять 3 последних байта
    //Ждем, когда в DR окажется N-2 байт, а в сдвиговом регистре
    //окажется N-1 байт
    //wait_event(!I2C_SR1_BTF, 1);
    wait_event(!(I2C->SR1&I2C_SR1_BTF), 1);
    //Запрещаем подтверждение в конце посылки
    //I2C_CR2_ACK = 0;
    I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);
    //Заплатка из Errata
    __disable_interrupt();
    //Читаем N-2 байт из RD, тем самым позволяя принять в сдвиговый
    //регистр байт N, но теперь в конце приема отправится посылка NACK
    //*data++ = I2C_DR;
    *data++ = I2C->DR;
    //Посылка STOP
    //I2C_CR2_STOP = 1;
    I2C->CR2 |= I2C_CR2_STOP;
    //Читаем N-1 байт
    //*data++ = I2C_DR;
    *data++ = I2C->DR;
    //Заплатка из Errata
    __enable_interrupt();
    //Ждем, когда N-й байт попадет в DR из сдвигового регистра
    //wait_event(!I2C_SR1_RXNE, 1);
    wait_event(!(I2C->SR1&I2C_SR1_RXNE), 1);
    //Читаем N байт
    //*data++ = I2C_DR;
    *data++ = I2C->DR;
  }
  
  //Ждем отправки СТОП посылки
  //wait_event(I2C_CR2_STOP, 1);
  wait_event(I2C->CR2&I2C_CR2_STOP, 1);
  //Сбрасывает бит POS, если вдруг он был установлен
  //I2C_CR2_POS = 0;
  I2C->CR2 &= (uint8_t)(~I2C_CR2_POS);
  
  return I2C_SUCCESS;
}
