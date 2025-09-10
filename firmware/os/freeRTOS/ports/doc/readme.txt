移植建議

--------------------------------------------------------------------------------
portmacro.h
這個文件決定了freeRTOS該如何移植

最重要的，是從portYIELD_FROM_ISR開始，定義的一些函數形式的宏：

#define portYIELD_FROM_ISR()                    vTaskSwitchContext()
該宏在中斷服務程序中調用。
事實上vTaskSwitchContext的實際功能並不是字面含義，僅僅是選擇一個當前最高優先級的線程

#define portYIELD()             __asm__ __volatile__ ( "l.sys 0x0" )
該宏在線程被迫放棄運行權時調用。
為了統一調度方式，採用syscall以便與中斷切換線程一致。

#define xPortStartScheduler vPortRestoreContext
freeRTOS開始運行，就靠這個函數。
在中斷調度的方案中，vPortRestoreContext通常由彙編實現，是中斷返回的部分。
freeRTOS的初始化中，至少創建一個線程，並且初始化程序處理超級用戶模式；
這意味著處於與中斷類似的環境中，利用中斷返回來進入第一個需要運行的線程，由此來開啟OS之門。

#define portDISABLE_INTERRUPTS()        local_irq_disable()
#define portENABLE_INTERRUPTS()         local_irq_enable()
#define portINIT_CRITICAL()  unsigned int __critical_flags
#define portENTER_CRITICAL() __critical_flags = local_irq_disable()
#define portEXIT_CRITICAL() local_irq_restore(__critical_flags)
這幾個與中斷相關的宏，採用arch/mcu定義的函數。
但事實上freeRTOS與arch/mcu相關的部分很少，獨立移植是有可能的，
祇需要把這些相關函數在freeRTOS移植時單獨實現即可。
甚至可以不必調用函數，直接以宏擴展實現。

--------------------------------------------------------------------------------
portasm.S
非要採用彙編實現的移植，在這個文件實現

OS_Int_Switch
中斷的分支，獲取中斷號之後，在此進行現場保護和分支。
通常的做法，應該與Linux類似，註冊中斷入口及處理上下文。
在openrisc系列的實現中，r9被用於傳遞中斷號。l.sys入口處置r9為32

vPortRestoreContext
前述，xPortStartScheduler的實現，就在這裏。
OS_Int_Switch的中斷返回部分

vApplicationIdleHook
vApplicationTickHook
vApplicationStackOverflowHook
這幾個Hook函數，在這裏提供，並且聲明為.weak
這樣，如果外部沒有定義Hook，則使用這裏的空函數

exception_entry
每個異常都使用l.jal跳轉到此，因此會傳入r9作為異常號

--------------------------------------------------------------------------------
port.c
移植的實現，祇要能用c的，都在這裏。

pxPortInitialiseStack
每個任務創建之時，都會調用此函數進行堆棧現場的初始化

vPortEndScheduler
基本上祇調用vTaskDelete(NULL);

--------------------------------------------------------------------------------
相關的trace宏
traceTASK_SWITCHED_IN
traceTASK_SWITCHED_OUT
traceINCREASE_TICK_COUNT
traceLOW_POWER_IDLE_BEGIN
traceLOW_POWER_IDLE_END
traceTASK_PRIORITY_INHERIT
traceBLOCKING_ON_QUEUE_RECEIVE
traceBLOCKING_ON_QUEUE_SEND
traceMOVED_TASK_TO_READY_STATE
tracePOST_MOVED_TASK_TO_READY_STATE
traceTASK_CREATE
traceTASK_CREATE_FAILED
traceTASK_DELETE
traceTASK_DELAY_UNTIL
traceTASK_DELAY
traceTASK_SUSPEND
traceTASK_RESUME
...
上面這些宏，在FreeRTOS.h中有默認定義，都是一些空宏。
如果有需要，可以在portmacro.h中定義某個宏為某個函數調用，這樣就可以在裏面添加打印

