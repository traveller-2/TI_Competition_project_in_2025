#ifndef _APP_SYS_MODE_H_
#define _APP_SYS_MODE_H_


//����ҳ��
typedef enum {
    DEFAULT_PAGE=0,    // Ĭ����ҳ
    PID_PAGE,          // ����ҳ
    DISTANCE_PAGE,     // ����ҳ
    SET_PAGE,          // ����ҳ
    PARAMETER_PAGE     // ��������ҳ
} SystemPageShow;

//���幦��
typedef enum {
    NO_FUNCTION,             // �޹���
    SPEED_FUNCTION,          // ���ٹ���
    DISTANCE_FUNCTION,       // ���๦��
} Function;

//���崥���¼�
typedef enum {
    IDLE_EVENT=0,       				//����
    ENTER_EVENT,        				//�����¼�
    QUIT_EVENT,         				//�˳��¼�
    MOTOR_EVENT,        				//����¼�
    LONG_PRESS_ADD_START_EVENT,         //�����ӿ�ʼ�¼�
    LONG_PRESS_SUBTRACT_START_EVENT,    //��������ʼ�¼�
    LONG_PRESS_END_EVENT,               //���������¼�
} SystemEvent;

//������״̬
typedef enum{
    MOTOR_STATUS_OFF=0,        //�����
    MOTOR_STATUS_ON,           //�����
}MotorStatus;

//���尴������״̬
typedef enum{
    LONG_PRESS_END=0,           //����ֹͣ
    LONG_PRESS_ADD_START,       //������
    LONG_PRESS_SUBTRACT_START,  //������
}LongPressStatus;

//����ϵͳ�����Ϣ
typedef struct {
SystemPageShow show_state;          //��ǰ������ʾҳ
LongPressStatus long_press_state;   //��ǰ��������״̬
int default_page_flag;              //��ǰ��ҳ��Ĭ��ҳ��ѡ�������
int set_page_flag;                  //��ǰ����ҳѡ�������
MotorStatus motor_flag;             //��ǰ���״̬
Function function_state;            //��ǰ����
} SystemStatus;

extern SystemStatus system_status;

void sys_event_init(void);
void event_manager(SystemStatus *status, SystemEvent Event);
SystemPageShow get_show_state(void);
void set_default_page_flag(int flag);
int get_default_page_flag(void);
int get_set_page_flag(void);
MotorStatus get_motor_status_flag(void);
void set_motor_status_flag(MotorStatus flag);
LongPressStatus get_long_press_state(void);
Function get_functional_mode(void);
void set_functional_mode(Function mode);
#endif
 