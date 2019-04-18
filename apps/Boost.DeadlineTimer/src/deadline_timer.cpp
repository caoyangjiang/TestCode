// Copyright 2017 Caoyang Jiang

#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>

// Asynchronize time
void handler(boost::system::error_code ec)
{
  if (!ec)
    std::cout << "Timer fired." << std::endl;
  else
    std::cout << "Timer failed with " << ec.message() << std::endl;
}

int main()
{
  boost::asio::io_service service;
  boost::asio::deadline_timer timer(service);

  timer.expires_from_now(boost::posix_time::seconds(3));
  timer.async_wait(handler);

  service.run();
  return 0;
}

/*
// Synchronize time
int main()
{
  boost::asio::io_service service;
  boost::asio::deadline_timer timer(service);

  timer.expires_from_now(boost::posix_time::seconds(3));
  timer.wait();
  return 0;
}
*/
