#include "app_sys_mode.h"

SystemStatus system_status;

void sys_event_init(void)
{
    system_status.show_state = DEFAULT_PAGE;
    system_status.default_page_flag = 0;
    system_status.set_page_flag = 0;
}

//�����¼�
//�¼�������
void event_manager(SystemStatus *status, SystemEvent Event)
{
    switch(Event)
    {
        case ENTER_EVENT: //���������¼�
            if( status->show_state == DEFAULT_PAGE )
            {
                if( status->default_page_flag == 0 )
                {
                    status->show_state = PID_PAGE;
                    status->function_state = SPEED_FUNCTION;
                }
                else if( status->default_page_flag == 1 )
                {
                    status->show_state = DISTANCE_PAGE;
                    status->function_state = DISTANCE_FUNCTION;
                }
            }
            else if( status->show_state == PID_PAGE || status->show_state == DISTANCE_PAGE )
            {
                //��������ģʽ
                status->show_state = SET_PAGE;
            }
            else if( status->show_state == SET_PAGE )
            {
                //�������ģʽ
                status->show_state = PARAMETER_PAGE;                
            }
            break;

        case QUIT_EVENT: //�����˳��¼�
            if( status->show_state == SET_PAGE )
            {
                if( status->function_state == SPEED_FUNCTION )
                    status->show_state = PID_PAGE;
                else if( status->function_state == DISTANCE_FUNCTION )
                    status->show_state = DISTANCE_PAGE;
            }
            else if( status->show_state == PID_PAGE || status->show_state == DISTANCE_PAGE )
            {
                status->show_state = DEFAULT_PAGE;
                status->motor_flag = MOTOR_STATUS_OFF;
                status->function_state = NO_FUNCTION;
            }
            else if( status->show_state == PARAMETER_PAGE )
            {
                status->show_state = SET_PAGE;
            }
            break;

        case MOTOR_EVENT:   //��������¼�

            if(status->motor_flag == MOTOR_STATUS_OFF )
            {
                status->motor_flag = MOTOR_STATUS_ON;
            }
            else
            {
                status->motor_flag = MOTOR_STATUS_OFF;
            }
            break;
        
        case LONG_PRESS_ADD_START_EVENT://���������ӿ�ʼ�¼�
            status->long_press_state = LONG_PRESS_ADD_START;
            break;

        case LONG_PRESS_SUBTRACT_START_EVENT://������������ʼ�¼�
            status->long_press_state = LONG_PRESS_SUBTRACT_START;
            break;
            
        case LONG_PRESS_END_EVENT://�������������¼�
            status->long_press_state = LONG_PRESS_END;
            break;       

        default:break;    
    }
}

//��ȡ��ʾ����״̬
SystemPageShow get_show_state(void)
{
    return system_status.show_state;
}

//����Ĭ�Ͻ����ѡ���־
void set_default_page_flag(int flag)
{
    system_status.default_page_flag = flag;
}
//��ȡĬ�Ͻ����ѡ���־
int get_default_page_flag(void)
{
    return system_status.default_page_flag;
}
//��ȡ���ý����ѡ���־
int get_set_page_flag(void)
{
    return system_status.set_page_flag;
}
//���õ��״̬
void set_motor_status_flag(MotorStatus flag)
{
    system_status.motor_flag = flag;
}
//��ȡ���״̬
MotorStatus get_motor_status_flag(void)
{
    return system_status.motor_flag;
}

//��ȡ����������״̬
LongPressStatus get_long_press_state(void)
{
    return system_status.long_press_state;
}

//��ȡ֮ǰ�Ƕ��ٹ��ܻ��Ƕ��๦��
Function get_functional_mode(void)
{
    return system_status.function_state;
}
//�������ٶ�PID���ܻ��Ǿ���PID����
void set_functional_mode(Function mode)
{
    system_status.function_state = mode;
}