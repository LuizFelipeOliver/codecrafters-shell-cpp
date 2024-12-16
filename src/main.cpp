#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>

using namespace std;

string get_path(string command) {
  string path_env = getenv("PATH");

  stringstream ss(path_env);

  string path;
  while (getline(ss, path, ':')) {

    string abs_path = path + "/" + command;
    if (filesystem::exists(abs_path)) {
      return abs_path;
    }
  }
  return "";
}

enum commands { type, echo, cd, quit, invalid };

unordered_map<string, commands> command_map = {
    {"type", type}, {"echo", echo}, {"cd", cd}, {"exit", quit}};

commands string_to_commands(const string &str) {
  auto it = command_map.find(str);
  return it != command_map.end() ? it->second : invalid;
}

int main() {
  // Flush after every std::cout / std:cerr
  cout << std::unitbuf;
  cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  string input;

  while (true) {
    cout << "$ ";
    getline(std::cin, input);

    if (input.empty())
      continue;

    istringstream iss(input);
    string command;

    iss >> command;
    string arg = input.substr(command.size() + 1);

    string not_found = " not found\n";

    string path_command = get_path(command);

    switch (string_to_commands(command)) {

    case echo:
      cout << arg << "\n";
      break;

    case type:
      if (string_to_commands(arg) != invalid) {
        cout << arg << " is a shell builtin\n";
      } else if (!path_command.empty()) {
        cout << arg << " is " << path_command << std::endl;
      } else {
        cout << arg << ":" << not_found;
      }
      break;

    case quit:
      return 0;
      break;
    case invalid:
      if (!path_command.empty()) {
        string full_command = command + "" + arg;
        int result = system(full_command.c_str());
        cout << 0;
      }

      break;
    default:
      cout << input << ": command" << not_found;
      break;
    }
  }
  return 0;
}
