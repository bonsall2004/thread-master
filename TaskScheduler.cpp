/*
 * lumos-compiler
 * Author: bonsall2004
 * Repository: https://github.com/bonsall2004/lumos-compiler
 * Description: Simple little compiler.
 */
#include "includes/TaskScheduler.h"

std::vector<std::thread> TaskScheduler::active_tasks;

void TaskScheduler::AddTask(std::thread&& task) {
  active_tasks.emplace_back(std::move(task));
}

void TaskScheduler::RemoveTask(std::thread& task) {
  auto it = std::find_if(active_tasks.begin(), active_tasks.end(), [&](const std::thread& t) {
    return std::addressof(t) == std::addressof(task);
  });

  if (it != active_tasks.end()) {
    it->detach();
    active_tasks.erase(it);
  }
}


void TaskScheduler::JoinTask(std::thread& task) {
  auto it = std::find_if(active_tasks.begin(), active_tasks.end(), [&](const std::thread& t) {
    return std::addressof(t) == std::addressof(task);
  });

  if (it != active_tasks.end()) {
    it->join();
    active_tasks.erase(it);
  }
}

void TaskScheduler::JoinAllTasks() {
  for (auto& task : active_tasks) {
    task.join();
  }
  active_tasks.clear();
}