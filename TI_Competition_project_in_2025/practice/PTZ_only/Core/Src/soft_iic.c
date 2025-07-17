#include "soft_iic.h"
#include "cmsis_os.h"  // ʹ�� osDelay()

#define IIC_DELAY_US 5  // ͨ���ٶ�΢����3~10us ֮���ȶ�

// ΢�뼶��ʱ������ 168MHz ��Ƶ��
static void IIC_Delay_US(uint16_t us)
{
    uint32_t count = us * (168 / 4);  // ���ƹ���
    while (count--) __NOP();         // �ղ���ָ����ʱ
}


static void IIC_Delay(void)
{
    IIC_Delay_US(IIC_DELAY_US);
}

// ��ʼ�� IIC ����
void SoftIIC_Init(void)
{
    IIC_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = IIC_SDA_GPIO_PIN | IIC_SCL_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStruct);

    SDA_HIGH();
    SCL_HIGH();
}

// ���� SDA ����
static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = IIC_SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStruct);
}

// ���� SDA ���
static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = IIC_SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStruct);
}

// Start
void SoftIIC_Start(void)
{
    SDA_OUT();
    SDA_HIGH();
    SCL_HIGH();
    IIC_Delay();
    SDA_LOW();
    IIC_Delay();
    SCL_LOW();
}

// Stop
void SoftIIC_Stop(void)
{
    SDA_OUT();
    SCL_LOW();
    SDA_LOW();
    IIC_Delay();
    SCL_HIGH();
    SDA_HIGH();
    IIC_Delay();
}

// Send byte
void SoftIIC_SendByte(uint8_t data)
{
    SDA_OUT();
    for (uint8_t i = 0; i < 8; i++)
		{
				if (data & 0x80)
						SDA_HIGH();
				else
						SDA_LOW();
				IIC_Delay();          // <-- ����
				data <<= 1;
				SCL_HIGH();
				IIC_Delay();
				SCL_LOW();
				IIC_Delay();
		}
}

// Read byte
uint8_t SoftIIC_ReadByte(uint8_t ack)
{
    uint8_t data = 0;
    SDA_IN();
    for (uint8_t i = 0; i < 8; i++)
		{
				SCL_LOW();
				IIC_Delay();
				SCL_HIGH();
				IIC_Delay();       // <-- ����
				data <<= 1;
				if (READ_SDA())
						data++;
				IIC_Delay();
		}
    if (ack)
        SoftIIC_Ack();
    else
        SoftIIC_NAck();
    return data;
}

// Wait for ACK
uint8_t SoftIIC_WaitAck(void)
{
    uint8_t timeout = 0;
    SDA_IN();
    SDA_HIGH();
    IIC_Delay();
    SCL_HIGH();
    IIC_Delay();
    while (READ_SDA())
    {
        timeout++;
        if (timeout > 250)
        {
            SoftIIC_Stop();
            return 1;
        }
    }
    SCL_LOW();
    return 0;
}

// Send ACK
void SoftIIC_Ack(void)
{
    SCL_LOW();
    SDA_OUT();
    SDA_LOW();
    IIC_Delay();
    SCL_HIGH();
    IIC_Delay();
    SCL_LOW();
}

// Send NACK
void SoftIIC_NAck(void)
{
    SCL_LOW();
    SDA_OUT();
    SDA_HIGH();
    IIC_Delay();
    SCL_HIGH();
    IIC_Delay();
    SCL_LOW();
}
