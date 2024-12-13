#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <valarray>

enum commands { echo, cd, quit, type, invalid };

commands string_to_commands(std::string str) {
  if (str.find("echo") != std::string::npos)
    return echo;

  if (str.find("cd") != std::string::npos)
    return cd;

  if (str.find("exit") != std::string::npos)
    return quit;

  if (str.find("type") != std::string::npos)
    return type;

  return invalid;
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
        std::cout << input.substr(5) << "is a shell builtin \n";
      else
        std::cout << input.substr(5) << " not found" << '\n';
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
