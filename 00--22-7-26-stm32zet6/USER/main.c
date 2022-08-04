#include "include.h"

float ch = 0;
void i0()
{
	int i = 0;
	for(i = 0; i < 16; i++)
		if(PFin(i) == 0)
			ch = i;
}

PID v1_pid;
PID v2_pid;
PID v3_pid;
PID v4_pid;
PID yaw_pid;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable ,ENABLE);//禁用JTAG 启用 SWD
	delay_init();
	////   		    
	OLED_Init();
	////
	trans_bt_init(1, 115200);
	trans_bt_init(2, 115200);
	trans_bt_init(3, 115200);
	trans_others_init(4, 115200);
	uart_init(5, 115200, 0, 0, 0, 0, 0);

    mpu__init();
	ccd_init();
	move_init();
	
	ic_init(1);
	ic_init(2);
	ic_init(3);
	ic_init(4);
	
	gpi_init(i0);
	gpo_init();
	PEout(0) = PEout(1) = PEout(2) = PEout(3) = PEout(4) = PEout(5) = PEout(6) = 1;
	
	PID_init(&v1_pid, 0, 0, 0, 1);
	PID_init(&v2_pid, 0, 0, 0, 1);
	PID_init(&v3_pid, 0, 0, 0, 1);
	PID_init(&v4_pid, 0, 0, 0, 1);
	PID_init(&yaw_pid, 0, 0, 0, 1);

	maincycle_ms_init(10);

	while(1)
	{
		;		
	}
}

//***************************************************//
//两轮速度之和
float P_v = 0, I_v = 0, D_v = 0;
float a_v1 = 0, a_v2, a_v3, a_v4 = 0;

float v1 = 0, v2 = 0, v3 = 0, v4 = 0;

//float pwm1 = 0, pwm2 = 0, pwm3 = 0, pwm4 = 0;

//***************************************************//
//偏航角
float P_yaw = 0, I_yaw = 0, D_yaw = 0;
float a_yaw = 0;

//float yaw = 0;

//1，4轮向前量；2，3轮向后量
float vd = 0;

//***************************************************//
enum
{
	RUN,
	TURN
}state = RUN;
float cnt_c = 0, cnt_max = 0;

char work = 0;

float a_vs = 0;
float a_vs_max = 0, a_v_max = 0;
float dicr1 = 0, dicr2 = 0;

int i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;

void Maincycle_Handler()
{
	static int cnt = 0;
	int i = 0;
	
	CPUoccupationRate_Calculatestart();
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	
	cnt = (cnt >= 99) ? 0 : cnt + 1;
	
	mpu_getdata();
	ccd_getdata();
	
    v1 = -ic_getdata(1) + 1;
	v2 = ic_getdata(2) - 1;
	v3 = ic_getdata(3) - 1;
	v4 = -ic_getdata(4) + 1;
	
	if(trans_bt_R(2, &work, 0,0,&P_v, &I_v, &D_v, 0, 0,
								&P_yaw, &I_yaw, &D_yaw, 0, &a_vs_max,
								&dicr1, &dicr2, &a_v_max, 0,0,
								0,0,0,0,0,
								0,0,0,0,0,
								0,0,0,0,0))
	{
		PID_init(&v1_pid, P_v, I_v, D_v, 0);
		PID_init(&v2_pid, P_v, I_v, D_v, 0);
		PID_init(&v3_pid, P_v, I_v, D_v, 0);
		PID_init(&v4_pid, P_v, I_v, D_v, 0);
		PID_init(&yaw_pid, P_yaw, I_yaw, D_yaw, 0);
		if(work == 1)
		{
			straight_set(0);
			while(1);
		}
	}
	
	a_vs = sqrt(dicr1 * dicr1 + dicr2 * dicr2);
	if(a_vs != 0) a_yaw = atan2(-dicr1, dicr2) / 3.1415 * 180;
	
	if(yaw - a_yaw > 180)
		a_yaw += 360;
	else if(a_yaw - yaw > 180)
		a_yaw -= 360;	
		
	vd = PID_calc1(&yaw_pid, yaw, a_yaw);
	
	if(a_vs > a_vs_max) a_vs = a_vs_max;
	else if(a_vs < -a_vs_max) a_vs = -a_vs_max;
	
	a_v1 = a_vs - vd;
	if(a_v1 > a_v_max) a_v1 = a_v_max;
	else if(a_v1 < -a_v_max) a_v1 = -a_v_max;
	a_v4 = a_v1;
	
	a_v2 = a_vs + vd;
	if(a_v2 > a_v_max) a_v2 = a_v_max;
	else if(a_v2 < -a_v_max) a_v2 = -a_v_max; 
	a_v3 = a_v2;
	
	straight_add(PID_calc2(&v1_pid, v1, a_v1), 
				 PID_calc2(&v2_pid, v2, a_v2),  
				 PID_calc2(&v3_pid, v3, a_v3),  
				 PID_calc2(&v4_pid, v4, a_v4) );
	
	spin_add(0, 2800);

	if(trans_others_R(4, &i1, &i2, &i3, &i4, &i5, 0,0,0,0,0))
		;

	if(cnt % 2 == 0)
		trans_bt_T(2,0,0,0, yaw, roll, pitch, CPUoccupationRate_Calculatefinish(), v1,
						    v2, v3, v4, a_v1,
							a_v2,i1,i2,i3,i4,i5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	
	if(cnt == 0) PEout(0) = PEout(1) = PEout(2) = PEout(3) = PEout(4) = PEout(5) = PEout(6) = 1;
	else if(cnt == 50) PEout(0) = PEout(1) = PEout(2) = PEout(3) = PEout(4) = PEout(5) = PEout(6) = 0;
}
