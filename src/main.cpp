#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

enum commands { type, echo, cd, quit, invalid };

commands string_to_commands(std::string str) {
  if (str.find("type") != std::string::npos)
    return type;

  if (str.find("echo") != std::string::npos)
    return echo;

  if (str.find("cd") != std::string::npos)
    return cd;

  if (str.find("exit") != std::string::npos)
    return quit;

  return invalid;
}

std::string get_path(std::string command) {
  const char *env_path = std::getenv("PATH");
  if (!env_path) {
    return "";
  }

  std::stringstream ss(env_path);
  std::string path;

  while (ss.eof()) {
    std::filesystem::path abs_path = std::filesystem::path(path) / command;

    if (std::filesystem::exists(abs_path)) {
      return abs_path;
    }
  }

  return "";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  std::string input = " ";

  while (!input.empty()) {
    std::cout << "$ ";
    std::getline(std::cin, input);

    switch (string_to_commands(input)) {

    case echo:
      std::cout << input.substr(5) << "\n";
      break;

    case type:
      if (string_to_commands(input.substr(5)) != invalid)
        std::cout << input.substr(5) << " is a shell builtin\n";
      else
        std::cout << input.substr(5) << " in " << get_path(input.substr(5))
                  << std::endl;
      break;

    case quit:
      return 0;
      break;

    default:
      std::cout << input << ": command not found" << '\n';
      break;
    }
  }
  return 0;
}
