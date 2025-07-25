#ifndef __EMM_V5_H
#define __EMM_V5_H

#include <stdint.h>
#include <stdbool.h>

/**
  * @brief    速度模式控制
  * @param    addr：电机地址
  * @param    dir ：方向，0为CW，其余值为CCW
  * @param    vel ：速度，单位RPM，范围0~5000
  * @param    acc ：加速度，范围0~255，0为直接启动
  * @param    snF ：多机同步标志，true启用，false不启用
  */
void Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF);

/**
  * @brief    位置模式控制
  * @param    addr：电机地址
  * @param    dir ：方向，0为CW，其余为CCW
  * @param    vel ：速度（RPM）
  * @param    acc ：加速度
  * @param    clk ：位置脉冲数
  * @param    raF ：相对/绝对运动标志，true为绝对，false为相对
  * @param    snF ：多机同步标志
  */
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF);

/**
  * @brief    设置单圈回零的零点位置
  * @param    addr：电机地址
  * @param    svF ：是否存储标志，true为存储，false为不存储
  */
void Emm_V5_Origin_Set_O(uint8_t addr, bool svF);

/**
  * @brief    电机使能/关闭控制
  * @param    addr：电机地址
  * @param    state：true为使能，false为关闭
  * @param    snF  ：多机同步标志
  */
void Emm_V5_En_Control(uint8_t addr, bool state, bool snF);

/**
  * @brief    触发回零
  * @param    addr   ：电机地址
  * @param    o_mode ：回零模式（0-3）
  * @param    snF    ：多机同步标志
  */
void Emm_V5_Origin_Trigger_Return(uint8_t addr, uint8_t o_mode, bool snF);

#endif  // __EMM_V5_H
