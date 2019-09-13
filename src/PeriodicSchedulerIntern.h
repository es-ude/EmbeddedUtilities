#ifndef PERIODICSCHEDULER_PERIODICSCHEDULERINTERN_H
#define PERIODICSCHEDULER_PERIODICSCHEDULERINTERN_H

#include "EmbeddedUtilities/PeriodicScheduler.h"

static void
executeTaskIfDue(InternalTask *tasks, uint8_t index);

static void
resetTask(Task *task);

#endif //PERIODICSCHEDULER_PERIODICSCHEDULERINTERN_H
