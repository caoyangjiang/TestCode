// Copyright 2017 Caoyang Jiang

#include <chrono>
#include <iostream>
#include <thread>

#include "Event.h"

void Thread(Event& event)
{
  std::this_thread::sleep_for(std::chrono::seconds(3));
  event.Set();
  std::cout << "Thread woke up" << std::endl;
};

int main()
{
  Event event = Event::GenerateNewEvent(Event::EventWaitType::EventWaitSignal);

  std::thread thread(Thread, std::ref(event));
  while (!event.Wait(100))
  {
  }

  std::cout << "triggered main" << std::endl;
  event.Close();
  thread.join();
  return 0;
}
