
CINCLUDE_FILE += catOS/include
CINCLUDE_FILE += catOS/user
CINCLUDE_FILE += catOS/user/bsp/board_led
CINCLUDE_FILE += catOS/user/bsp/iic
CINCLUDE_FILE += catOS/user/bsp/lcd
CINCLUDE_FILE += catOS/user/bsp/uart


CINCLUDE_FILE += catOS/src/components/cat_sys_print
CINCLUDE_FILE += catOS/src/drivers/uart
ifeq (CATOS_USE_CM_BACKTRACE, y)
	CINCLUDE_FILE += catOS/src/third_party/cm_backtrace
endif
CINCLUDE_FILE += catOS/src/schedule
CINCLUDE_FILE += catOS/src/schedule/edf

CINCLUDE_FILE += catOS/Libraries/CMSIS/Include
CINCLUDE_FILE += catOS/Libraries/CMSIS/Device/ST/STM32F1xx/Include
CINCLUDE_FILE += catOS/Libraries/STM32F1xx_HAL_Driver/Inc

CINCLUDE_FILE += catOS/port/arm/stm32f103vet6_hal


subdir-y += bsp

obj-y += app.o
obj-y += main.o
obj-y += stm32f1xx_it.o
obj-y += system_stm32f1xx.o

obj-y += st_vet6_start.o
#obj-y += mx_startup.o
#obj-y += startup_stm32f103xe.o

include $(RULE_DIR)/rules.mk
