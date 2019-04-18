// Copyright 2017 Caoyang Jiang

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

class Event
{
 public:
  enum EventWaitType
  {
    EventWaitInvalid,  ///< This event is uninitialized.
    EventWaitDummy,    ///< The event to be waited on is a dummy event. Used to
                     ///< keep the index numbers straight, to avoid a O(n) pass
                     ///< through the whole event wait list.
    EventWaitSignal,  ///< The event to be waited on is not a socket-based
                      ///< event, but an application-triggered signal event.
    EventWaitRead,    ///< The event to be waited on is a socket read event.
    EventWaitWrite    ///< The event to be waited on is a socket write event.
  };

  /// Creates and returns a new event.
  static Event GenerateNewEvent(Event::EventWaitType type);

 public:
  Event(int fd, Event::EventWaitType eventType);
  Event();

  // Does not delete the event. Call Event::Close() when you don't need it
  // anymore.
  //  ~Event();

  /**
   * @brief       Initializes the event to a new instance. If the Event was
   * previously initialized, does *not* Close the old instance.
   *
   * @param[in]  type  The type
   */
  void Create(EventWaitType type);

  /**
   * @brief      Deinitializes the Event. If there exists any copies of this
   * Event, their state will be undefined.
   */
  void Close();

  /**
   * @brief      Determine if this event is uninitialized.
   *
   * @return     True if null, False otherwise.
   */
  bool IsNull() const;

  /**
   * @brief      Determines if valid.
   *
   * @return     True if valid, False otherwise.
   */
  bool IsValid() const;

  /**
   * @brief       Returns true if this event is not null.
   */
  void Reset();

  /**
   * @brief      Sets the event, i.e. sets it to "1".
   */
  void Set();

  /**
   * @brief      Determine if the event is in set state.
   *
   * @return     Returns true if the event is in set state.
   */
  bool Test() const;

  /**
   * @brief      Wait on the event
   *
   * @param[in]  msecs  The msecs
   *
   * @return     Returns true if the event was set, or got set during the
   * timeout period, or returns false if the event was not set before the
   * timeout occurred.
   */
  bool Wait(uint64_t msecs) const;

  ///

  /**
   * @brief      Get the underlying type of the event
   *
   * @return     Returns the underlying type of the event, specifying what kind
   * of system object is being represented by this event.
   */
  inline EventWaitType Type() const
  {
    return type;
  }

 private:
  EventWaitType type;

 public:
  int fd[2];
};
