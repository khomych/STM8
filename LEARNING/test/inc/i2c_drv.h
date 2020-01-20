#ifndef _I2C_DRV_H
#define _I2C_DRV_H

//#include <iostm8.h>
//#include <intrinsics.h>

#include "stm8s_conf.h"

#define F_MASTER_MHZ    16UL
#define F_MASTER_HZ     16000000UL

//100 кГц
#define F_I2C_HZ    100000UL


//Результат выполнения операции с i2c
typedef enum {
    I2C_SUCCESS = 0,
    I2C_TIMEOUT,
    I2C_ERROR
} t_i2c_status;

// Инициализация I2C интерфейса                                   
extern void i2c_master_init(unsigned long f_master_hz, unsigned long f_i2c_hz);

// Запись регистра slave-устройства
extern t_i2c_status  i2c_wr_reg(unsigned char address, 
                               unsigned char reg_addr,
                               char * data, unsigned char length);

// Чтение регистра slave-устройства
extern t_i2c_status  i2c_rd_reg(unsigned char address, 
                               unsigned char reg_addr,
                               char * data, 
                               unsigned char length);

#endif