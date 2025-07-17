#include "vl53l0x_i2c.h"

/*-------------------------- I2Cд�������� --------------------------*/
/**
  * @brief  д�뵥���ֽڵ�ָ���Ĵ���
  * @param  address: 7λI2C�豸��ַ
  * @param  index: �Ĵ�����ַ
  * @param  data: ��д������
  * @retval HAL״̬�루HAL_OK=�ɹ���
  */
u8 VL53L0X_write_byte(u8 address, u8 index, u8 data)
{
    return HAL_I2C_Mem_Write(&hi2c_vl, address, index, 
                           I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

/**
  * @brief  д��16λ���ݵ�ָ���Ĵ���
  * @param  address: 7λI2C�豸��ַ
  * @param  index: �Ĵ�����ַ
  * @param  data: ��д���16λ���ݣ�MSB-first��
  */
u8 VL53L0X_write_word(u8 address, u8 index, u16 data)
{
    uint8_t buffer[2] = {(data >> 8) & 0xFF, data & 0xFF};
    return HAL_I2C_Mem_Write(&hi2c_vl, address, index, 
                           I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
}

/**
  * @brief  д��32λ���ݵ�ָ���Ĵ���
  * @param  address: 7λI2C�豸��ַ
  * @param  index: �Ĵ�����ַ
  * @param  data: ��д���32λ���ݣ�MSB-first��
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
  * @brief  ����д�����ֽ�
  * @param  address: 7λI2C�豸��ַ
  * @param  index: ��ʼ�Ĵ�����ַ
  * @param  pdata: ���ݻ�����ָ��
  * @param  count: д���ֽ���
  */
u8 VL53L0X_write_multi(u8 address, u8 index, u8 *pdata, u16 count)
{
    return HAL_I2C_Mem_Write(&hi2c_vl, address, index, 
                           I2C_MEMADD_SIZE_8BIT, pdata, count, 100);
}

/*-------------------------- I2C���������� --------------------------*/
/**
  * @brief  �ӼĴ�����ȡ�����ֽ�
  * @param  address: 7λI2C�豸��ַ
  * @param  index: �Ĵ�����ַ
  * @param  pdata: ���ݴ洢ָ��
  */
u8 VL53L0X_read_byte(u8 address, u8 index, u8 *pdata)
{
    return HAL_I2C_Mem_Read(&hi2c_vl, address, index, 
                          I2C_MEMADD_SIZE_8BIT, pdata, 1, 100);
}

/**
  * @brief  �ӼĴ�����ȡ16λ����
  * @param  address: 7λI2C�豸��ַ
  * @param  index: �Ĵ�����ַ
  * @param  pdata: ���ݴ洢ָ�루MSB-first��
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
  * @brief  �ӼĴ�����ȡ32λ����
  * @param  address: 7λI2C�豸��ַ
  * @param  index: �Ĵ�����ַ
  * @param  pdata: ���ݴ洢ָ�루MSB-first��
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
  * @brief  ������ȡ����ֽ�
  * @param  address: 7λI2C�豸��ַ
  * @param  index: ��ʼ�Ĵ�����ַ
  * @param  pdata: ���ݻ�����ָ��
  * @param  count: ��ȡ�ֽ���
  */
u8 VL53L0X_read_multi(u8 address, u8 index, u8 *pdata, u16 count)
{
    return HAL_I2C_Mem_Read(&hi2c_vl, address, index, 
                          I2C_MEMADD_SIZE_8BIT, pdata, count, 100);
}
