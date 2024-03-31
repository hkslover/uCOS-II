/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            WIN32 Sample Code
*
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE    128
#define  TASK_START_PRIO    5

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK        AppStartTaskStk[TASK_STK_SIZE];
OS_STK        AppStartTaskStk1[TASK_STK_SIZE];

OS_EVENT* my_sem;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
static  void  AppStartTask1(void *p_arg);

#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx(INT8U rx_data);
#endif

/*
*********************************************************************************************************
*                                                _tmain()
*
* Description : This is the standard entry point for C++ WIN32 code.  
* Arguments   : none
*********************************************************************************************************
*/

void main(int argc, char *argv[])
{
	INT8U  err;
	
	
#if 0
	BSP_IntDisAll();                       /* For an embedded target, disable all interrupts until we are ready to accept them */
#endif
	
	OSInit();                              /* Initialize "uC/OS-II, The Real-Time Kernel"                                      */
	my_sem = OSSemCreate(0);
	
	
	OSTaskCreateExt(AppStartTask,
		(void *)0,
		(OS_STK *)&AppStartTaskStk1[TASK_STK_SIZE-1],
		TASK_START_PRIO,
		TASK_START_PRIO,
		(OS_STK *)&AppStartTaskStk1[0],
		TASK_STK_SIZE,
		(void *)0,
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(AppStartTask1,
		(void *)0,
		(OS_STK *)&AppStartTaskStk[TASK_STK_SIZE-1],
		TASK_START_PRIO+1,
		TASK_START_PRIO+1,
		(OS_STK *)&AppStartTaskStk[0],
		TASK_STK_SIZE,
		(void *)0,
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
#if OS_TASK_NAME_SIZE > 11
	OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", &err);
#endif
	
#if OS_TASK_NAME_SIZE > 14
	// 设置系统任务空闲任务的任务名称
	OSTaskNameSet(OS_IDLE_PRIO, (INT8U *)"uC/OS-II Idle", &err);
#endif
	
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
	// 设置系统任务统计任务的任务名称
	OSTaskNameSet(OS_STAT_PRIO, "uC/OS-II Stat", &err);
#endif
	
	OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)                               */
}


void  AppStartTask (void *p_arg)
{
	p_arg = p_arg;
	INT8U err;
#if 0
	BSP_Init();                                  /* For embedded targets, initialize BSP functions                             */
#endif
	
	
#if OS_TASK_STAT_EN > 0
	OSStatInit();                                /* Determine CPU capacity                                                     */
#endif
	
	while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{      
		OS_Printf("开始等待信号量\n");
		OSSemPend(my_sem,0,&err);
		OS_Printf("信号量接收成功\n");/* your code here. Create more tasks, etc.                                    */
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

void  AppStartTask1 (void *p_arg)
{
	p_arg = p_arg;
	
#if 0
	BSP_Init();                                  /* For embedded targets, initialize BSP functions                             */
#endif
	
	
#if OS_TASK_STAT_EN > 0
	OSStatInit();                                /* Determine CPU capacity                                                     */
#endif
	// OS_ENTER_CRITICAL();
	while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{       		
		OS_Printf("马上发送信号量\n");  /* your code here. Create more tasks, etc.                                    */
		OSTimeDlyHMSM(0, 0, 5, 0);
		OSSemPost(my_sem);
		OS_Printf("信号量发送成功\n");
	}
}

