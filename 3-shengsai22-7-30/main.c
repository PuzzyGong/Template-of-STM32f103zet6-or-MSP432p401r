#include "include.h"

//***************************************************//
enum{
	CERTAIN_V = 1,
	TRACK = 2
}state = CERTAIN_V;
char CERTAIN_V_or_TRACK = 0;

enum{
	STABLE = 1,
	READY = 2,
	RUN = 3,
	ADJUST = 4
}Task1_state = STABLE;

char work = 0;

float cnt_c = 0;
float cnt_stopline = 0;  char cnt_stopline_flag = 0;
float CPU = 0;

int _1_to_2_flag = 0;
int _2_from_1_flag = 0;
char trans_4 = 0;
//***************************************************//
//于目标线的距离
PID d_pid;

float P_d = 0, I_d = 0, D_d = 0;
float a_d = 0 ;

int d1 = 0, d2 = 0;      float d = 0;

float a_vd = 0;
float a_vd_max = 18;

//***************************************************//
//与前方的距离
PID s_pid;

float P_s = 0, I_s = 0, D_s = 0;
float a_s = 20;

int s1 = 0, s2 = 0;      float s = 0;

float a_vs = 0;
float a_vs_max = 30;

//***************************************************//
//两轮速度
PID v1_pid;
PID v2_pid;

float P_v = 0, I_v = 0, D_v = 0;
float a_v1 = 0, a_v2 = 0,         a_v = 0;

float v1 = 0, v2 = 0;

//float pwmv1 = 0, pwmv2 = 0;

//***************************************************//
float d_ = 0, s_ = 0, v1_ = 0, v2_ = 0;

int main(void)
{
    SysInit();
	delay_init();
	delay_ms(2000);
	
	PID_init(&v1_pid, 100, 80, 0, 1);
	PID_init(&v2_pid, 100, 80, 0, 1);
	PID_init(&d_pid, 0.06, 0, 2, 1);
    PID_init(&s_pid, 3, 0, 0, 1);
	
	
	ic_init();
	
	oled_init("CPU","cnt","T1s","lin",
			  "1t2","2f1","s"," ");

	
	LED_Init();
	//BUZZ_On();
	KEY_Init();
	
	//LED_R_On();
	//LED_G_On();
	//LED_B_On();

	trans_others_init(4, 115200);
    trans_others_init(2, 115200);
	trans_bt_init(3, 115200);
	trans_others_init(1, 115200);
	
	pwm_init();
	
	 /*中断优先级管理，切记定时器中断优先级不能高于外部中断优先级*/
	Interrupt_setPriority(INT_PORT2, 1 << 4);
	Interrupt_setPriority(INT_PORT4, 1 << 4);
	Interrupt_setPriority(INT_PORT5, 1 << 4);
	Interrupt_setPriority(INT_PORT6, 1 << 4);
	Interrupt_setPriority(INT_TA3_0, 1 << 5);

	TimA3_Init(30000 - 1, 48);	
    MAP_Interrupt_enableMaster(); // 开启总中断
	printf("success");

    while (1)
    {	
		;
    }
}


void TA3_0_IRQHandler(void)
{
	static int cnt = 0;
	cnt = (cnt == 9) ? 0 : cnt + 1;
	
	MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
	
	if(KEY2 == 1)
		Task1_state = READY;
	
	if(Task1_state == READY)
		if(cnt_c++ == 20)
			cnt_c = 0, Task1_state = RUN, a_v = 30, _1_to_2_flag = 1;
		
	if(trans_bt_R(3,
		&work,&CERTAIN_V_or_TRACK, 0,
		&P_v, &I_v, &D_v, &a_v ,0,
		&P_d, &I_d, &D_d, &a_d ,&a_vd_max,
		&P_s, &I_s, &D_s, &a_s ,&a_vs_max,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0))
	{
		if(work)
		{
			MAP_Interrupt_disableMaster();
			speed_set(0,0);
			while(1);
		}
		if     (CERTAIN_V_or_TRACK == 0) state = CERTAIN_V;
		else if(CERTAIN_V_or_TRACK == 1) state = TRACK;
		
		PID_init(&v1_pid, P_v, I_v, D_v, 0);
		PID_init(&v2_pid, P_v, I_v, D_v, 0);
		PID_init(&d_pid, P_d, I_d, D_d, 0);
		PID_init(&s_pid, P_s, I_s, D_s, 0);
	}
	
	if(_2_from_1_flag == 1)
		a_v = 30, Task1_state = RUN;
	else if(_2_from_1_flag == 2)
	{
		Task1_state = ADJUST;
	}
	
	if(Task1_state == ADJUST)
		if(cnt_c++ == 20)
		{
			MAP_Interrupt_disableMaster();
			speed_set(0,0); 
			BUZZ_Off();
			while(1);		
		}

	
//***************************************************//
//于目标线的距离
	if(trans_others_R(1, &d1, &d2, 0,0,0,0,0,0,0,0))
		d = d1 - 300;
	  
	a_vd = PID_calc1(&d_pid, d, a_d);
	if(a_vd > a_vd_max)		a_vd = a_vd_max;
	else if(a_vd < -a_vd_max) a_vd = -a_vd_max;	
	
//***************************************************//
	if(d2 != 0 && cnt_stopline_flag == 0)
	{
		cnt_stopline ++, cnt_stopline_flag = 1;
	}
	else if(d2 == 0)
		cnt_stopline_flag = 0;
		
	
	if(cnt_stopline == 2)
		_1_to_2_flag = 2, BUZZ_On();

//***************************************************//
//与前方的距离
	if(trans_others_R(2, &s1, &s2, 0,0,0,0,0,0,0,0)) 
		s = s1 * 100 + s2;
	
	if(Task1_state == ADJUST || CERTAIN_V_or_TRACK == TRACK) 
	{
		a_v = -  PID_calc1(&s_pid, s, a_s);
		if(a_v > a_vs_max)		a_v = a_vs_max;
		else if(a_v < -a_vs_max) a_v = -a_vs_max;	
	}

//***************************************************//	
//两轮速度
	v1 = get_v1(), v2 = get_v2();
	a_v1 = a_v - a_vd;
	a_v2 = a_v + a_vd;
	
	if(cnt_stopline >= 2)
		speed_set(0,0);
	else
		if(Task1_state == RUN || Task1_state == ADJUST) speed_adjust(PID_calc2(&v1_pid, v1, a_v1), PID_calc2(&v2_pid, v2, a_v2));

//***************************************************//
	if(trans_others_R(4,
		&_2_from_1_flag, 0,0,0,0,
		0,0,0,0,0))
		;
	
	trans_bt_T(3,
		state,0,0,
		d, v1, v2, s, cnt_stopline,
		a_v1,a_v2,0,0,0,
		0,0,0,0,0,
		d_, v1_, v2_, s_, 0,
		0,0,0,0,0,
		0,0,0,0,0);
	

	printf("%3d   \n", _1_to_2_flag);
	
	oled_show( CPU, cnt, Task1_state, cnt_stopline,
		  _1_to_2_flag,_2_from_1_flag, s,0);
		  
	CPU = MAP_Timer_A_getCounterValue(TIMER_A3_BASE) / 300.0;
}
