#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <string>

class data {
public:
  char key[1024];
  char value[1024];
  data() {}
  data(std::string _key_, std::string _value_);
  void print_info();
};

bool find(std::string filename, std::string _key_);
void create(std::string filename, std::string _key_, std::string _value_);
void read(std::string filename);
void update_value(std::string filename, std::string _key_, std::string _value_);
void del(std::string filename, std::string _key_);
std::string get_value(std::string filename, std::string _key_);

#endif
