
VERIFY_MSG = ***** making catOS src dir *****

CINCLUDE_FILE += catOS/Libraries/CMSIS/Include
CINCLUDE_FILE += catOS/Libraries/CMSIS/Device/ST/STM32F1xx/Include
CINCLUDE_FILE += catOS/Libraries/STM32F1xx_HAL_Driver/Inc

CINCLUDE_FILE += catOS/include
CINCLUDE_FILE += catOS/user
CINCLUDE_FILE += catOS/src/components/cat_shell
CINCLUDE_FILE += catOS/src/components/cat_sys_print

CINCLUDE_FILE += catOS/port/arm/stm32f103vet6_hal

subdir-y += components
subdir-y += drivers
subdir-y += support
subdir-y += third_party

obj-y += cat_event.o
obj-$(CATOS_USE_FLAG_GROUP) += cat_flag_group.o
obj-$(CATOS_USE_HOOKS) += cat_hooks.o
obj-y += cat_idle.o
obj-y += cat_lib.o
obj-y += cat_list.o
obj-$(CATOS_USE_MAILBOX) += cat_mailbox.o
obj-$(CATOS_USE_MEM) += cat_mem.o
obj-$(CATOS_USE_MUTEX) += cat_mutex.o
obj-$(CATOS_USE_SEM) += cat_sem.o
obj-y += cat_task.o
obj-y += cat_time.o
obj-y += cat_timer.o
obj-y += catos.o


include $(RULE_DIR)/rules.mk