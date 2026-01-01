#ifndef RTT_WRAPPER_H
#define RTT_WRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*在此处引用外部文件：       begin*/	
#include "SEGGER_RTT.h"
/*引用外部文件end*/	

#ifdef __cplusplus
}
#endif

#ifdef LOG_ENABLE

    #ifdef LOG_INFO_ENABLE
        #define LOG_INFO(format,...)   SEGGER_RTT_printf(0, " %s/%s-%d: " RTT_CTRL_TEXT_WHITE format "\n", \
                                                            RTT_CTRL_RESET \
                                                            __FILE__, \
                                                            __FUNCTION__, \
                                                            __LINE__, \
                                                            ##__VA_ARGS__)
    #else
         #define LOG_INFO(format,...) do{}while(0)
    #endif   

    #ifdef LOG_WARNING_ENABLE
        #define LOG_WARNING(format,...) SEGGER_RTT_printf(0," %s/%s-%d: " RTT_CTRL_TEXT_YELLOW format "\n", \
                                                            RTT_CTRL_RESET \
                                                            __FILE__, \
                                                            __FUNCTION__, \
                                                            __LINE__, \
                                                            ##__VA_ARGS__)
    #else
        #define LOG_WARNING(format,...) do{}while(0)
    #endif

    #ifdef LOG_ERROR_ENABLE
        #define LOG_ERROR(format,...)  SEGGER_RTT_printf(0, " %s/%s-%d: " RTT_CTRL_TEXT_RED format "\n", \
                                                            RTT_CTRL_RESET \
                                                            __FILE__, \
                                                            __FUNCTION__, \
                                                            __LINE__, \
                                                            ##__VA_ARGS__)
    #else
        #define LOG_ERROR(format,...) do{}while(0)
    #endif  

#else
    
#define LOG_INFO(format,...) do{}while(0)
#define LOG_WARNING(format,...) do{}while(0)
#define LOG_ERROR(format,...) do{}while(0)

#endif // DEBUG

#endif