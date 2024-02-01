#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

void watchdog_enable(void);
void watchdog_disable(void);

void task_watchdog_strobe(void);

#endif