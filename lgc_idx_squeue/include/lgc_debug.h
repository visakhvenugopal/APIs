/*
 * -----------------------------------------------------------------------+
 * file		: lgc_debug.h
 * date		: 02/31/2020
 * compiler	: gcc
 * author	: Visakh Venugopal [visakh.vinayak@gmail.com]
 *
 * Exposes debug interfaces available.
 * -----------------------------------------------------------------------+
 */

/*To enable debug make with with, EXTRA_CFLAGS:= -DDEBUG */

#ifndef LGC_DEBUG_H
#define LGC_DEBUG_H

#define DBG_CRITICAL 0
#define DBG_ERROR 0
#define DBG_WARN 1
#define DBG_INFO 2
#define DBG_MSG 3
#define DBG_DEBUG 4

#ifdef DEBUG
#ifdef DEBUG_LVL
#define G_DEBUG_LEVEL DEBUG_LVL
#else
#define G_DEBUG_LEVEL DBG_CRITICAL
#endif

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* journelctl log loglevels
	0: emerg
	1: alert
	2: crit
	3: err
	4: warning
	5: notice
	6: info
	7: debug
*/
#define DPRINTF(lvl,fmt,args...)	\
	do {					\
		if (lvl <= g_log_LEVEL) {	\
			switch (lvl) {		\
				case DBG_CRITICAL : printf("<3>" ANSI_COLOR_RED "ERR! " ANSI_COLOR_RESET); break;	\
				case DBG_WARN : printf("<3>" ANSI_COLOR_YELLOW "WRN! " ANSI_COLOR_RESET); break;	\
				case DBG_INFO	: printf("<5>" ANSI_COLOR_CYAN "NFO! " ANSI_COLOR_RESET); break;	\
				case DBG_MSG	: printf("<6>" ANSI_COLOR_BLUE "MSG! " ANSI_COLOR_RESET); break;	\
				case DBG_DEBUG	: printf("<7>DBG! "); break;	\
			}			\
			printf(fmt, ##args);	\
		} 		 		\
	} while (0)

#else
#define DPRINTF(lvl,fmt,args...) do{}while (0)
#endif
#endif

