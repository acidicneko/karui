#pragma once

#define VERSION "1.0.0"
#define DEFAULT_THREAD_COUNT 4

#include <stddef.h>
#include <thread>

static inline size_t GetThreads() {
  size_t t = std::thread::hardware_concurrency();
  if (t == 0) {
    return DEFAULT_THREAD_COUNT;
  }
  return t;
}
