#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <thread>
#include "TaskScheduler.hpp" // Include your TaskScheduler header file

#define CAT_(A, B) A ## B
#define CAT(A, B) CAT_(A, B)

template <typename... Args>
class Dispatcher
{
  public:
    using Callback = std::function<void(Args...)>;

    Dispatcher() = default;

    explicit Dispatcher(std::string name) : name(name)
    {
      dispatchers[name] = this;
    }

    explicit Dispatcher(Callback callback)
    {
      callbacks.push_back(callback);
    }

    Dispatcher(std::string name, Callback callback) : name(name)
    {
      dispatchers[name] = this;
      callbacks.push_back(callback);
    }

    ~Dispatcher()
    {
      if(name.empty()) return;
      auto it = dispatchers.find(name);
      if(it != dispatchers.end())
      {
        dispatchers.erase(it);
      }
    }

    static Dispatcher* GetEventByName(const std::string& name)
    {
      auto it = dispatchers.find(name);
      if(it != dispatchers.end())
      {
        return it->second;
      }
      else
      {
        return nullptr;
      }
    }

    void SetTimer(int64_t time, Args... args)
    {
      TaskScheduler::AddTask(std::thread([time, args..., this]()
      {
        std::this_thread::sleep_for(std::chrono::seconds(time));
        Dispatch(args...);
      }));
    }

    void SetTimer(std::chrono::milliseconds ms, Args... args)
    {
      TaskScheduler::AddTask(std::thread([ms, args..., this]()
      {
        std::this_thread::sleep_for(ms);
        Dispatch(args...);
      }));
    }

    [[nodiscard]] volatile bool* SetPeriodicTimer(std::chrono::milliseconds interval, Args... args)
    {
      volatile bool* bIsLooping = new bool(true);
      TaskScheduler::AddTask(std::thread([interval, bIsLooping, args..., this]()
      {
        do
        {
          std::this_thread::sleep_for(interval);
          Dispatch(args...);
        }
        while(*bIsLooping);
      }));

      return bIsLooping;
    }

    [[nodiscard]] volatile bool* SetPeriodicTimer(uint64_t interval, Args... args)
    {
      return SetPeriodicTimer(std::chrono::milliseconds(interval * 1000), args...);
    }

    [[maybe_unused]] static bool CallEventByName(const std::string& name, Args... args)
    {
      Dispatcher* event = GetEventByName(name);
      if(event == nullptr) return false;
      event->Dispatch(args...);
      return true;
    }

    [[maybe_unused]]
    void AddListener(Callback callback)
    {
      callbacks.push_back(callback);
    }

    [[maybe_unused]] void DispatchAsync(Args... args)
    {
      for(auto& callback : callbacks)
      {
        TaskScheduler::AddTask(std::thread([callback, args...]()
        {
          callback(args...);
        }));
      }
    }

    void Dispatch(Args... args)
    {
      for(auto& callback : callbacks)
      {
        callback(args...);
      }
    }

  private:
    std::string name;
    std::vector<Callback> callbacks;
    static std::map<std::string, Dispatcher*> dispatchers;
};

template <typename... Args> std::map<std::string, Dispatcher<Args...>*> Dispatcher<Args...>::dispatchers;


/**
 * @brief This macro is used to register a callback and bind it to a dispatcher
 * @param DispatcherName this is the name of the dispatcher you put into the DEFINE_DISPATCHER macro
 * @param Callback The callback is the function that should be run when the dispatcher is called, it should contain the same argument types that you passed through to the DEFINE_DISPATCHER
 * @example @code{.cpp} REGISTER_CALLBACK_WITH_DISPATCHER(DispatcherName, Callback)
 * REGISTER_CALLBACK_WITH_DISPATCHER(DispatcherName, [](){  }) @endcode
 * @usage @code{cpp}
 * DEFINE_DISPATCHER(my_event, int, int)
 *
 * void my_cool_function(int a, int b)
 * {
 *   std::cout << a + b << std::endl;
 * }
 *
 * int main()
 * {
 *   my_event.Dispatch(10, 20);
 *   my_event.DispatchAsync(20, 30);
 *   my_event.SetTimer(10, 40, 40);
 *
 *   TaskScheduler::JoinAllTasks();
 *   return 0;
 * }
 *
 * REGISTER_CALLBACK_WITH_DISPATCHER(my_event, my_cool_function)
 *
 *
 */
#define REGISTER_CALLBACK_WITH_DISPATCHER(x, y) namespace {\
  struct CAT(x, __LINE__) {                                \
    CAT(x, __LINE__)() {                                   \
      (x).AddListener(y);                                  \
    };                                                     \
  };                                                       \
  static CAT(x, __LINE__) CAT(m_,__LINE__);                \
  }

/**
 * @brief Use this macro to define a dispatcher, the dispatcher should be defined at the top of the file.
 * @param Name This will be the name of the variable you use to access the dispatcher.
 * @param Arguments this is optional; these are the argument types for the dispatcher.
 * @example @code{.cpp} DEFINE_DISPATCHER(EventName, int, int, float, const char*); @endcode
 * @usage @code{c}
 *
 * DEFINE_DISPATCHER(my_event, int, int);
 *
 * int main()
 * {
 *   my_event.Dispatch(10, 40);
 * }@endcode
 * @return void
 */
#define DEFINE_DISPATCHER(Name, ARGTYPES...) Dispatcher<ARGTYPES> Name