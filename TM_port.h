#ifndef __TM_PORT_H
#define __TM_PORT_H


#ifdef cplusplus
extern "C"{
#endif

/* set the Macro according to your platform  example : 0：PC 1：arm32 */
#define PLATFORM_PC     (0)
#define PLATFORM_ARM32  (~PLATFORM_PC)
#ifndef Time_t
    #if PLATFORM_PC
        #define Time_t  unsigned __int64
    #elif PLATFORM_ARM32
        #define Time_t  unsigned int
    #endif
#endif

/* put your debug output here example : printf */
/* #include <stdio.h> */
#define USE_USER_LOG (0)
#if USE_USER_LOG
#define TM_LOG(format,...)  printf(format,##__VA_ARGS__)
#else
#define TM_LOG(...)  
#endif


#ifdef cplusplus
}
#endif


#endif




