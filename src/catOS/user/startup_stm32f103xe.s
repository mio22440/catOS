
.equ Stack_Size, 0x00000400

                #AREA    STACK, NOINIT, READWRITE, ALIGN=3
                .section STACK, "aw"
                .balign 8
Stack_Mem:
.org    Stack_Mem + Stack_Size
__initial_sp:
                                                  
# <h> Heap Configuration
#   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
# </h>

.equ Heap_Size, 0x00000200

                #AREA    HEAP, NOINIT, READWRITE, ALIGN=3
                .section HEAP, "aw"
                .balign 8
__heap_base:
Heap_Mem:
.org Heap_Mem + Heap_Size
__heap_limit:

                #PRESERVE8
                #THUMB
                .thumb


# Vector Table Mapped to Address 0 at Reset
                #AREA    RESET, DATA, READONLY
                .section RESET, "a"
                .global  __Vectors
                .global  __Vectors_End
                .global  __Vectors_Size

__Vectors:
                .word     __initial_sp               @ Top of Stack
                .word     Reset_Handler              @ Reset Handler
                .word     NMI_Handler                @ NMI Handler
                .word     HardFault_Handler          @ Hard Fault Handler
                .word     MemManage_Handler          @ MPU Fault Handler
                .word     BusFault_Handler           @ Bus Fault Handler
                .word     UsageFault_Handler         @ Usage Fault Handler
                .word     0                          @ Reserved
                .word     0                          @ Reserved
                .word     0                          @ Reserved
                .word     0                          @ Reserved
                .word     SVC_Handler                @ SVCall Handler
                .word     DebugMon_Handler           @ Debug Monitor Handler
                .word     0                          @ Reserved
                .word     PendSV_Handler             @ PendSV Handler
                .word     SysTick_Handler            @ SysTick Handler

                @ External Interrupts
                .word     WWDG_IRQHandler            @ Window Watchdog
                .word     PVD_IRQHandler             @ PVD through EXTI Line detect
                .word     TAMPER_IRQHandler          @ Tamper
                .word     RTC_IRQHandler             @ RTC
                .word     FLASH_IRQHandler           @ Flash
                .word     RCC_IRQHandler             @ RCC
                .word     EXTI0_IRQHandler           @ EXTI Line 0
                .word     EXTI1_IRQHandler           @ EXTI Line 1
                .word     EXTI2_IRQHandler           @ EXTI Line 2
                .word     EXTI3_IRQHandler           @ EXTI Line 3
                .word     EXTI4_IRQHandler           @ EXTI Line 4
                .word     DMA1_Channel1_IRQHandler   @ DMA1 Channel 1
                .word     DMA1_Channel2_IRQHandler   @ DMA1 Channel 2
                .word     DMA1_Channel3_IRQHandler   @ DMA1 Channel 3
                .word     DMA1_Channel4_IRQHandler   @ DMA1 Channel 4
                .word     DMA1_Channel5_IRQHandler   @ DMA1 Channel 5
                .word     DMA1_Channel6_IRQHandler   @ DMA1 Channel 6
                .word     DMA1_Channel7_IRQHandler   @ DMA1 Channel 7
                .word     ADC1_2_IRQHandler          @ ADC1 & ADC2
                .word     USB_HP_CAN1_TX_IRQHandler  @ USB High Priority or CAN1 TX
                .word     USB_LP_CAN1_RX0_IRQHandler @ USB Low  Priority or CAN1 RX0
                .word     CAN1_RX1_IRQHandler        @ CAN1 RX1
                .word     CAN1_SCE_IRQHandler        @ CAN1 SCE
                .word     EXTI9_5_IRQHandler         @ EXTI Line 9..5
                .word     TIM1_BRK_IRQHandler        @ TIM1 Break
                .word     TIM1_UP_IRQHandler         @ TIM1 Update
                .word     TIM1_TRG_COM_IRQHandler    @ TIM1 Trigger and Commutation
                .word     TIM1_CC_IRQHandler         @ TIM1 Capture Compare
                .word     TIM2_IRQHandler            @ TIM2
                .word     TIM3_IRQHandler            @ TIM3
                .word     TIM4_IRQHandler            @ TIM4
                .word     I2C1_EV_IRQHandler         @ I2C1 Event
                .word     I2C1_ER_IRQHandler         @ I2C1 Error
                .word     I2C2_EV_IRQHandler         @ I2C2 Event
                .word     I2C2_ER_IRQHandler         @ I2C2 Error
                .word     SPI1_IRQHandler            @ SPI1
                .word     SPI2_IRQHandler            @ SPI2
                .word     USART1_IRQHandler          @ USART1
                .word     USART2_IRQHandler          @ USART2
                .word     USART3_IRQHandler          @ USART3
                .word     EXTI15_10_IRQHandler       @ EXTI Line 15..10
                .word     RTC_Alarm_IRQHandler        @ RTC Alarm through EXTI Line
                .word     USBWakeUp_IRQHandler       @ USB Wakeup from suspend
                .word     TIM8_BRK_IRQHandler        @ TIM8 Break
                .word     TIM8_UP_IRQHandler         @ TIM8 Update
                .word     TIM8_TRG_COM_IRQHandler    @ TIM8 Trigger and Commutation
                .word     TIM8_CC_IRQHandler         @ TIM8 Capture Compare
                .word     ADC3_IRQHandler            @ ADC3
                .word     FSMC_IRQHandler            @ FSMC
                .word     SDIO_IRQHandler            @ SDIO
                .word     TIM5_IRQHandler            @ TIM5
                .word     SPI3_IRQHandler            @ SPI3
                .word     UART4_IRQHandler           @ UART4
                .word     UART5_IRQHandler           @ UART5
                .word     TIM6_IRQHandler            @ TIM6
                .word     TIM7_IRQHandler            @ TIM7
                .word     DMA2_Channel1_IRQHandler   @ DMA2 Channel1
                .word     DMA2_Channel2_IRQHandler   @ DMA2 Channel2
                .word     DMA2_Channel3_IRQHandler   @ DMA2 Channel3
                .word     DMA2_Channel4_5_IRQHandler @ DMA2 Channel4 & Channel5
