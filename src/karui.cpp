#include <cstdlib>
#include <fstream>
#include <iostream>
#include <karui.hpp>
#include <string>
#include <vector>
#include <yaml.h>

void karui::builder::ParserConfig(std::string ConfigFileLocation) {
  std::ifstream yamlFile(ConfigFileLocation);
  if (!yamlFile.is_open()) {
    std::cout << "Failed to open configuration file: " << ConfigFileLocation
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string yamlString((std::istreambuf_iterator<char>(yamlFile)),
                         std::istreambuf_iterator<char>());
  yamlFile.close();

  yaml_parser_t parser;
  yaml_document_t document;

  if (!yaml_parser_initialize(&parser)) {
    std::cout << "Failed to initialize YAML parser!" << std::endl;
    exit(EXIT_FAILURE);
  }

  yaml_parser_set_input_string(
      &parser, (const unsigned char *)yamlString.c_str(), yamlString.length());

  if (!yaml_parser_load(&parser, &document)) {
    std::cout << "Failed to parse YAML configuration!" << std::endl;
    yaml_parser_delete(&parser);
    exit(EXIT_FAILURE);
  }

  yaml_node_t *root = yaml_document_get_root_node(&document);
  if (!root || root->type != YAML_MAPPING_NODE) {
    std::cout << "Invalid YAML structure - expected mapping at root!"
              << std::endl;
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    exit(EXIT_FAILURE);
  }

  auto findKey = [&](const char *key) -> yaml_node_t * {
    for (yaml_node_pair_t *pair = root->data.mapping.pairs.start;
         pair < root->data.mapping.pairs.top; pair++) {
      yaml_node_t *key_node = yaml_document_get_node(&document, pair->key);
      if (key_node && key_node->type == YAML_SCALAR_NODE) {
        if (strcmp((char *)key_node->data.scalar.value, key) == 0) {
          return yaml_document_get_node(&document, pair->value);
        }
      }
    }
    return nullptr;
  };

  yaml_node_t *threads_node = findKey("threads");
  if (threads_node && threads_node->type == YAML_SCALAR_NODE) {
    this->threads = std::atoi((char *)threads_node->data.scalar.value);
  }

  yaml_node_t *buildDir_node = findKey("buildDir");
  if (buildDir_node && buildDir_node->type == YAML_SCALAR_NODE) {
    this->buildFolder = std::string((char *)buildDir_node->data.scalar.value);
  } else {
    std::cout << "No build directory specified!\n\"buildDir\" not defined in "
                 "configuration!\n";
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    exit(EXIT_FAILURE);
  }

  yaml_node_t *srcDir_node = findKey("srcDir");
  if (srcDir_node && srcDir_node->type == YAML_SCALAR_NODE) {
    this->srcFolder = std::string((char *)srcDir_node->data.scalar.value);
  } else {
    std::cout << "No src directory specified!\n\"srcDir\" not defined in "
                 "configuration!\n";
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    exit(EXIT_FAILURE);
  }

  yaml_node_t *compiler_node = findKey("cc");
  if (compiler_node && compiler_node->type == YAML_SCALAR_NODE) {
    this->compiler = std::string((char *)compiler_node->data.scalar.value);
  } else {
    std::cout
        << "No compiler specified!\n\"cc\" not defined in configuration!\n";
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    exit(EXIT_FAILURE);
  }

  yaml_node_t *target_node = findKey("target");
  if (target_node && target_node->type == YAML_SCALAR_NODE) {
    this->target = std::string((char *)target_node->data.scalar.value);
  } else {
    std::cout
        << "No target specified!\n\"target\" not defined in configuration!\n";
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    exit(EXIT_FAILURE);
  }

  yaml_node_t *flags_node = findKey("flags");
  if (flags_node && flags_node->type == YAML_SEQUENCE_NODE) {
    for (yaml_node_item_t *item = flags_node->data.sequence.items.start;
         item < flags_node->data.sequence.items.top; item++) {
      yaml_node_t *flag_node = yaml_document_get_node(&document, *item);
      if (flag_node && flag_node->type == YAML_SCALAR_NODE) {
        this->compilerOptions.push_back(
            std::string((char *)flag_node->data.scalar.value));
      }
    }
  }

  yaml_node_t *ldflags_node = findKey("ldflags");
  if (ldflags_node && ldflags_node->type == YAML_SEQUENCE_NODE) {
    for (yaml_node_item_t *item = ldflags_node->data.sequence.items.start;
         item < ldflags_node->data.sequence.items.top; item++) {
      yaml_node_t *ldflag_node = yaml_document_get_node(&document, *item);
      if (ldflag_node && ldflag_node->type == YAML_SCALAR_NODE) {
        this->linkerOptions.push_back(
            std::string((char *)ldflag_node->data.scalar.value));
      }
    }
  }

  yaml_document_delete(&document);
  yaml_parser_delete(&parser);
}
