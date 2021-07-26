#include "app.h"
#include "catos.h"
#include "bsp_board_led.h"
#include "cat_uart.h"
#include "oled.h"
#include "bmp.h"

#if (CATOS_ENABLE_SYS_PRINT == 1)
  #include "cat_sys_print.h"
#endif //#if (CATOS_ENABLE_SYS_PRINT == 1)

#ifdef USER_USE_CM_BACKTRACE
#include "cm_backtrace.h"
#endif

__attribute__((section("app_region")))
struct _cat_TCB_t task1;
struct _cat_TCB_t task2;
struct _cat_TCB_t task3;
struct _cat_TCB_t task4;

cat_stack_type_t task1_env[1024];
cat_stack_type_t task2_env[1024];
cat_stack_type_t task3_env[1024];
cat_stack_type_t task4_env[1024];

uint32_t sched_task1_times = 0;
uint32_t sched_task2_times = 0;
uint32_t sched_task3_times = 0;
uint32_t sched_task4_times = 0;


float myusage = 0.0f;

uint8_t uart_buf[128] = "test";
int task1_flag;
void task1_entry(void *arg)
{
	  board_led_init();
#if 0
    cat_uart_init();
#ifdef USER_USE_CM_BACKTRACE
    cm_backtrace_init("CmBacktrace", "0.0.0", "0.0.0");
#endif //#ifdef USER_USE_CM_BACKTRACE
#endif //#if 0
		cat_uart_transmit("testing...\r\n", 12);
    CAT_SYS_PRINTF("%d testing CAT_SYS_PRINTF\r\n", 1);
    for(;;)
    {
			//CAT_SYS_PRINTF("%d\n", task1_flag);
      //CAT_SYS_PRINTF("testing CAT_SYS_PRINTF\r\n");
      //CAT_SYS_PRINTF("hello");
      //CAT_SYS_PUTCHAR('a');
      sched_task1_times++;
      task1_flag = 0;
		  board_led_on();
      cat_task_delay(100);
      task1_flag = 1;
		  board_led_off();
      cat_task_delay(100);
    }
}

int task2_flag;
void task2_entry(void *arg)
{
		//uint8_t t = ' ';
	
    for(;;)
    {
				//cat_bsp_uart_transmit("task2_flag_1->0\n", 16);
        sched_task2_times++;
        task2_flag = 0;
        cat_task_delay(100);
				//cat_bsp_uart_transmit("task2_flag_0->1\n", 16);
        task2_flag = 1;
        cat_task_delay(100);
			
//				OLED_ShowPicture(0,0,128,64,BMP1,1);
//		OLED_Refresh();
//		cat_task_delay(50);
//		OLED_Clear();
//		OLED_ShowChinese(0,0,0,16,1);//?
//		OLED_ShowChinese(18,0,1,16,1);//?
//		OLED_ShowChinese(36,0,2,16,1);//?
//		OLED_ShowChinese(54,0,3,16,1);//?
//		OLED_ShowChinese(72,0,4,16,1);//?
//		OLED_ShowChinese(90,0,5,16,1);//?
//		OLED_ShowChinese(108,0,6,16,1);//?
//		OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
//		OLED_ShowString(20,32,"2014/05/01",16,1);
//		OLED_ShowString(0,48,"ASCII:",16,1);  
//		OLED_ShowString(63,48,"CODE:",16,1);
//		OLED_ShowChar(48,48,t,16,1);//??ASCII??	   
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,48,t,3,16,1);
//		OLED_Refresh();
//		cat_task_delay(50);
//		OLED_Clear();
//		OLED_ShowChinese(0,0,0,16,1);  //16*16 ?
//	  OLED_ShowChinese(16,0,0,24,1); //24*24 ?
//		OLED_ShowChinese(24,20,0,32,1);//32*32 ?
//	  OLED_ShowChinese(64,0,0,64,1); //64*64 ?
//		OLED_Refresh();
//	  cat_task_delay(50);
//  	OLED_Clear();
//		OLED_ShowString(0,0,"ABC",8,1);//6*8 “ABC”
//		OLED_ShowString(0,8,"ABC",12,1);//6*12 “ABC”
//	  OLED_ShowString(0,20,"ABC",16,1);//8*16 “ABC”
//		OLED_ShowString(0,36,"ABC",24,1);//12*24 “ABC”
//	  OLED_Refresh();
//		cat_task_delay(50);
//		OLED_ScrollDisplay(11,4,1);
    }
}

int task3_flag;
void task3_entry(void *arg)
{

    for(;;)
    {
			//cat_bsp_uart_transmit("task3_flag_1->0\n", 16);
      sched_task3_times++;
        task3_flag = 0;
        cat_task_delay(100);
			
			//cat_bsp_uart_transmit("task3_flag_0->1\n", 16);
        task3_flag = 1;
        cat_task_delay(100);
    }
}

int task4_flag;
void task4_entry(void *arg)
{
    for(;;)
    {
				//cat_bsp_uart_transmit("task4_flag_1->0\n", 16);
        sched_task4_times++;
        task4_flag = 0;
        cat_task_delay(100);
			
			//cat_bsp_uart_transmit("task4_flag_0->1\n", 16);
        task4_flag = 1;
        cat_task_delay(100);
    }
}



void cat_user_tasks_init(void)
{
    task_init("task1_task", &task1, task1_entry, NULL, 0, task1_env, sizeof(task1_env));
    //task_init(&task2, task2_entry, NULL, 1, task2_env, sizeof(task2_env));
    //task_init(&task3, task3_entry, NULL, 1, task3_env, sizeof(task3_env));
    //task_init(&task4, task4_entry, NULL, 1, task4_env, sizeof(task4_env));
}
