#include <cJSON/cJSON.h>
#include <cstddef>
#include <fstream>
#include <scbuild.hpp>

void scbuild::builder::ParserConfig(std::string ConfigFileLocation) {
  std::ifstream jsonFile(ConfigFileLocation);
  std::string jsonString((std::istreambuf_iterator<char>(jsonFile)),
                         std::istreambuf_iterator<char>());
  cJSON *json = cJSON_Parse(jsonString.c_str());
  cJSON *threads = cJSON_GetObjectItem(json, "threads");
  if (threads != NULL) {
    this->threads = threads->valueint;
    ;
  }
  cJSON *buildFolder = cJSON_GetObjectItem(json, "buildDir");
  if (buildFolder != NULL) {
    this->buildFolder = buildFolder->valuestring;
  }
  cJSON *compiler = cJSON_GetObjectItem(json, "cc");
  if (compiler != NULL) {
    this->compiler = compiler->valuestring;
  }
  cJSON *target = cJSON_GetObjectItem(json, "target");
  if (target != NULL) {
    this->target = target->valuestring;
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
