/*
 * thread-master
 * Author: bonsall2004
 * Description: simple multithreading and events library.
 */
#pragma once
#include <vector>
#include <thread>
#include <functional>
#include "TaskScheduler.h"
#include <cstdint>

template <typename... Args>
class Task
{
  public:
    using Callback = std::function<void(Args...)>;

    explicit Task(Callback callback, Args... args)
    {
      TaskThread = std::thread([callback, args...]()
      {
        callback(args...);
      });
      handle = (THANDLE*)TaskScheduler::AddTask(std::move(TaskThread));
    }

    ~Task()
    {
      TaskScheduler::RemoveTask(handle);
      delete handle;
    }

    THANDLE* get_handle()
    {
      return handle;
    }

  private:
    std::thread TaskThread;
    THANDLE* handle;
};