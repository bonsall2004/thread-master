/*
 * thread_master
 * Author: bonsall2004
 * Description: 
 */
#include <cstdio>
#include <Task.hpp>
#include <Dispatcher.hpp>

DEFINE_DISPATCHER(event);
DEFINE_DISPATCHER(event2, volatile bool*);

void testFunc()
{
  static int yes = 0;
  printf("Message #%d\n", yes++);
}

void testFunc1(volatile bool* disable)
{
  *disable = false;
}

int main()
{
  auto event_timer = event.SetPeriodicTimer(std::chrono::milliseconds(100));
  event2.SetTimer(7, event_timer);

  TaskScheduler::JoinAllTasks();
  delete event_timer;
  return 0;
}

REGISTER_CALLBACK_WITH_DISPATCHER(event, testFunc);
REGISTER_CALLBACK_WITH_DISPATCHER(event2, testFunc1);
