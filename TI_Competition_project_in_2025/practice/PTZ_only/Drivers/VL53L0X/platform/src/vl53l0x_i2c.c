#include "vl53l0x_i2c.h"

/*-------------------------- I2C写操作函数 --------------------------*/
/**
  * @brief  写入单个字节到指定寄存器
  * @param  address: 7位I2C设备地址
  * @param  index: 寄存器地址
  * @param  data: 待写入数据
  * @retval HAL状态码（HAL_OK=成功）
  */
u8 VL53L0X_write_byte(u8 address, u8 index, u8 data)
{
    return HAL_I2C_Mem_Write(&hi2c_vl, address, index, 
                           I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

/**
  * @brief  写入16位数据到指定寄存器
  * @param  address: 7位I2C设备地址
  * @param  index: 寄存器地址
  * @param  data: 待写入的16位数据（MSB-first）
  */
u8 VL53L0X_write_word(u8 address, u8 index, u16 data)
{
    uint8_t buffer[2] = {(data >> 8) & 0xFF, data & 0xFF};
    return HAL_I2C_Mem_Write(&hi2c_vl, address, index, 
                           I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
}

/**
  * @brief  写入32位数据到指定寄存器
  * @param  address: 7位I2C设备地址
  * @param  index: 寄存器地址
  * @param  data: 待写入的32位数据（MSB-first）
  */
u8 VL53L0X_write_dword(u8 address, u8 index, u32 data)
{
    uint8_t buffer[4] = {
        (data >> 24) & 0xFF, (data >> 16) & 0xFF,
        (data >> 8) & 0xFF, data & 0xFF
    };
    return HAL_I2C_Mem_Write(&hi2c_vl, address, index, 
                           I2C_MEMADD_SIZE_8BIT, buffer, 4, 100);
}

/**
  * @brief  连续写入多个字节
  * @param  address: 7位I2C设备地址
  * @param  index: 起始寄存器地址
  * @param  pdata: 数据缓冲区指针
  * @param  count: 写入字节数
  */
u8 VL53L0X_write_multi(u8 address, u8 index, u8 *pdata, u16 count)
{
    return HAL_I2C_Mem_Write(&hi2c_vl, address, index, 
                           I2C_MEMADD_SIZE_8BIT, pdata, count, 100);
}

/*-------------------------- I2C读操作函数 --------------------------*/
/**
  * @brief  从寄存器读取单个字节
  * @param  address: 7位I2C设备地址
  * @param  index: 寄存器地址
  * @param  pdata: 数据存储指针
  */
u8 VL53L0X_read_byte(u8 address, u8 index, u8 *pdata)
{
    return HAL_I2C_Mem_Read(&hi2c_vl, address, index, 
                          I2C_MEMADD_SIZE_8BIT, pdata, 1, 100);
}

/**
  * @brief  从寄存器读取16位数据
  * @param  address: 7位I2C设备地址
  * @param  index: 寄存器地址
  * @param  pdata: 数据存储指针（MSB-first）
  */
u8 VL53L0X_read_word(u8 address, u8 index, u16 *pdata)
{
    uint8_t buffer[2];
    u8 status = HAL_I2C_Mem_Read(&hi2c_vl, address, index, 
                               I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
    *pdata = (buffer[0] << 8) | buffer[1];
    return status;
}

/**
  * @brief  从寄存器读取32位数据
  * @param  address: 7位I2C设备地址
  * @param  index: 寄存器地址
  * @param  pdata: 数据存储指针（MSB-first）
  */
u8 VL53L0X_read_dword(u8 address, u8 index, u32 *pdata)
{
    uint8_t buffer[4];
    u8 status = HAL_I2C_Mem_Read(&hi2c_vl, address, index, 
                               I2C_MEMADD_SIZE_8BIT, buffer, 4, 100);
    *pdata = ((uint32_t)buffer[0] << 24) | 
             ((uint32_t)buffer[1] << 16) | 
             ((uint32_t)buffer[2] << 8)  | 
              buffer[3];
    return status;
}

/**
  * @brief  连续读取多个字节
  * @param  address: 7位I2C设备地址
  * @param  index: 起始寄存器地址
  * @param  pdata: 数据缓冲区指针
  * @param  count: 读取字节数
  */
u8 VL53L0X_read_multi(u8 address, u8 index, u8 *pdata, u16 count)
{
    return HAL_I2C_Mem_Read(&hi2c_vl, address, index, 
                          I2C_MEMADD_SIZE_8BIT, pdata, count, 100);
}
