
#VERIFY_MSG = catOS/port/arm/stm32f103vet6_hal:$(RELATIVE_PATH)

CINCLUDE_FILE += catOS/Libraries/CMSIS/Include
CINCLUDE_FILE += catOS/Libraries/CMSIS/Device/ST/STM32F1xx/Include
CINCLUDE_FILE += catOS/Libraries/STM32F1xx_HAL_Driver/Inc

CINCLUDE_FILE += catOS/include
CINCLUDE_FILE += catOS/user

CINCLUDE_FILE += catOS/src/schedule
CINCLUDE_FILE += catOS/src/schedule/edf



obj-y += port_asm.o
obj-y += port.o

include $(RULE_DIR)/rules.mk
