#include <cJSON/cJSON.h>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <karui.hpp>

void karui::builder::ParserConfig(std::string ConfigFileLocation) {
  std::ifstream jsonFile(ConfigFileLocation);
  std::string jsonString((std::istreambuf_iterator<char>(jsonFile)),
                         std::istreambuf_iterator<char>());
  cJSON *json = cJSON_Parse(jsonString.c_str());
  cJSON *threads = cJSON_GetObjectItem(json, "threads");
  if (threads != NULL) {
    this->threads = threads->valueint;
  }

  cJSON *buildFolder = cJSON_GetObjectItem(json, "buildDir");
  if (buildFolder != NULL) {
    this->buildFolder = buildFolder->valuestring;
  } else {
    std::cout << "No build directory specified!\n\"buildDir\" not defined in configuration!\n";
    exit(EXIT_FAILURE);
  }

  cJSON *srcFolder = cJSON_GetObjectItem(json, "srcDir");
  if (srcFolder != NULL) {
    this->srcFolder = srcFolder->valuestring;
  } else {
    std::cout << "No src directory specified!\n\"srcDir\" not defined in configuration!\n";
    exit(EXIT_FAILURE);
  }


  cJSON *compiler = cJSON_GetObjectItem(json, "cc");
  if (compiler != NULL) {
    this->compiler = compiler->valuestring;
  } else {
    std::cout << "No compiler specified!\n\"cc\" not defined in configuration!\n";
    exit(EXIT_FAILURE);
  }

  cJSON *target = cJSON_GetObjectItem(json, "target");
  if (target != NULL) {
    this->target = target->valuestring;
  } else {
    std::cout << "No target specified!\n\"target\" not defined in configuration!\n";
    exit(EXIT_FAILURE);
  }


  cJSON *flags = cJSON_GetObjectItem(json, "flags");
  cJSON *flag = NULL;
  cJSON_ArrayForEach(flag, flags) {
    this->compilerOptions.push_back(flag->valuestring);
  }
  cJSON *ldflags = cJSON_GetObjectItem(json, "ldflags");
  cJSON *ldflag = NULL;
  cJSON_ArrayForEach(ldflag, ldflags) {
    this->linkerOptions.push_back(ldflag->valuestring);
  }
}
