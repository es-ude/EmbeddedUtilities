#ifndef PERIODICSCHEDULER_PERIODICSCHEDULER_H
#define PERIODICSCHEDULER_PERIODICSCHEDULER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <CException.h>

/**
 * Overall idea here is simple.
 * Have a timer interrupt set up, like so
 *
 * ```c
 * ISR(timer_vect)
 * {
 *  updateScheduledTasks(tasks, period);
 * }
 * ```
 *
 * and
 * ```c
 * void
 * updateScheduledTasks(PeriodicScheduler *self)
 * {
 *  // update period in ticks elapsed for all tasks
 * }
 * ```
 * then from main program call processTasks(Tasks *tasks)
 * implemented like so
 * ```c
 * void
 * processScheduledTasks(PeriodicScheduler *self)
 * {
 *  for (int i=0; i < number_of_tasks; i++)
 *  {
 *    if (taskIsDue(tasks[i]))
 *    {
 *      executeTask(tasks[i]);
 *      resetTasksElapsedTicks(tasks[i]);
 *    }
 *  }
 * }
 * ```
 *
 * Most important the PeriodicScheduler is not coupled
 * to any clock by design. Instead it counts ticks.
 * The Scheduler will let all added tasks age by the specified number of ticks
 * for each call to updateScheduledTasks(). A call to
 * processScheduledTasks() will then go through all tasks
 * and execute each of them whose elapsed time is bigger
 * than the specified period.
 *
 * After execution the elapsed time is reset.
 * This means that tasks are not executed
 * at the point in time where the timer interrupt is issued,
 * but just on the next time the processScheduledTasks() function
 * is called. And the new period for these tasks restarts only then.
 * E.g. you want to call a function every 50ms, but you run your
 * processScheduledTasks only every 100ms then your task will get
 * executed only every 100ms. So if you need your tasks to be
 * executed in time you will have to make sure your processScheduledTasks()
 * function is executed fast enough and that each of your tasks
 * does not take as long as to block the next periods tasks.
 *
 * The library has debug statements built in. To enable them
 * you have to define the functions declared in Debug.h and
 * make sure they are linked into your executable before you link
 * against the PeriodicScheduler lib. We expect the debug functions to
 * not add a new line after a string.
 *
 */

typedef enum PeriodicSchedulerExceptions
{
  PERIODIC_SCHEDULER_FULL_EXCEPTION = 0x01,
  PERIODIC_SCHEDULER_INVALID_TASK_EXCEPTION,
} PeriodicSchedulerExceptions;

typedef uint16_t Ticks;

typedef struct Task
{
  void (*function)(void *argument);
  void *argument;
  Ticks ticks_elapsed;
  Ticks period;
} Task;

/**
 * Although the definition of this struct is visible
 * further down, do not use the struct directly but
 * use the functions to manipulate it. This way
 * you ensure compatibility with upcoming changes.
 */
typedef struct PeriodicScheduler PeriodicScheduler;

/**
 * Execute every task in the scheduler for which
 * the configured time period has passed. After
 * execution the period restarts.
 */
void
processScheduledTasks(PeriodicScheduler *self);

/**
 * Call this from your timer interrupt service routine.
 * It updates all tasks in the Scheduler to reflect
 * the ticks passed.
 * This advances each tasks elapsed ticks by number_of_ticks.
 */
void
updateScheduledTasks(PeriodicScheduler *self,
                     Ticks number_of_elapsed_ticks);

/**
 * Returns the number of bytes needed for a Scheduler that
 * can hold maximum_number_of_tasks.
 */
size_t
getSchedulersRequiredMemorySize(uint8_t maximum_number_of_tasks);

/**
 * Each task is copied to the internal array of tasks.
 * You can therefore delete your task after this function returned.
 * The operation additionally returns an id for the task, that can later
 * be used to delete it.
 * The id is guaranteed to be within 0 and the maximum number of tasks minus one.
 * This fact can be used to save and manage data that needs be accessed during
 * task excecution.
 * Throws the PERIODIC_SCHEDULER_FULL_EXCEPTION when called while the
 * number of free slots is zero.
 */
uint8_t
addTaskToScheduler(PeriodicScheduler *self,
                   const Task        *task);

/**
 * The same as addTaskToScheduler
 */
uint8_t
scheduleTaskPeriodically(PeriodicScheduler *self,
                         const Task        *task);

/**
 * Removes all tasks from the current scheduler.
 * This essentially frees all slots in the schedule,
 * giving room for new tasks.
 */
void
removeAllTasksFromSchedule(PeriodicScheduler *self);

/**
 * Removes the task with the specified id from
 * the schedule.  */
void
removeScheduledTask(PeriodicScheduler *self,
                    uint8_t id);

/**
 * Returns the remain free slots in the schdule.
 * Use this function to determine how many tasks can still
 * be added to the scheduler.
 */
uint8_t
getNumberOfFreeSlotsInSchedule(const PeriodicScheduler *self);

/**
 * Returns a pointer to the task with the specified id
 */
Task *
getScheduledTaskById(const PeriodicScheduler *self,
                     uint8_t index);

/**
 * Creates a PeriodicScheduler struct at the given
 * memory area. The memory area is assumed to be big
 * enough to hold the PeriodicScheduler struct as well
 * as the array of Tasks. You can use the Macro
 * PERIODIC_SCHEDULER_SIZE to retrieve the necessary
 * size at compile time. IMPORTANT: Before using
 * the macro you will have to define PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS
 */
PeriodicScheduler *
createPeriodicScheduler(void   *memory,
                        uint8_t maximum_number_of_tasks);


#define PERIODIC_SCHEDULER_SIZE(maximum_number_of_tasks) ((( \
							     maximum_number_of_tasks) \
                                                           * sizeof(InternalTask)) \
                                                          + sizeof( \
							    PeriodicScheduler))

typedef struct InternalTask
{
  Task task;
  bool is_valid;
} InternalTask;

struct PeriodicScheduler
{
  InternalTask *tasks;
  const uint8_t limit;
};

#endif //PERIODICSCHEDULER_PERIODICSCHEDULER_H
