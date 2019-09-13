#include "EmbeddedUtilities/Debug.h"
#include "EmbeddedUtilities/PeriodicScheduler.h"
#include "src/PeriodicSchedulerIntern.h"

PeriodicScheduler *
createPeriodicScheduler(void   *memory,
                        uint8_t maximum_number_of_tasks)
{
  PeriodicScheduler *returned_scheduler = (PeriodicScheduler *) memory;

  // removing const here is okay since the memory area does not hold a const
  // object but raw memory either allocated on stack via an array definition or
  // by malloc
  *(uint8_t *)&returned_scheduler->limit = maximum_number_of_tasks;

  returned_scheduler->tasks        = memory + sizeof(PeriodicScheduler);
  for (uint8_t i = 0; i < maximum_number_of_tasks; i++)
    {
      returned_scheduler->tasks[i].is_valid = false;
    }
  return ((PeriodicScheduler *) memory);
}

uint8_t
getNumberOfFreeSlotsInSchedule(const PeriodicScheduler *self)
{
  uint8_t number_of_invalid_tasks = 0;
  for (uint8_t index = 0; index < self->limit; index++)
  {
    if (!self->tasks[index].is_valid)
    {
      number_of_invalid_tasks++;
    }
  }
  return number_of_invalid_tasks;
}

size_t
getSchedulersRequiredMemorySize(uint8_t task_limit)
{
  return task_limit * sizeof(InternalTask) + sizeof(PeriodicScheduler);
}

uint8_t
addTaskToScheduler(PeriodicScheduler *self,
                   const Task        *task)
{
    uint8_t index = 0;
    while (index < self->limit && self->tasks[index].is_valid)
      {
	index++;
      }
    if (index == self->limit)
      {
	Throw(PERIODIC_SCHEDULER_FULL_EXCEPTION);
      }
    else
      {
	self->tasks[index].task     = *task;
	self->tasks[index].is_valid = true;
	resetTask(&self->tasks[index].task);
	debug(String, "added task number ");
	debug(UInt16, index);
	debug(String, "\n");
      }
    return index;
}

uint8_t
scheduleTaskPeriodically(PeriodicScheduler *self,
                         const Task        *task)
{
  return addTaskToScheduler(self, task);
}

void
updateScheduledTasks(PeriodicScheduler *self,
                     Ticks number_of_ticks)
{
  for (uint8_t i = 0; i < self->limit; i++)
    {
      if (self->tasks[i].is_valid)
	{
	  ((Task*) (self->tasks + i))->ticks_elapsed += number_of_ticks;
	}
    }
}

void
processScheduledTasks(PeriodicScheduler *self)
{
  for (uint8_t i = 0; i < self->limit; i++)
    {
      executeTaskIfDue(self->tasks, i);
    }
}

void
removeAllTasksFromSchedule(PeriodicScheduler *self)
{
  for (uint8_t index = 0; index < self->limit; index++)
  {
    self->tasks[index].is_valid = false;
  }
}

void
executeTaskIfDue(InternalTask *tasks,
                 uint8_t       index)
{
  Task *task = (Task *) (tasks + index);
  if (task->ticks_elapsed >= task->period && ((InternalTask *) task)->is_valid)
    {
      debug(String, "executing task ");
      debug(UInt16, index);
      debug(String, "\n");
      task->function(task->argument);
      resetTask(task);
    }
}

void
resetTask(Task *task)
{
  task->ticks_elapsed = 0;
}

Task *
getScheduledTaskById(const PeriodicScheduler *self,
                     uint8_t index)
{
  InternalTask *task = self->tasks + index;
  if (!task->is_valid)
    {
      Throw(PERIODIC_SCHEDULER_INVALID_TASK_EXCEPTION);
    }
  return (Task *) (self->tasks + index);
}

void
removeScheduledTask(PeriodicScheduler *self,
                    uint8_t id)
{
  if (self->tasks[id].is_valid)
    {
      self->tasks[id].is_valid = false;
    }
  else
    {
      Throw(PERIODIC_SCHEDULER_INVALID_TASK_EXCEPTION);
    }
}
