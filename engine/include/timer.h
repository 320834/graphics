#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

class Timer {
public:
  Timer();

  void start();
  void reset();

  double elapsed_milliseconds();

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

#endif // TIMER_H_
