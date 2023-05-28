#ifndef __TASK_MANAGE_H
#define __TASK_MANAGE_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

#ifndef NULL
    #define NULL    (void*)(0)
#endif

#include "TM_port.h"

#ifndef TIME_CNT_MAX 
#define TIME_CNT_MAX ((Time_t)~((Time_t)0))
#endif

/* Execute Once no param*/
#define ExecuteOnce(func) \
do{ \
    static unsigned char flag = 1;\
    if(flag){flag = 0; func();}\
}while(0)

typedef enum{
  TASK_SUSPEND =        (int)0x00000001,
  TASK_CANCEL_SUSPEND = (int)0xFFFFFFFE
}Flag_Enum;

typedef struct TaskInfoBlock TIB_t;
typedef int(*TaskFuncCallback)(TIB_t*);

/**
 * @brief task info block
*/
typedef struct TaskInfoBlock{
    Time_t ExeTime;
    Time_t LastTime;
    Time_t ErrorTime;
    unsigned int TaskFlag; /* set task Suspend */
    void* userdat; 
    TaskFuncCallback hook;
    struct TaskInfoBlock * next;
}TaskInfoBlock;


/**
 * @brief task head
*/
typedef struct TaskInfoBlockHeadNode{
    unsigned short TaskCnt;
    struct TaskInfoBlock * next;
}TIBH_Node;


/**
 * @brief get task number
 * @param node  point to task head
 * @return task number
*/ 


/**
 * @brief TaskManager API
*/
TIBH_Node* GetHead(void);
/*void TaskManagerInit();*/
void TaskRegister(TaskFuncCallback hook, Time_t exetime, void * usrdat);
int  TaskRunning(Time_t gettime);
void TaskFree(void);
int TaskFreeOne(TaskFuncCallback del);
int TaskSetSuspend(TaskFuncCallback sus,Flag_Enum flag);


#ifdef __cplusplus
}
#endif 

#endif




































