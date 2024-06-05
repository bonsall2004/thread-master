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

void test()
{
  std::cout << "running statically" << std::endl;
}

int main()
{
  auto event_timer = event.SetPeriodicTimer(1);

  Dispatcher<>::SetTimer(5, event_timer);

  TaskScheduler::JoinAllTasks();
  return 0;
}

REGISTER_CALLBACK_WITH_DISPATCHER(event, testFunc);
