#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#define watchdog_reset() __asm__ __volatile__ ("wdr")

void watchdog_enable(void);
void watchdog_disable(void);

void task_watchdog_strobe(void);


#endif