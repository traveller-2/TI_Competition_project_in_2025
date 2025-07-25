#ifndef _APP_SYS_MODE_H_
#define _APP_SYS_MODE_H_


//定义页面
typedef enum {
    DEFAULT_PAGE=0,    // 默认首页
    PID_PAGE,          // 定速页
    DISTANCE_PAGE,     // 定距页
    SET_PAGE,          // 设置页
    PARAMETER_PAGE     // 参数调整页
} SystemPageShow;

//定义功能
typedef enum {
    NO_FUNCTION,             // 无功能
    SPEED_FUNCTION,          // 定速功能
    DISTANCE_FUNCTION,       // 定距功能
} Function;

//定义触发事件
typedef enum {
    IDLE_EVENT=0,       				//空闲
    ENTER_EVENT,        				//进入事件
    QUIT_EVENT,         				//退出事件
    MOTOR_EVENT,        				//电机事件
    LONG_PRESS_ADD_START_EVENT,         //长按加开始事件
    LONG_PRESS_SUBTRACT_START_EVENT,    //长按减开始事件
    LONG_PRESS_END_EVENT,               //长按结束事件
} SystemEvent;

//定义电机状态
typedef enum{
    MOTOR_STATUS_OFF=0,        //电机关
    MOTOR_STATUS_ON,           //电机开
}MotorStatus;

//定义按键长按状态
typedef enum{
    LONG_PRESS_END=0,           //长按停止
    LONG_PRESS_ADD_START,       //长按加
    LONG_PRESS_SUBTRACT_START,  //长按减
}LongPressStatus;

//定义系统相关信息
typedef struct {
SystemPageShow show_state;          //当前界面显示页
LongPressStatus long_press_state;   //当前按键长按状态
int default_page_flag;              //当前首页（默认页）选择的内容
int set_page_flag;                  //当前设置页选择的内容
MotorStatus motor_flag;             //当前电机状态
Function function_state;            //当前功能
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
 