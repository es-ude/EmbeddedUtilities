#include "EmbeddedUtilities/Debug.h"
#include "EmbeddedUtilities/PeriodicScheduler.h"
#include <CException.h>
#include <stdio.h>
#include <unity.h>

#define PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS (8)

static uint8_t number_of_calls_to_someTask      = 0;
static uint8_t number_of_calls_to_someOtherTask = 0;
static uint8_t memory[PERIODIC_SCHEDULER_SIZE(PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS)];
static PeriodicScheduler *scheduler = (PeriodicScheduler *)memory;

void
setUp(void)
{
  number_of_calls_to_someTask      = 0;
  number_of_calls_to_someOtherTask = 0;
  scheduler =
    createPeriodicScheduler(memory, PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS);
}

void
checkRemainingCapacityAfterInit(uint8_t expected_capacity)
{
  uint8_t memory[getSchedulersRequiredMemorySize(expected_capacity)];
  PeriodicScheduler *scheduler =
    createPeriodicScheduler(memory, expected_capacity);
  TEST_ASSERT_EQUAL_UINT8(expected_capacity,
                          getNumberOfFreeSlotsInSchedule(scheduler));
}

void
test_initScheduler(void)
{
  checkRemainingCapacityAfterInit(PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS);
}

void
test_initScheduler2(void)
{
  checkRemainingCapacityAfterInit(5);
}

void
test_initScheduler3(void)
{
  checkRemainingCapacityAfterInit(53);
}

void
test_addTasksUntilOutOfCapacity(void)
{
  uint8_t max_capacity = 20;
  uint8_t memory[getSchedulersRequiredMemorySize(max_capacity)];
  PeriodicScheduler *scheduler = createPeriodicScheduler(memory, max_capacity);
  Task task;
  for (uint8_t counter = 0; counter < max_capacity; counter++)
    {
      addTaskToScheduler(scheduler, &task);
    }
  CEXCEPTION_T e;
  Try
  {
    addTaskToScheduler(scheduler, &task);
    TEST_FAIL();
  }
  Catch(e) { TEST_ASSERT_EQUAL_UINT8(e, PERIODIC_SCHEDULER_FULL_EXCEPTION); }
}

void
someTask(void *argument)
{
  number_of_calls_to_someTask++;
}

void
test_addTaskAndCallDueTaskOnProcess(void)
{
  Task task = {
    .period   = 10,
    .function = someTask,
    .argument = NULL,
  };
  addTaskToScheduler(scheduler, &task);
  updateScheduledTasks(scheduler, 15);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL_UINT8(1, number_of_calls_to_someTask);
}

void
test_callTaskOnProcess2(void)
{
  Task task = {
    .period   = 1,
    .function = someTask,
    .argument = NULL,
  };
  addTaskToScheduler(scheduler, &task);
  updateScheduledTasks(scheduler, 1);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL_UINT8(1, number_of_calls_to_someTask);
}

void
test_addTaskAndNotCallItOnProcess(void)
{
  Task task = {
    .period   = 20,
    .function = someTask,
    .argument = NULL,
  };
  addTaskToScheduler(scheduler, &task);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL_UINT8(0, number_of_calls_to_someTask);
}

void
someOtherTask(void *first_arg)
{
  number_of_calls_to_someOtherTask++;
}

void
test_addTwoTasksAndCallOnlySecond(void)
{
  Task first = {
    .period   = 2,
    .function = someTask,
    .argument = NULL,
  };
  uint8_t arg = 23;
  Task second = {
    .period   = 1,
    .function = someOtherTask,
    .argument = &arg,
  };
  addTaskToScheduler(scheduler, &first);
  addTaskToScheduler(scheduler, &second);
  updateScheduledTasks(scheduler, 1);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL_UINT8(0, number_of_calls_to_someTask);
  TEST_ASSERT_EQUAL_UINT8(1, number_of_calls_to_someOtherTask);
}

