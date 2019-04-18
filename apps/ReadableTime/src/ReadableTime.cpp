// Copyright 2017 Caoyang Jiang

#include <ctime>
#include <iostream>
#include <string>

class Time
{
 public:
  enum TimeZone
  {
    UTC,
    LOCAL,
  };

  static std::string Get_Readable_Time(enum Time::TimeZone zone)
  {
    time_t raw_time;
    struct tm* ptm = nullptr;
    std::string time_string;
    std::time(&raw_time);

    switch (zone)
    {
      case TimeZone::LOCAL:
        ptm = std::localtime(&raw_time);
        time_string += "Local_";
        break;
      case TimeZone::UTC:
        ptm = std::gmtime(&raw_time);
        time_string += "UTC_";
        break;
      default: break;
    }

    time_string += std::to_string(ptm->tm_year + 1900);
    time_string += "_";
    time_string += std::to_string(ptm->tm_mon);
    time_string += "_";
    time_string += std::to_string(ptm->tm_mday);
    time_string += "_";
    time_string += std::to_string(ptm->tm_hour);
    time_string += "_";
    time_string += std::to_string(ptm->tm_min);
    time_string += "_";
    time_string += std::to_string(ptm->tm_sec);

    return time_string;
  }
};

int main()
{
  std::cout << Time::Get_Readable_Time(Time::TimeZone::LOCAL) << std::endl;
}
