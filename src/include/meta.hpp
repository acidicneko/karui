#pragma once

#define VERSION "2.2.2"
#define DEFAULT_THREAD_COUNT 4
#define DEFAULT_CONFIG_FILE "karui.yaml"

#include <stddef.h>
#include <thread>

static inline size_t GetThreads() {
  size_t t = std::thread::hardware_concurrency();
  if (t == 0) {
    return DEFAULT_THREAD_COUNT;
  }
  return t;
}
