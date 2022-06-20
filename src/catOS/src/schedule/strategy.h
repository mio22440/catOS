/**
 * @file strategy.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef STRATEGY_H
#define STRATEGY_H

#if (USE_EDF_SCHED == 1)
    #include "edf.h"
#endif

#define SCHED_STRATEGY_PRIO         0
#define SCHED_STRATEGY_EDF          1        /**< EDF调度*/

#endif

