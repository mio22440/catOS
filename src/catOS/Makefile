
ifeq ($(CATOS_USE_EDF_SCHED),y)
CFLAGS += -DUSE_EDF_SCHED=1
endif

subdir-y += port
subdir-y += src
subdir-y += user
subdir-y += Libraries

#VERIFY_MSG = catOS:$(RELATIVE_PATH)


include $(RULE_DIR)/rules.mk