void
test_executeOneTaskTwoTimes(void)
{
  Task task = {
    .period   = 2,
    .function = someTask,
    .argument = NULL,
  };
  addTaskToScheduler(scheduler, &task);

  updateScheduledTasks(scheduler, 2);
  processScheduledTasks(scheduler);

  updateScheduledTasks(scheduler, 2);
  processScheduledTasks(scheduler);

  TEST_ASSERT_EQUAL_UINT8(2, number_of_calls_to_someTask);
}

void
test_executeTwoTasksOneTimeEach(void)
{
  Task task = {
    .period   = 2,
    .function = someTask,
    .argument = NULL,
  };
  addTaskToScheduler(scheduler, &task);
  addTaskToScheduler(scheduler, &task);

  updateScheduledTasks(scheduler, 2);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL_UINT8(2, number_of_calls_to_someTask);
}

void
test_executeExactlyOnce(void)
{
  Task task = {
    .period   = 1,
    .function = someTask,
    .argument = NULL,
  };
  addTaskToScheduler(scheduler, &task);
  updateScheduledTasks(scheduler, 1);
  processScheduledTasks(scheduler);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL_UINT8(1, number_of_calls_to_someTask);
}

void
test_getRemainingCapacityAfterAddingTask(void)
{
  Task task;
  addTaskToScheduler(scheduler, &task);
  TEST_ASSERT_EQUAL_UINT8(PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS - 1,
                          getNumberOfFreeSlotsInSchedule(scheduler));
}

void
test_getRemainingCapacityAfterAddAndClear(void)
{
  Task task;
  addTaskToScheduler(scheduler, &task);
  removeAllTasksFromSchedule(scheduler);
  TEST_ASSERT_EQUAL_UINT8(PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS,
                          getNumberOfFreeSlotsInSchedule(scheduler));
}

void
someTaskWithArg(void *arg)
{
  TEST_ASSERT_EQUAL(15, *(uint8_t *)arg);
}

void
test_callTaskWithArg(void)
{
  uint8_t arg = 15;
  Task task   = {
    .function = someTaskWithArg,
    .period   = 1,
    .argument = &arg,
  };
  addTaskToScheduler(scheduler, &task);
  updateScheduledTasks(scheduler, 1);
  processScheduledTasks(scheduler);
}

void
test_taskIsResetOnAdd(void)
{
  Task task = {
    .function      = someTask,
    .argument      = NULL,
    .period        = 2,
    .ticks_elapsed = 14,
  };
  addTaskToScheduler(scheduler, &task);
  updateScheduledTasks(scheduler, 1);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL_UINT8(0, number_of_calls_to_someTask);
}

void
test_getTaskForIndex(void)
{
  Task task1 = { .argument = (void *)1 };
  Task task2 = { .argument = (void *)2 };
  Task task3 = { .argument = (void *)3 };
  addTaskToScheduler(scheduler, &task1);
  addTaskToScheduler(scheduler, &task2);
  addTaskToScheduler(scheduler, &task3);
  TEST_ASSERT_EQUAL_PTR((void *)2, getScheduledTaskById(scheduler, 1)->argument);
}

void
test_removeAndGet(void)
{
  Task task1 = { .argument = (void *)1 };
  Task task2 = { .argument = (void *)2 };
  addTaskToScheduler(scheduler, &task1);
  addTaskToScheduler(scheduler, &task2);
  removeScheduledTask(scheduler, 0);
  CEXCEPTION_T  exception;
  Try {
  getScheduledTaskById(scheduler, 0);
  TEST_FAIL();
  }
  Catch (exception)
  {
    TEST_ASSERT_EQUAL(PERIODIC_SCHEDULER_INVALID_TASK_EXCEPTION, exception);
  }
}

void
test_removeAndProcess(void)
{
  Task task = {
    .function = someTask,
    .period   = 1,
  };
  addTaskToScheduler(scheduler, &task);
  addTaskToScheduler(scheduler, &task);
  removeScheduledTask(scheduler, 0);
  updateScheduledTasks(scheduler, 1);
  processScheduledTasks(scheduler);
  TEST_ASSERT_EQUAL(1, number_of_calls_to_someTask);
}

