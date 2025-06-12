#include <build.hpp>
#include <compiler.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <karui.hpp>
#include <string>
#include <utils.hpp>
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

  yaml_node_t *hooks_node = findKey("hooks");
  if (hooks_node && hooks_node->type == YAML_SEQUENCE_NODE) {
    for (yaml_node_item_t *item = hooks_node->data.sequence.items.start;
         item < hooks_node->data.sequence.items.top; item++) {
      yaml_node_t *hook_node = yaml_document_get_node(&document, *item);
      if (!hook_node || hook_node->type != YAML_MAPPING_NODE)
        continue;

      karui::hook new_hook;

      for (yaml_node_pair_t *pair = hook_node->data.mapping.pairs.start;
           pair < hook_node->data.mapping.pairs.top; pair++) {
        yaml_node_t *key_node = yaml_document_get_node(&document, pair->key);
        yaml_node_t *value_node =
            yaml_document_get_node(&document, pair->value);

        if (!key_node || key_node->type != YAML_SCALAR_NODE)
          continue;
        std::string keyStr((char *)key_node->data.scalar.value);

        if (keyStr == "name" && value_node &&
            value_node->type == YAML_SCALAR_NODE) {
          new_hook.name = std::string((char *)value_node->data.scalar.value);
        } else if (keyStr == "commands" && value_node &&
                   value_node->type == YAML_SEQUENCE_NODE) {
          for (yaml_node_item_t *cmd_item =
                   value_node->data.sequence.items.start;
               cmd_item < value_node->data.sequence.items.top; cmd_item++) {
            yaml_node_t *cmd_node =
                yaml_document_get_node(&document, *cmd_item);
            if (cmd_node && cmd_node->type == YAML_SCALAR_NODE) {
              new_hook.commands.push_back(
                  std::string((char *)cmd_node->data.scalar.value));
            }
          }
        }
      }

      if (!new_hook.name.empty()) {
        this->hooks.push_back(new_hook);
      }
    }
  }

  yaml_node_t *workflow_node = findKey("workflow");
  if (workflow_node && workflow_node->type == YAML_SEQUENCE_NODE) {
    for (yaml_node_item_t *item = workflow_node->data.sequence.items.start;
         item < workflow_node->data.sequence.items.top; item++) {
      yaml_node_t *workflow_item = yaml_document_get_node(&document, *item);
      if (workflow_item && workflow_item->type == YAML_SCALAR_NODE) {
        this->workflow.push_back(
            std::string((char *)workflow_item->data.scalar.value));
      }
    }
  }

  yaml_node_t *include_node = findKey("extraObjs");
  if (include_node && include_node->type == YAML_SEQUENCE_NODE) {
    for (yaml_node_item_t *item = include_node->data.sequence.items.start;
         item < include_node->data.sequence.items.top; item++) {

      yaml_node_t *include_item = yaml_document_get_node(&document, *item);
      if (!include_item) {
        std::cerr << "YAML error: null item in sequence." << std::endl;
        continue;
      }

      if (include_item->type != YAML_SCALAR_NODE) {
        std::cerr << "YAML error: non-scalar item in sequence." << std::endl;
        continue;
      }

      if (!include_item->data.scalar.value) {
        std::cerr << "YAML error: scalar value is null." << std::endl;
        continue;
      }

      this->extraObjs.push_back(
          std::string((char *)include_item->data.scalar.value));
    }
  }

  yaml_node_t *verbose_node = findKey("verbose");
  if (verbose_node && verbose_node->type == YAML_SCALAR_NODE) {
    std::string verboseStr((char *)verbose_node->data.scalar.value);
    this->verbose = (verboseStr == "true" || verboseStr == "1");
  } else {
    this->verbose = false; // Default to false if not specified
  }
  yaml_document_delete(&document);
  yaml_parser_delete(&parser);
}
