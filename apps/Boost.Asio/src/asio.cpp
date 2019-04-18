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

// Reverse DNS lookup
int main()
{
  try
  {
    boost::asio::io_service service;
    boost::asio::ip::tcp::resolver resolver(service);
    boost::asio::ip::tcp::endpoint google(
        boost::asio::ip::address::from_string("172.217.11.16"), 80);
    boost::asio::ip::tcp::resolver::iterator end,
        iter = resolver.resolve(google);

    while (iter != end)
    {
      std::cout << "host name: " << iter->host_name()
                << ", service: " << iter->service_name() << std::endl;
      iter++;
    }
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}

/*
// DNS lookup
int main()
{
  try
  {
    boost::asio::io_service service;
    boost::asio::ip::tcp::resolver resolver(service);
    boost::asio::ip::tcp::resolver::query query("www.google.com", "80");
    boost::asio::ip::tcp::resolver::iterator end,
        iter = resolver.resolve(query);

    while (iter != end)
    {
      boost::asio::ip::tcp::endpoint endpoint = iter->endpoint();
      std::cout << "Address: " << endpoint.address()
                << ", port: " << endpoint.port() << std::endl;
      iter++;
    }
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}
*/

/*
//Test thread pool and worker
int main()
{
  boost::asio::io_service service;
  std::unique_ptr<boost::asio::io_service::work> worker;
  worker = std::make_unique<boost::asio::io_service::work>(service);
  boost::thread_group pool;

  for (int i = 0; i < 4; i++)
  {
    pool.create_thread([&service]() { service.run(); });
  }

  for (int i = 0; i < 20; i++)
  {
    service.post([i] {
      std::cout << i << std::endl;
      boost::this_thread::sleep(boost::posix_time::seconds(1));
    });
  }

  worker.reset();
  pool.join_all();

  return 0;
}
*/

/*
// Test thread pool
int main()
{
  boost::asio::io_service service;

  for (int i = 0; i < 20; i++)
  {
    service.post([i] {
      std::cout << i << std::endl;
      boost::this_thread::sleep(boost::posix_time::seconds(1));
    });
  }

  boost::thread_group pool;
  for (int i = 0; i < 4; i++)
  {
    pool.create_thread([&service]() { service.run(); });
  }

  pool.join_all();

  return 0;
}
*/

/*
// Test Post
int main()
{
  boost::asio::io_service service;
  service.post([] { std::cout << "Hello, world!" << std::endl; });
  std::cout << "Greetings: " << std::endl;
  service.run();
  return 0;
}
*/