void
check_lastAddedTaskIsStillCalledAfterRemovingAPreviousTask(uint8_t number_of_tasks,
                                uint8_t index_for_removal)
{
  Task task = {
    .function = someTask,
    .period   = 1,
  };

  Task other_task = {
    .function = someOtherTask,
    .period   = 1,
  };

  for (uint8_t i = 0; i < number_of_tasks - 1; i++)
    {
      addTaskToScheduler(scheduler, &task);
    }

  addTaskToScheduler(scheduler, &other_task);
  removeScheduledTask(scheduler, index_for_removal);

  updateScheduledTasks(scheduler, 1);
  processScheduledTasks(scheduler);

  TEST_ASSERT_EQUAL(1, number_of_calls_to_someOtherTask);
}

void
test_TasksMoveToFrontOnRemovalOfFirst(void)
{
  check_lastAddedTaskIsStillCalledAfterRemovingAPreviousTask(6,0);
}

void
test_TasksMoveToFrontOnRemovalOfThird(void)
{

  check_lastAddedTaskIsStillCalledAfterRemovingAPreviousTask(8, 4);
}

void
test_AddTaskAndUseIdForRemoval(void)
{
  Task task;
  uint8_t id = addTaskToScheduler(scheduler, &task);
  removeScheduledTask(scheduler, id);
  TEST_ASSERT_EQUAL(PERIODIC_SCHEDULER_MAX_NUMBER_OF_TASKS,
                    getNumberOfFreeSlotsInSchedule(scheduler));
}

void
test_AddTaskAndRemoveSameIdTwice(void)
{
  Task task;
  uint8_t id = addTaskToScheduler(scheduler, &task);
  CEXCEPTION_T exception;
  removeScheduledTask(scheduler, id);
  Try
  {
    removeScheduledTask(scheduler, id);
    TEST_FAIL();
  }
  Catch(exception)
  {
    TEST_ASSERT_EQUAL(PERIODIC_SCHEDULER_INVALID_TASK_EXCEPTION,
                      exception);
  }
}

void
test_AddTaskTwoTimesAndRemoveSameIdTwice(void)
{
  Task task;
  uint8_t id = scheduleTaskPeriodically(scheduler, &task);
  CEXCEPTION_T exception;
  addTaskToScheduler(scheduler, &task);
  removeScheduledTask(scheduler, id);
  Try {
    removeScheduledTask(scheduler, id);
    TEST_FAIL_MESSAGE("exception not thrown");
  } Catch(exception)
  {
    TEST_ASSERT_EQUAL(PERIODIC_SCHEDULER_INVALID_TASK_EXCEPTION, exception);
  }
}

void
test_AddAndRemoveInArbitrarySequence(void)
{
  Task task = {
    .function = someTask,
    .period = 1,
    .argument = (void *) 6,
  };

  uint8_t first_id = scheduleTaskPeriodically(scheduler, &task);
  scheduleTaskPeriodically(scheduler, &task);
  uint8_t mid_id = scheduleTaskPeriodically(scheduler, &task);
  scheduleTaskPeriodically(scheduler, &task);
  task.argument = (void*) 3;
  uint8_t last_id = scheduleTaskPeriodically(scheduler, &task);
  removeScheduledTask(scheduler, mid_id);
  TEST_ASSERT_EQUAL_PTR((void *)3, getScheduledTaskById(scheduler, last_id)->argument);
}

void
test_getNonExistentItemThrowsException(void)
{
  Task task = {
    .function = someTask,
    .period = 1,
    .argument = (void*)1,
  };

  CEXCEPTION_T exception;
  uint8_t id = scheduleTaskPeriodically(scheduler, &task);
  Try
  {
    getScheduledTaskById(scheduler, id+1);
    TEST_FAIL();
  }
  Catch(exception)
  {
    TEST_ASSERT_EQUAL(PERIODIC_SCHEDULER_INVALID_TASK_EXCEPTION, exception);
  }
}
