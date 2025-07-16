#include "timer.h"

using high_res = std::chrono::high_resolution_clock; 

Timer::Timer()
: m_start{high_res::now()}
{}

void Timer::start() {
  m_start = high_res::now(); 
}

void Timer::reset() {
  m_start = high_res::now();
}

double Timer::elapsed_milliseconds() {
  auto now = high_res::now();

  return std::chrono::duration<double, std::milli>(now - m_start).count();
}
