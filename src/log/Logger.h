#pragma once

// Log all to Serial, comment this line to disable logging
#define LOG Serial

#if defined(LOG)
    #define log_print(...) LOG.print(__VA_ARGS__)
    #define log_println(...) LOG.println(__VA_ARGS__)
#else
    #define log_print(...) ((void)0)
    #define log_println(...) ((void)0)
#endif
