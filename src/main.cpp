#include <cstring>
#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  std::cout << "$ ";

  std::string input;
  std::getline(std::cin, input);
  char *token = std::strtok(&input[0], " ");

  char *command = token;

  if (std::strcmp(command, "exit 0") == 0) {
    return 0;
    std::cout << input << ": command not found\n";
  }

  std::cout << input << ": command not found\n";
}
