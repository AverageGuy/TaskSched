# TaskSched

## Overview

TaskSched is a cooperative multitasking scheduler for Arduino processors, designed as a simpler alternative to existing schedulers like TaskScheduler. It provides a flexible and efficient way to manage multiple tasks in Arduino projects, particularly tested on ESP32 and ESP8266 processors.

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

## Usage

### Creating Tasks

Tasks are created using the `Task` constructor:

```cpp
SafePtr<Task> task = SafePtr<Task>(new Task(callback, interval, enabled, iterations, name, runImmediately));
```

- `callback`: Function to be called (must accept a `Task*` parameter)
- `interval`: Time between calls (milliseconds if integer, seconds if float)
- `enabled`: Whether the task starts enabled
- `iterations`: Number of times to run (0 for infinite)
- `name`: Descriptive name for the task
- `runImmediately`: Whether to run immediately when enabled

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

