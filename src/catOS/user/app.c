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
struct _cat_TCB_t task2;

#if (USE_EDF_SCHED == 1)
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

void task2_entry(void *arg)
{
    for(;;)
    {
        cat_task_delay(100);
        cat_task_delay(100);
    }
}


int task3_flag;
void task3_entry(void *arg)
{
      CAT_SYS_PRINTF("task3 running...\r\n");
}

int task4_flag;
void task4_entry(void *arg)
{
    CAT_SYS_PRINTF("task4 running...\r\n");
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

    task_init(
      "task2_task",
      &task2,
      task2_entry,
      NULL,
      0,
      task2_env,
      sizeof(task2_env),
      SCHED_STRATEGY_PRIO
    );

#if (USE_EDF_SCHED == 1)
    edf_create_task(
      "task3_edf",
      &task3,
      task3_entry,
      NULL,
      task3_env,
      sizeof(task3_env),
      100
    );

    edf_create_task(
      "task4_edf",
      &task4,
      task4_entry,
      NULL,
      task4_env,
      sizeof(task4_env),
      200
    );
#endif

}
