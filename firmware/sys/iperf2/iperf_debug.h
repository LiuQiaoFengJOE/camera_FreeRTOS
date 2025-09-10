
#ifndef _IPERF_DEBUG_H_
#define _IPERF_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define IPERF_DBG_ON    1
#define IPERF_WARN_ON    1
#define IPERF_ERR_ON    1
#define IPERF_ABORT_ON    0

#define IPERF_SYSLOG    printf

#define IPERF_ABORT()    abort()

#define IPERF_LOG(flags, fmt, ...)    \
    do {                                \
        if (flags)                         \
            IPERF_SYSLOG(fmt, ##__VA_ARGS__);    \
    } while (0)

#define IPERF_DBG(fmt, ...)    IPERF_LOG(IPERF_DBG_ON, "[iperf] "fmt, ##__VA_ARGS__)
#define IPERF_WARN(fmt, ...)    IPERF_LOG(IPERF_WARN_ON, "[iperf WARN] "fmt, ##__VA_ARGS__)
#define IPERF_ERR(fmt, ...)                                \
    do {                                                    \
        IPERF_LOG(IPERF_ERR_ON, "[iperf ERR] %s():%d, "fmt,    \
               __func__, __LINE__, ##__VA_ARGS__);                    \
        if (IPERF_ABORT_ON)                                    \
            IPERF_ABORT();                                    \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* _IPERF_DEBUG_H_ */
