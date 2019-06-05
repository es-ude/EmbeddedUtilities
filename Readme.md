## EmbeddedUtil

This repository holds a collection of small utilities. These currently include
* PeriodicScheduler
* BitManipulation
* Mutex
* MultiReaderBuffer
* Callback
* Debug

### PeriodicScheduler
The scheduler offers a way to execute tasks periodically. The period can be specified for each task separately.

The scheduler is not automatically bound to any interrupt. You as a user will have to call an update function to tell the scheduler
how many logical ticks have elapsed since the last call.

### BitManipulation
This is a header only library, containing 
several functions for operations on byte arrays, such as setting or clearing the i-th bit in an array.

### Mutex
Simple Mutex implementation. The user has to provide a function that ensures an atomic context.

### Debug
A header only library offering macros for printing debug messages. When compiled with `-DDEBUG=0`, debug output is disabled and strings contained in the arguments of the debug statements will be removed through compiler optimization. The user will have to provide functions for printing the several symbols.

A version for which the user only has to 
provide a single function, might be added in 
the future.

### MultiReaderBuffer
A single producer, multiple consumer, fifo buffer.

### Callback
Contains a general definition for callbacks, used at several places.