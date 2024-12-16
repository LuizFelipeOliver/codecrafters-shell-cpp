#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

using namespace std;

enum commands { type, echo, cd, quit, invalid };

commands string_to_commands(string str) {
  if (str.find("type") != string::npos)
    return type;

  if (str.find("echo") != string::npos)
    return echo;

  if (str.find("cd") != string::npos)
    return cd;

  if (str.find("exit") != string::npos)
    return quit;

  return invalid;
}

string get_path(string command) {
  string path_env = getenv("PATH");

  stringstream ss(path_env);

  string path;
  while (!ss.eof()) {

    getline(ss, path, ':');

    string abs_path = path + "/" + command;
    if (filesystem::exists(abs_path)) {
      return abs_path;
    }
  }
  return "";
}

int main() {
  // Flush after every std::cout / std:cerr
  cout << std::unitbuf;
  cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  string input = " ";

  while (!input.empty()) {
    cout << "$ ";
    getline(std::cin, input);

    string command = input.substr(5);
    string not_found = " not found\n";

    string path_command = get_path(command);

    switch (string_to_commands(input)) {

    case echo:
      cout << command << "\n";
      break;

    case type:
      if (string_to_commands(command) != invalid) {
        cout << command << " is a shell builtin\n";
      } else if (!path_command.empty()) {
        cout << command << " is " << path_command << std::endl;
      } else {
        cout << command << ":" << not_found;
      }
      break;

    case quit:
      return 0;
      break;

    default:
      cout << path_command << input << ": command" << not_found;
      break;
    }
  }
  return 0;
}
