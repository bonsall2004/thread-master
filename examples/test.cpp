/*
 * thread_master
 * Author: bonsall2004
 * Description: 
 */
#include <cstdio>
#include <Task.h>
#include <Dispatcher.h>
//#include <MemDBG>

Dispatcher<> Beans;

void testFunc()
{
  printf("Testt\n");
}

void testFunc1()
{
  printf("Tomatoes are disgusting\n");
}

int main()
{
  Beans.SetTimer(1);
  Beans.DispatchAsync();

  TaskScheduler::JoinAllTasks();
  return 0;
}

REGISTER_CALLBACK_WITH_DISPATCHER(Beans, testFunc);
REGISTER_CALLBACK_WITH_DISPATCHER(Beans, testFunc1);
