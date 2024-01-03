
# TaskSched

## Overview

Yet another cooperative multitasking scheduler for Arduino processors. I was unable to make the
current crop of schedulers work correctly, so I decided to write my own.
This version is a simpler implementation of the flavor of TaskScheduler
(<https://github.com/arkhipenko/TaskScheduler>). 
## Contents

- [Main Features
- [Installation](#installation)
- [Usage](#usage)
  - [Creating Tasks](#creating-tasks)
  - [Task Methods](#task-methods) 
  - [Scheduler](#scheduler)
- [Examples](#examples)
- [API Reference](#api-reference)
- [Issues](#issues)
- [Contributing](#contributing)

## Installation

## Main Features  

1. Periodic task execution. The interval is specified in the task
   creation but can be changed. The times are in milliseconds only.
   Unlike TaskScheduler, the timing isn't necessarily exact if the
   program takes too much time in a task. It's scheduled on a best
   effort basis. If the code is tight enough it'll work if your time
   constraints aren't critical. There is no "catchup" support.
   
2. Support for task enable/disable.   

3. Support for number of iterations the task can make. The task will be
   disabled when the iteration count is exhausted. Unlimited iterations
   are also supported.
   
4. Task can be scheduled to run immediately when enabled if necessary
   or wait for the interval to expire.
   
5. A task can be restarted as it was defined or restarted with new
   parameters, such as different interval, different callback function,
   etc.
   
6. Timing control of the scheduler can be governed by a Ticker  
   (TickTwo) class object or by a call to the Scheduler running in the  
   loop.
   
7. Unlike TaskScheduler the processor does not sleep during idle time.   

***
git clone https://github.com/AverageGuy/tasksched.git 
***

## Usage

Intro paragraph explaining overall workflow.

### Creating Tasks

Explain how to create Task objects.

Code snippets for constructor variations. 

### Task Methods

Explain key methods for enabling, scheduling, etc.

Code snippets for method usage.

### Scheduler 

Explain how to create Scheduler, register tasks, start scheduling.

Code snippets for scheduler usage.

## Examples

Overview of included examples with links. Short description of each.

# API Reference

Auto generated docs for classes, methods, etc.

## Issues

Explain where to report issues, request features. 

## Contributing

Overview of how to contribute to the library.

Let me know if you would like me to expand any section with more details!
