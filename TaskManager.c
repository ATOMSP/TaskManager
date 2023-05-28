#include "TaskManager.h"


/**
 * @brief Task head
*/
static TIBH_Node node;

static inline Time_t getTimeDif(Time_t now, Time_t last) {
    if ((int)((int)now - (int)last) >= 0) {
        return (now - last);
    }
    else {
        return ((TIME_CNT_MAX - last + 1) + now);
    }
}

inline int getTaskCnt(TIBH_Node * node){
    return node->TaskCnt;
}


/**
 * @brief Get the point to task head
 * @param  None
 * @return Point to task head
*/
TIBH_Node* GetHead(void) {
    return &node;
}


/**
 * @brief Init the point to task head
 * @param  None
*/
static void TaskManagerInit(void)
{
    node.TaskCnt = 0;
    node.next = NULL;
    TM_LOG("TM Init successed\r\n");
}


/**
 * @brief Register a new task 
 * @param hook Callback of the new task
 * @param exetime execute task time
*/
void TaskRegister(TaskFuncCallback hook, Time_t exetime, void * usrdat
){
    ExecuteOnce(TaskManagerInit);
    if (!hook){
        TM_LOG("TM TaskRegister don't find hook\r\n");
        return;
    }
    TaskInfoBlock * Tib = (TaskInfoBlock *)malloc(sizeof(TaskInfoBlock));
    if(!Tib){
        TM_LOG("TM TaskRegister create Tib Failed because malloc failed\r\n");
        return;
    }
    Tib->TaskFlag = 0;
    Tib->userdat = usrdat;
    Tib->ExeTime = exetime;
    Tib->hook = hook;
    Tib->LastTime = 0;
    Tib->ErrorTime = 0;
    Tib->next = NULL;
    node.TaskCnt++;
    if (node.next) {
        TaskInfoBlock* ptr = node.next;
        while (ptr->next) {
            ptr = ptr->next;
        }
        ptr->next = Tib;
    }
    else {
        node.next = Tib;
    }
    TM_LOG("TM TaskRegister create Tib succeed\r\n");
}


/**
 * @brief Free all task
 * @param  None
*/
void TaskFree(void)
{
    TaskInfoBlock* task = node.next;
    TaskInfoBlock* ptr = task;
    while (task) {
        ptr = task;
        task = task->next;
        free(ptr);
    }
    node.TaskCnt = 0;
    TM_LOG("TM TaskFree free Tib succeed\r\n");
}

/**
 * @brief Search a node according to func
 * @param  TaskFuncCallback
*/
inline static TaskInfoBlock* SearchPoint(TaskFuncCallback func)
{
    TaskInfoBlock* task = node.next;
    while (task) {
        if(task->hook == func){
          return task;          
        }
        task = task->next;
    }
    return NULL;
}

/**
 * @brief Free a task
 * @param  TaskFuncCallback 
*/
int TaskFreeOne(TaskFuncCallback del)
{
    TaskInfoBlock* task = node.next;
    if(task == NULL){
      return 1;
    }
    while (task->next) {
        if(task->next->hook == del){
          TaskInfoBlock* ptr = task->next;
          task->next = ptr->next;
          free(ptr);
          node.TaskCnt --;
          TM_LOG("TM TaskFree Task free\r\n");  
          return 0;          
        }
        task = task->next;
    }
    TM_LOG("TM TaskFree Task don't find\r\n");  
    return 1;
}
/**
 * @brief set a task suspend
 * @param  TaskFuncCallback 
 * @param  Flag_Enum 
*/
int TaskSetSuspend(TaskFuncCallback sus,Flag_Enum flag)
{
    TaskInfoBlock* task = SearchPoint(sus);
    if(task == NULL){
      TM_LOG("TM TaskSetSuspend Tib don't find\r\n");  
      return 1;
    }
    if(flag == TASK_SUSPEND){
      task->TaskFlag |= flag;
    }
    else if(flag == TASK_CANCEL_SUSPEND){
      task->TaskFlag &= flag;
    }
    else{ 
      TM_LOG("TM TaskSetSuspend Tib Flag_Enum don't know\r\n");  
      return 1;
    }
    TM_LOG("TM TaskSetSuspend Tib set succeed\r\n");
    return 0;
}

/**
 * @brief Task running
 * @param gettime Nowtime
*/
int TaskRunning(Time_t gettime)
{
    if (!node.TaskCnt){
        TM_LOG("TM TaskRunning Tib failed because no tasks\r\n");
        return 1;
    }
    TaskInfoBlock* task = node.next;
    Time_t timedif = 0;
    while(task){
        timedif = getTimeDif(gettime, task->LastTime);
        if(timedif >= task->ExeTime && !(task->TaskFlag & 0x00000001)){
            task->ErrorTime = timedif - task->ExeTime;
            task->LastTime = gettime;
            /* set task Suspend */
            if(task->hook(task)){ return 1;}
        }
        task = task->next;
    }
    TM_LOG("TM TaskRunning Tib succeed\r\n");
    return 0;
}