__Vectors_End:

.equ __Vectors_Size, __Vectors_End - __Vectors

                @AREA    |.text|, CODE, READONLY
                .section .text,"ax"
                
# Reset handler
#.type Reset_Handler, "function"
Reset_Handler:
                .global  .weak, Reset_Handler
                .global  __main
                .global  SystemInit
                LDR     R0, =SystemInit
                #BLX     R0               
                mov lr, pc
                mov pc, r0
                LDR     R0, =__main
                BX      R0
                
# Dummy Exception Handlers (infinite loops which can be modified)

#.type NMI_Handler, "function"
                .thumb
@ NMI_Handler:
@                 .global  .weak, NMI_Handler                
@                 B       .
@ HardFault_Handler:
@                 .global  .weak, HardFault_Handler          
@                 B       .
@ MemManage_Handler:
@                 .global  .weak, MemManage_Handler          
@                 B       .
@ BusFault_Handler:
@                 .global  .weak, BusFault_Handler           
@                 B       .
@ UsageFault_Handler:
@                 .global  .weak, UsageFault_Handler         
@                 B       .
@ SVC_Handler:
@                 .global  .weak, SVC_Handler                
@                 B       .
@ DebugMon_Handler:
@                 .global  .weak, DebugMon_Handler           
@                 B       .
@ PendSV_Handler:
@                 .global  .weak, PendSV_Handler             
@                 B       .
@ SysTick_Handler:
@                 .global  .weak, SysTick_Handler            
@                 B       .
                

