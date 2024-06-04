/*
 * lumos-compiler
 * Author: bonsall2004
 * Repository: https://github.com/bonsall2004/lumos-compiler
 * Description: Simple little compiler.
 */
#include "includes/TaskScheduler.h"
#include <thread>
#include <stdexcept>

static int32_t last_id;

std::unordered_map<int32_t, std::thread> TaskScheduler::test = {};

THANDLE* TaskScheduler::AddTask(std::thread&& task)
{
  auto* handle = new THANDLE(last_id++);
  test.emplace(handle->task_id, std::move(task));
  return handle;
}

void TaskScheduler::RemoveTask(THANDLE* handle)
{
  auto it = test.find(handle->task_id);

  if(it != test.end())
  {
    if(it->second.joinable())
    {
      it->second.detach();
    }
    test.erase(it);
  }
}
void TaskScheduler::JoinTask(THANDLE* handle)
{
  auto it = test.find(handle->task_id);

  if(it != test.end())
  {
    if(it->second.joinable())
    {
      it->second.join();
    }
    test.erase(it);
  }
}

void TaskScheduler::JoinAllTasks()
{
  for(auto& pair : test)
  {
    if(pair.second.joinable())
    {
      pair.second.join();
    }
  }
  test.clear();
}

std::pair<const int32_t, std::thread>& TaskScheduler::get_thread_by_handle(THANDLE* handle)
{
  auto it = test.find(handle->task_id);
  if(it != test.end())
  {
    return *it;
  }

  throw std::invalid_argument("Fatal: THANDLE Not Found in Scheduler");
}