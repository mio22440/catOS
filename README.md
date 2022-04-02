# feature_edf_sched

## 说明

### 变量
1.CINCLUDE_FILE 包含目录，以SRC_DIR为根目录
2.CINCLUDE_FILE_FLAG 给CINCLUDE_FILE添加了前缀，作为编译时的flag

## 日志
2022/4/3
EDF模块基本完成，但还没想到具体验证方法

2022/4/1
卧槽edf_infinite_call里面没加while(1)

2022/3/31
今天又调试了EDF的功能，增加了宏开关，在tick_handler里面有while(1)时不会进入hard_fault(在写到这里时我反应过来，都在tick_handler里了，中断都关了，都不进行调度了，肯定暴露不了问题啊，这也刚好解释了之前加了while(1)之后pendsv不触发的问题)
->要想继续调试得去掉while(1)或者想办法让任务可以切换(pendsv中断可以触发)