Default_Handler:

                .global  .weak, WWDG_IRQHandler            
                .global  .weak ,PVD_IRQHandler             
                .global  .weak ,TAMPER_IRQHandler          
                .global  .weak ,RTC_IRQHandler             
                .global  .weak ,FLASH_IRQHandler           
                .global  .weak ,RCC_IRQHandler             
                .global  .weak ,EXTI0_IRQHandler           
                .global  .weak ,EXTI1_IRQHandler           
                .global  .weak ,EXTI2_IRQHandler           
                .global  .weak ,EXTI3_IRQHandler           
                .global  .weak ,EXTI4_IRQHandler           
                .global  .weak ,DMA1_Channel1_IRQHandler   
                .global  .weak ,DMA1_Channel2_IRQHandler   
                .global  .weak ,DMA1_Channel3_IRQHandler   
                .global  .weak ,DMA1_Channel4_IRQHandler   
                .global  .weak ,DMA1_Channel5_IRQHandler   
                .global  .weak ,DMA1_Channel6_IRQHandler   
                .global  .weak ,DMA1_Channel7_IRQHandler   
                .global  .weak ,ADC1_2_IRQHandler          
                .global  .weak ,USB_HP_CAN1_TX_IRQHandler  
                .global  .weak ,USB_LP_CAN1_RX0_IRQHandler 
                .global  .weak ,CAN1_RX1_IRQHandler        
                .global  .weak ,CAN1_SCE_IRQHandler        
                .global  .weak ,EXTI9_5_IRQHandler         
                .global  .weak ,TIM1_BRK_IRQHandler        
                .global  .weak ,TIM1_UP_IRQHandler         
                .global  .weak ,TIM1_TRG_COM_IRQHandler    
                .global  .weak ,TIM1_CC_IRQHandler         
                .global  .weak ,TIM2_IRQHandler            
                .global  .weak ,TIM3_IRQHandler            
                .global  .weak ,TIM4_IRQHandler            
                .global  .weak ,I2C1_EV_IRQHandler         
                .global  .weak ,I2C1_ER_IRQHandler         
                .global  .weak ,I2C2_EV_IRQHandler         
                .global  .weak ,I2C2_ER_IRQHandler         
                .global  .weak ,SPI1_IRQHandler            
                .global  .weak ,SPI2_IRQHandler            
                .global  .weak ,USART1_IRQHandler          
                .global  .weak ,USART2_IRQHandler          
                .global  .weak ,USART3_IRQHandler          
                .global  .weak ,EXTI15_10_IRQHandler       
                .global  .weak ,RTC_Alarm_IRQHandler        
                .global  .weak ,USBWakeUp_IRQHandler       
                .global  .weak ,TIM8_BRK_IRQHandler        
                .global  .weak ,TIM8_UP_IRQHandler         
                .global  .weak ,TIM8_TRG_COM_IRQHandler    
                .global  .weak ,TIM8_CC_IRQHandler         
                .global  .weak ,ADC3_IRQHandler            
                .global  .weak ,FSMC_IRQHandler            
                .global  .weak ,SDIO_IRQHandler            
                .global  .weak ,TIM5_IRQHandler            
                .global  .weak ,SPI3_IRQHandler            
                .global  .weak ,UART4_IRQHandler           
                .global  .weak ,UART5_IRQHandler           
                .global  .weak ,TIM6_IRQHandler            
                .global  .weak ,TIM7_IRQHandler            
                .global  .weak ,DMA2_Channel1_IRQHandler   
                .global  .weak ,DMA2_Channel2_IRQHandler   
                .global  .weak ,DMA2_Channel3_IRQHandler   
                .global  .weak ,DMA2_Channel4_5_IRQHandler 

WWDG_IRQHandler:
PVD_IRQHandler:
TAMPER_IRQHandler:
RTC_IRQHandler:
FLASH_IRQHandler:
RCC_IRQHandler:
EXTI0_IRQHandler:
EXTI1_IRQHandler:
EXTI2_IRQHandler:
EXTI3_IRQHandler:
EXTI4_IRQHandler:
DMA1_Channel1_IRQHandler:
DMA1_Channel2_IRQHandler:
DMA1_Channel3_IRQHandler:
DMA1_Channel4_IRQHandler:
DMA1_Channel5_IRQHandler:
DMA1_Channel6_IRQHandler:
DMA1_Channel7_IRQHandler:
ADC1_2_IRQHandler:
USB_HP_CAN1_TX_IRQHandler:
USB_LP_CAN1_RX0_IRQHandler:
CAN1_RX1_IRQHandler:
CAN1_SCE_IRQHandler:
EXTI9_5_IRQHandler:
TIM1_BRK_IRQHandler:
TIM1_UP_IRQHandler:
TIM1_TRG_COM_IRQHandler:
TIM1_CC_IRQHandler:
TIM2_IRQHandler:
TIM3_IRQHandler:
TIM4_IRQHandler:
I2C1_EV_IRQHandler:
I2C1_ER_IRQHandler:
I2C2_EV_IRQHandler:
I2C2_ER_IRQHandler:
SPI1_IRQHandler:
SPI2_IRQHandler:
USART1_IRQHandler:
USART2_IRQHandler:
USART3_IRQHandler:
EXTI15_10_IRQHandler:
RTC_Alarm_IRQHandler:
USBWakeUp_IRQHandler:
TIM8_BRK_IRQHandler:
TIM8_UP_IRQHandler:
TIM8_TRG_COM_IRQHandler:
TIM8_CC_IRQHandler:
ADC3_IRQHandler:
FSMC_IRQHandler:
SDIO_IRQHandler:
TIM5_IRQHandler:
SPI3_IRQHandler:
UART4_IRQHandler:
UART5_IRQHandler:
TIM6_IRQHandler:
TIM7_IRQHandler:
DMA2_Channel1_IRQHandler:
DMA2_Channel2_IRQHandler:
DMA2_Channel3_IRQHandler:
DMA2_Channel4_5_IRQHandler:
                B       .

                

                .balign

#*******************************************************************************
# User Stack and Heap initialization
#*******************************************************************************
                 #IF      :DEF:__MICROLIB
                 .ifdef __MICROLIB
                
                 .global  __initial_sp
                 .global  __heap_base
                 .global  __heap_limit
                
                 #ELSE
                 .else
                
                 .global  __use_two_region_memory
                 .global  __user_initial_stackheap
                 
__user_initial_stackheap:

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 #ALIGN
                 .align

                 #ENDIF
                 .endif

                 #END
                 .end

#************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*****
