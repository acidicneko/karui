#include <cstring>
#include <encoder.hpp>
#include <fstream>
#include <iostream>
#include <stdio.h>

// CLASS functions definition
data::data(std::string _key_, std::string _value_) {
  strcpy(key, _key_.c_str());
  strcpy(value, _value_.c_str());
}

void data::print_info() {
  std::cout << "Key: " << key << "\tValue: " << value << std::endl;
}

int is(std::string filename) {
  std::ifstream obj(filename);
  if (!obj) {
    obj.close();
    return 0;
  }
  obj.close();
  return 1;
}

// functions definition
bool find(std::string filename, std::string _key_) {
  std::fstream f(filename, std::ios::in | std::ios::binary);
  data obj;
  while (f.read((char *)&obj, sizeof(obj))) {
    if (strcmp(obj.key, _key_.c_str()) == 0)
      return true;
  }
  return false;
}

void create(std::string filename, std::string _key_, std::string _value_) {
  if (find(filename, _key_)) {
    /*std::cout << "data with key: " << _key_ << ", already exists\n";
    return;*/
    // std::cout << "updating key: \"" << _key_ << "\"" << std::endl;
    update_value(filename, _key_, _value_);
    return;
  }
  data obj(_key_, _value_);
  std::ofstream f(filename, std::ios::binary | std::ios::app);
  f.write((char *)&obj, sizeof(obj));
  f.close();
}

void read(std::string filename) {
  std::ifstream f(filename, std::ios::binary);
  data obj;
  while (f.read((char *)&obj, sizeof(obj)))
    obj.print_info();
  f.close();
}

void update_value(std::string filename, std::string _key_,
                  std::string _value_) {
  std::fstream f(filename, std::ios::in | std::ios::out | std::ios::binary);
  data obj;
  bool found = false;
  while (f.read((char *)&obj, sizeof(obj))) {
    if (strcmp(obj.key, _key_.c_str()) == 0) {
      found = true;
      strcpy(obj.value, _value_.c_str());
      f.seekp((unsigned long)f.tellg() - sizeof(obj), std::ios::beg);
      f.write((char *)&obj, sizeof(obj));
    }
  }
  if (!found)
    std::cout << "key with name: " << _key_ << " doesn't exists\n";
}

std::string get_value(std::string filename, std::string _key_) {
  if (!find(filename, _key_)) {
    return "";
  }
  std::fstream f(filename, std::ios::in | std::ios::binary);
  data obj;
  std::string __value;
  while (f.read((char *)&obj, sizeof(obj))) {
    if (strcmp(obj.key, _key_.c_str()) == 0) {
      __value = obj.value;
      return __value;
    }
  }
  return 0;
}

void del(std::string filename, std::string _key_) {
  if (!is(filename))
    return;
  if (!find(filename, _key_)) {
    std::cout << "key " << _key_ << "\' doesn't exists!\n";
    return;
  }
  data obj;
  std::ifstream f(filename, std::ios::binary);
  std::ofstream temp(".temp", std::ios::binary);

  while (f.read((char *)&obj, sizeof(obj))) {
    if (strcmp(obj.key, _key_.c_str())) {
      temp.write((char *)&obj, sizeof(obj));
    }
  }

  f.close();
  temp.close();
  remove(filename.c_str());
  rename(".temp", filename.c_str());
}
