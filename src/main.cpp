#include <cstdlib>
#include <cstring>
#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  do {
    // Uncomment this block to pass the first stage
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    char *token = std::strtok(&input[0], " ");

    char *command = token;

    if (std::strcmp(command, "exit 0") == 0)
      return 0;

    if (std::strcmp(command, "echo") == 0) {
      token = std::strtok(nullptr, " ");
      bool check_last_token = false;
      while (token != nullptr) {
        if (check_last_token) {
          std::cout << token;
        } else {
          std::cout << token << " ";
        }
        token = std::strtok(nullptr, "");
        if (token == nullptr) {
          check_last_token = true;
        }
      }
      std::cout << std::endl;
      return 0;
    }

    std::cout << input << ": command not found\n";

  } while (true);
}
