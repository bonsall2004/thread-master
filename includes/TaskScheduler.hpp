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
#include <unordered_map>
#include <chrono>

namespace thread_master
{
  struct THANDLE
  {
      explicit THANDLE(int32_t id) : task_id(id), creation_time(std::chrono::system_clock::now())
      {
      }

      const int32_t task_id;
      const std::chrono::time_point<std::chrono::system_clock> creation_time;

      [[nodiscard]] bool IsValid() const
      {
        return valid;
      }

      bool operator==(const THANDLE& other) const
      {
        return task_id == other.task_id;
      }

      bool operator!=(const THANDLE& other) const
      {
        return task_id != other.task_id;
      }

      bool operator<(const THANDLE& other) const
      {
        return task_id < other.task_id;
      }

      bool operator>(const THANDLE& other) const
      {
        return task_id > other.task_id;
      }

      bool operator<=(const THANDLE& other) const
      {
        return task_id <= other.task_id;
      }

      bool operator>=(const THANDLE& other) const
      {
        return task_id >= other.task_id;
      }

      THANDLE& operator=(const THANDLE& other)
      {
        if(this == &other)
          return *this;
        valid = other.valid;
        return *this;
      }

    private:
      bool valid = true;

  };
}

using namespace thread_master;
class TaskScheduler
{
  public:
    static THANDLE* AddTask(std::thread&& task);
    static void RemoveTask(THANDLE* handle);
    static void JoinTask(THANDLE* handle);
    static void JoinAllTasks();

  private:
    static std::unordered_map<int32_t, std::thread> test;

    static std::pair<const int32_t, std::thread>& get_thread_by_handle(THANDLE* handle);
};
