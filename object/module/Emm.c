#include "Emm.h"
#include "ti_msp_dl_config.h"
#include "usart.h"

/**
  * @brief    �ٶ�ģʽ
  * @param    addr�������ַ
  * @param    dir ������       ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�       ����Χ0 - 5000RPM
  * @param    acc �����ٶ�     ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    snF �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
  uint8_t cmd[16] = {0};

  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xF6;                       // ������
  cmd[2] =  dir;                        // ����
  cmd[3] =  (uint8_t)(vel >> 8);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[4] =  (uint8_t)(vel >> 0);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[5] =  acc;                        // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6] =  snF;                        // ���ͬ���˶���־
  cmd[7] =  0x6B;                       // У���ֽ�
  
  // ��������
  UART_SendArray(cmd, 8);
}

/**
  * @brief    λ��ģʽ
  * @param    addr�������ַ
  * @param    dir ������        ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�(RPM)   ����Χ0 - 5000RPM
  * @param    acc �����ٶ�      ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    clk ��������      ����Χ0- (2^32 - 1)��
  * @param    raF ����λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  * @param    snF �����ͬ����־ ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF)
{
  uint8_t cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xFD;                      // ������
  cmd[2]  =  dir;                       // ����
  cmd[3]  =  (uint8_t)(vel >> 8);       // �ٶ�(RPM)��8λ�ֽ�
  cmd[4]  =  (uint8_t)(vel >> 0);       // �ٶ�(RPM)��8λ�ֽ� 
  cmd[5]  =  acc;                       // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6]  =  (uint8_t)(clk >> 24);      // ������(bit24 - bit31)
  cmd[7]  =  (uint8_t)(clk >> 16);      // ������(bit16 - bit23)
  cmd[8]  =  (uint8_t)(clk >> 8);       // ������(bit8  - bit15)
  cmd[9]  =  (uint8_t)(clk >> 0);       // ������(bit0  - bit7 )
  cmd[10] =  raF;                       // ��λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  cmd[11] =  snF;                       // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[12] =  0x6B;                      // У���ֽ�
  
  // ��������
  UART_SendArray(cmd, 13);
}

/**
  * @brief    ���õ�Ȧ��������λ��
  * @param    addr  �������ַ
  * @param    svF   ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Set_O(uint8_t addr, bool svF)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x93;                       // ������
  cmd[2] =  0x88;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  UART_SendArray(cmd, 5);
}

/**
  * @brief    ʹ���źſ���
  * @param    addr  �������ַ
  * @param    state ��ʹ��״̬     ��trueΪʹ�ܵ����falseΪ�رյ��
  * @param    snF   �����ͬ����־ ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_En_Control(uint8_t addr, bool state, bool snF)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xF3;                       // ������
  cmd[2] =  0xAB;                       // ������
  cmd[3] =  (uint8_t)state;             // ʹ��״̬
  cmd[4] =  snF;                        // ���ͬ���˶���־
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  UART_SendArray(cmd, 6);
}

/**
  * @brief    ��������
  * @param    addr   �������ַ
  * @param    o_mode ������ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ������㣬2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  * @param    snF   �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Trigger_Return(uint8_t addr, uint8_t o_mode, bool snF)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x9A;                       // ������
  cmd[2] =  o_mode;                     // ����ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ������㣬2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  cmd[3] =  snF;                        // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  UART_SendArray(cmd, 5);
}