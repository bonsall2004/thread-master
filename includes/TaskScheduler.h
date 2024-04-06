/*
 * lumos-compiler
 * Author: bonsall2004
 * Repository: https://github.com/bonsall2004/lumos-compiler
 * Description: Simple little compiler.
 */

#pragma once
#include <vector>
#include <thread>
#include <algorithm>

class TaskScheduler {
public:
  static void AddTask(std::thread&& task);

  static void RemoveTask(std::thread& task);

  static void JoinTask(std::thread& task);

  static void JoinAllTasks();

private:
  static std::vector<std::thread> active_tasks;
};