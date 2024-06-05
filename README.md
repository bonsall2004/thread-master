# Thread Master

A simplistic threading, event, and task library for C++.

## Installation

This project uses CMake, and I've made it very easy for you to use.

**Step 1:** Clone the repo

```bash
git clone https://github.com/bonsall2004/thread-master.git
```

**Step 2:** Add the directory to your CMakeLists.txt

```cmake
add_subdirectory(thread-master)
```

**Step 3:** Link the library and include its directory in your CMakeLists.txt

```cmake
target_link_libraries(YourExecutableOrLibraryName PRIVATE thread_master)
target_include_directories(YourExecutableOrLibraryName PRIVATE thread-master/includes)
```

Congratulations! You have now included the library in your project. Read the documentation below to get started.

## Documentation

### Tasks

Tasks are set up to use templates where the template arguments are the function parameter types. You can define tasks using lambda functions or callbacks.

#### Using Lambda Functions:

```cpp
int argument = 10;
Task<int> my_task = Task<int>([&]() {
    // Your task logic here
}, argument);
```

#### Using Callbacks:

```cpp
void task_func(int yes) {
    std::cout << yes << std::endl;
}

int main() {
    Task<int> my_task(task_func);
}
```

Tasks spawn their own threads and add themselves directly to the TaskScheduler. You can remove a task at any time using the TaskScheduler:

```cpp
TaskScheduler::RemoveTask(task_ref.get_handle());
```

You can join a task with:

```cpp
TaskScheduler::JoinTask(task_ref.get_handle());
```

Or join all tasks with:

```cpp
TaskScheduler::JoinAllTasks();
```

If you already have an `std::thread` and you'd like to add it to the TaskScheduler, you can do so using:

```cpp
TaskScheduler::AddTask(std::thread([](){
    // Your thread logic here
}));
```

### Dispatchers

Dispatchers provide a mechanism for managing and triggering events in your application. You can register callbacks with a dispatcher and then trigger those callbacks when needed.

#### Creating a Dispatcher

To create a dispatcher, you use the `DEFINE_DISPATCHER` macro. You can specify the types of arguments the dispatcher will handle as template parameters. If no arguments are needed, you can create a dispatcher without specifying any types.

```cpp
// Define a dispatcher with argument types int, int, and float
DEFINE_DISPATCHER(MyDispatcher, int, int, float);

// Define a dispatcher with no arguments
DEFINE_DISPATCHER(EmptyDispatcher);
```

#### Registering Callbacks

Once you have defined a dispatcher, you can register callbacks with it using the `REGISTER_CALLBACK_WITH_DISPATCHER` macro. These callbacks should match the signature expected by the dispatcher.

```cpp
// Define a callback function
void myCallback(int arg1, int arg2, float arg3) {
    // Your callback logic here
}

// Register the callback with the dispatcher
REGISTER_CALLBACK_WITH_DISPATCHER(MyDispatcher, myCallback);
```

#### Triggering Events

You can trigger events associated with a dispatcher by calling the `Dispatch` method. This will invoke all registered callbacks with the specified arguments.

```cpp
// Trigger an event with arguments
MyDispatcher.Dispatch(10, 20, 3.14f);
```

#### Additional Methods

The `Dispatcher` class provides additional methods for managing events and callbacks.

- `SetTimer`: Sets a one-time timer to trigger an event after a specified time interval.
- `SetPeriodicTimer`: Sets a periodic timer to trigger an event repeatedly at a specified interval.
- `CallEventByName`: Calls an event by name, triggering all registered callbacks associated with that event.
- `AddListener`: Adds a callback listener to the dispatcher.

### Example Usage

```cpp
#include <iostream>
#include "Dispatcher.hpp"

// Define a dispatcher with argument types int, int, and float
DEFINE_DISPATCHER(MyDispatcher, int, int, float);

// Define a callback function
void myCallback(int arg1, int arg2, float arg3) {
    std::cout << "Callback called with arguments: " << arg1 << ", " << arg2 << ", " << arg3 << std::endl;
}

int main() {
    // Trigger an event with arguments
    MyDispatcher.Dispatch(10, 20, 3.14f);

    // Set a timer to trigger an event after 2 seconds
    MyDispatcher.SetTimer(2, 30, 40, 6.28f);

    // Set a periodic timer to trigger an event every  second
    std::function<void()> stopTimer = MyDispatcher.SetPeriodicTimer(std::chrono::milliseconds(1000), 50, 60, 9.42f);
    
    Dispatcher::SetTimer(5, stopTimer);
    
    return 0;
}

REGISTER_CALLBACK_WITH_DISPATCHER(MyDispatcher, myCallback);
```

This demonstrates the usage of dispatchers and their associated methods for managing events and callbacks in a multithreaded environment.
