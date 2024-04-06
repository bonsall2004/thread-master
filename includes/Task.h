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

template <typename... Args>
class Task {
public:
  using Callback = std::function<void(Args...)>;

  explicit Task(Callback callback, Args... args) {
    TaskThread = std::thread([callback, args...]() {
      callback(args...);
    });
    TaskScheduler::AddTask(std::move(TaskThread));
  }

private:
  std::thread TaskThread;
};