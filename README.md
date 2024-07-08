# TaskSched

## Overview

TaskSched is a cooperative multitasking scheduler for Arduino processors, designed as a simpler alternative to existing schedulers like TaskScheduler. It provides a flexible and efficient way to manage multiple tasks in Arduino projects, particularly tested on ESP32 and ESP8266 processors.

A cooperative task scheduler is a system that manages the execution of multiple tasks, but with a twist compared to a traditional scheduler. Here's the key difference:

Cooperative: In a cooperative scheme, tasks are responsible for voluntarily giving up control of the processor when they're done or need to wait for something. The scheduler simply provides a starting point and trusts the tasks to behave.
This is in contrast to a preemptive scheduler, where the operating system can interrupt a running task and switch to another one.

Here's a breakdown of how cooperative scheduling works:

- The scheduler starts a task.
- The task runs until it finishes its work or needs to wait for something (like user input or data from another task).
- The task then signals the scheduler that it's done or needs to wait.
- The scheduler picks another task to run.
Pros and Cons:

Pros: Simpler to implement, fewer system resources needed.
Cons: Unpredictable behavior if tasks don't cooperate, can lead to unresponsive systems if a task gets stuck.
Use Cases:

Often used in early operating systems or embedded systems with limited resources.
Can be a good choice for specific situations where tasks have well-defined execution times and don't rely on external events heavily.

## Contents

- [Main Features](#main-features)
- [Installation](#installation)
- [Usage](#usage)
  - [Creating Tasks](#creating-tasks)
  - [Task Methods](#task-methods)
  - [Scheduler](#scheduler)
- [Examples](#examples)
- [API Reference](#api-reference)
- [Issues](#issues)
- [Contributing](#contributing)

## Main Features

1. **Periodic task execution**: Tasks can be set to run at specified intervals, with times specified in milliseconds or seconds.
2. **Task enable/disable support**: Tasks can be dynamically enabled or disabled.
3. **Iteration control**: Tasks can be set to run for a specific number of iterations or indefinitely.
4. **Immediate or delayed execution**: Tasks can be scheduled to run immediately when enabled or wait for the interval to expire.
5. **Task restart and parameter modification**: Tasks can be restarted with original or new parameters (interval, callback function, etc.).
6. **Flexible timing control**: The scheduler can be run periodically in the main loop.
7. **Safe pointer implementation**: Uses `SafePtr` for improved memory management and safety.

## Installation

To install TaskSched, clone the repository:

```
git clone https://github.com/AverageGuy/tasksched.git
```

Or download the [ZIP](https://github.com/AverageGuy/pyconky/archive/refs/heads/main.zip) file.

## Usage

### Creating Tasks

Tasks are created using the `Task` constructor:

```
SafePtr<Task> task = SafePtr<Task>(new Task(callback, interval, enabled, iterations, name, runImmediately));
SafePtr<Task> task = SafePtr<Task>(new Task(VoidCallBack, interval, enabled, iterations, name, runImmediately));
```

- `TaskCallback`: Function to be called (must accept a `Task*` parameter)
- `VoidCallback`: Function to be called (no parameter)
- `interval`: Time between calls milliseconds if integer
- `enabled`: Whether the task starts enabled
- `iterations`: Number of times to run (0 for infinite)
- `name`: Descriptive name for the task
- `runImmediately`: Whether to run immediately when enabled without waiting for interval to expire

### Task Methods

Key methods for managing tasks:

- `enable()`: Enable the task
- `disable()`: Disable the task
- `restart()`: Restart the task with original parameters
- `setInterval(newInterval)`: Set a new interval
- `setIterations(newIterations)`: Set a new iteration count
- `isEnabled()`: Check if the task is enabled
- `isFirstIteration()`: Check if it's the first iteration
- `isLastIteration()`: Check if it's the last iteration

### Scheduler

The `Sched` class manages multiple tasks:

```cpp
Sched scheduler;
scheduler.addTask(task);
scheduler.begin();

void loop() {
    scheduler.run();
}
```

## Examples

See the `examples/SkedBlink1/SkedBlink1.ino` file for a basic example of blinking an LED using TaskSched.

## API Reference

For detailed API documentation, please refer to the Doxygen-generated documentation in the `docs` folder.

## Issues

If you encounter any issues or have feature requests, please open an issue on the GitHub repository.

## Contributing

Contributions to TaskSched are welcome. Please feel free to submit pull requests or open issues to discuss potential improvements.

