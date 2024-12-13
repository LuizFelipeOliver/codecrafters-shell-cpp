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

    if (input == "exit 0")
      std::cout << input << ": command not found\n";
    return 0;

    if (input == "echo") {
      std::cout << input.substr(5);
      return 0;
    }

  } while (true);
}
