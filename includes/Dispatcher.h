#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <thread>
#include "TaskScheduler.h" // Include your TaskScheduler header file


template <typename... Args>
class Dispatcher {
public:
  using Callback = std::function<void(Args...)>;

  Dispatcher() = default;

  explicit Dispatcher(std::string name) : name(name) {
    dispatchers[name] = this;
  }

  explicit Dispatcher(Callback callback) {
    callbacks.push_back(callback);
  }

  Dispatcher(std::string name, Callback callback) : name(name) {
    dispatchers[name] = this;
    callbacks.push_back(callback);
  }

  ~Dispatcher() {
    if (name.empty()) return;
    auto it = dispatchers.find(name);
    if (it != dispatchers.end()) {
      dispatchers.erase(it);
    }
  }

  static Dispatcher* GetEventByName(const std::string& name) {
    auto it = dispatchers.find(name);
    if (it != dispatchers.end()) {
      return it->second;
    } else {
      return nullptr;
    }
  }

  void SetTimer(int64_t time, Args... args) {
    TaskScheduler::AddTask(std::thread([time, args..., this]() {
      std::this_thread::sleep_for(std::chrono::seconds(time));
      Dispatch(args...);
    }));
  }

  [[maybe_unused]] static bool CallEventByName(const std::string& name, Args... args) {
    Dispatcher* event = GetEventByName(name);
    if (event == nullptr) return false;
    event->Dispatch(args...);
    return true;
  }

  [[maybe_unused]] void AddListener(Callback callback) {
    callbacks.push_back(callback);
  }

  [[maybe_unused]] void DispatchAsync(Args... args) {
    for (auto& callback : callbacks) {
      TaskScheduler::AddTask(std::thread([callback, args...](){
        callback(args...);
      }));
    }
  }

  void Dispatch(Args... args) {
    for (auto& callback : callbacks) {
      callback(args...);
    }
  }

  [[maybe_unused]] void JoinAllTasks() {
    TaskScheduler::JoinAllTasks();
  }

private:
  std::string name;
  std::vector<Callback> callbacks;
  static std::map<std::string, Dispatcher*> dispatchers;
};

template <typename... Args>
std::map<std::string, Dispatcher<Args...>*> Dispatcher<Args...>::dispatchers;
