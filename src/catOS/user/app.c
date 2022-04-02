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

#if (USE_EDF_SCHED == 1)
  #include "edf.h"
#endif

__attribute__((section("app_region")))
struct _cat_TCB_t task1;

#if (USE_EDF_SCHED == 1)
edf_task_t task2;
edf_task_t task3;
edf_task_t task4;
#endif

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
    uint32_t status = cat_task_enter_critical();
	  board_led_init();
#if 0
    cat_uart_init();
#ifdef USER_USE_CM_BACKTRACE
    cm_backtrace_init("CmBacktrace", "0.0.0", "0.0.0");
#endif //#ifdef USER_USE_CM_BACKTRACE
#endif //#if 0
		cat_uart_transmit("testing...\r\n", 12);
    CAT_SYS_PRINTF("%d testing CAT_SYS_PRINTF\r\n", 1);
    cat_task_exit_critical(status);
    for(;;)
    {
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
      CAT_SYS_PRINTF("task2 running...\r\n");
}

int task3_flag;
void task3_entry(void *arg)
{
    CAT_SYS_PRINTF("task3 running...\r\n");
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
    task_init(
      "task1_task",
      &task1,
      task1_entry,
      NULL,
      0,
      task1_env,
      sizeof(task1_env),
      SCHED_STRATEGY_PRIO
    );

#if (USE_EDF_SCHED == 1)
    edf_create_task(
      "task2_edf",
      &task2,
      task2_entry,
      NULL,
      task2_env,
      sizeof(task2_env),
      100
    );

    edf_create_task(
      "task3_edf",
      &task3,
      task3_entry,
      NULL,
      task3_env,
      sizeof(task3_env),
      200
    );
#endif

}
