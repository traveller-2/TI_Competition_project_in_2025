#ifndef __EMM_V5_H
#define __EMM_V5_H

#include <stdint.h>
#include <stdbool.h>

/**
  * @brief    �ٶ�ģʽ����
  * @param    addr�������ַ
  * @param    dir ������0ΪCW������ֵΪCCW
  * @param    vel ���ٶȣ���λRPM����Χ0~5000
  * @param    acc �����ٶȣ���Χ0~255��0Ϊֱ������
  * @param    snF �����ͬ����־��true���ã�false������
  */
void Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF);

/**
  * @brief    λ��ģʽ����
  * @param    addr�������ַ
  * @param    dir ������0ΪCW������ΪCCW
  * @param    vel ���ٶȣ�RPM��
  * @param    acc �����ٶ�
  * @param    clk ��λ��������
  * @param    raF �����/�����˶���־��trueΪ���ԣ�falseΪ���
  * @param    snF �����ͬ����־
  */
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF);

/**
  * @brief    ���õ�Ȧ��������λ��
  * @param    addr�������ַ
  * @param    svF ���Ƿ�洢��־��trueΪ�洢��falseΪ���洢
  */
void Emm_V5_Origin_Set_O(uint8_t addr, bool svF);

/**
  * @brief    ���ʹ��/�رտ���
  * @param    addr�������ַ
  * @param    state��trueΪʹ�ܣ�falseΪ�ر�
  * @param    snF  �����ͬ����־
  */
void Emm_V5_En_Control(uint8_t addr, bool state, bool snF);

/**
  * @brief    ��������
  * @param    addr   �������ַ
  * @param    o_mode ������ģʽ��0-3��
  * @param    snF    �����ͬ����־
  */
void Emm_V5_Origin_Trigger_Return(uint8_t addr, uint8_t o_mode, bool snF);

#endif  // __EMM_V5_H
