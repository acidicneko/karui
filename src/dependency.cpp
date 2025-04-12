#include <cstdlib>
#include <ctime>
#include <dependency.hpp>
#include <meta.hpp>
#include <sys/stat.h>
#include <time.h>
#include <utils.hpp>

unsigned long getEpochModifiedTime(std::string Filename) {
  //struct stat *FileStat = (struct stat *)malloc(sizeof(struct stat));
  struct stat* FileStat = new struct stat;
  if (stat(Filename.c_str(), FileStat) != 0) {
    return 0;
  }
  time_t CurrentModified = FileStat->st_mtime;
  delete FileStat; 
  return (unsigned long)CurrentModified;
}

bool dependency::CheckModified(std::string srcFilename, std::string objFilename) {
  unsigned long SrcModified = getEpochModifiedTime(srcFilename);
  unsigned long ObjModified = getEpochModifiedTime(objFilename);
  if (SrcModified > ObjModified) {
    return true;
  }
  return false;
}
