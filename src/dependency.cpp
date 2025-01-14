#include <cstdlib>
#include <ctime>
#include <dependency.hpp>
#include <encoder.hpp>
#include <meta.hpp>
#include <sys/stat.h>
#include <time.h>
#include <utils.hpp>

unsigned long getEpochModifiedTime(std::string Filename) {
  struct stat *FileStat = (struct stat *)malloc(sizeof(struct stat));
  stat(Filename.c_str(), FileStat);
  time_t CurrentModified = FileStat->st_mtime;
  free(FileStat);
  return (unsigned long)CurrentModified;
}

bool dependency::CheckModified(std::string Filename) {
  if (!utils::FileExists(DEFAULT_META_FILE)) {
    return false; // if file doesn't exist, it's not modified
  }
  if (!find(DEFAULT_META_FILE, Filename)) {
    return false;
  }
  unsigned long CurrentModified = getEpochModifiedTime(Filename);
  unsigned long LastModified =
      std::atoi(get_value(DEFAULT_META_FILE, Filename).c_str());
  if (CurrentModified > LastModified) {
    return true;
  }
  return false;
}

void dependency::UpdateMeta(std::string Filename) {
  unsigned long CurrentModified = getEpochModifiedTime(Filename);
  create(DEFAULT_META_FILE, Filename, std::to_string(CurrentModified));
}
