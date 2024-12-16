#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

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

vector<string> split_string(const string &input) {
  stringstream ss(input);
  string world;
  vector<string> tokens;

  while (ss >> world) {
    tokens.push_back(world);
  }

  return tokens;
}

enum commands { program, type, echo, cd, quit, invalid };

commands string_to_commands(string str) {
  if (str == "type")
    return type;

  if (str == "echo")
    return echo;

  if (str == "cd")
    return cd;

  if (str == "exit")
    return quit;

  if (!get_path(str).empty())
    return program;

  return invalid;
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

    vector<string> tokens = split_string(input);

    string command = tokens[0];
    string args = input.substr(command.size() + 1);
    string not_found = " not found\n";

    string path_command = get_path(args);

    switch (string_to_commands(command)) {

    case echo:
      cout << args << "\n";
      break;

    case type:
      if (string_to_commands(args) != invalid) {
        cout << command << " is a shell builtin\n";
      } else if (!path_command.empty()) {
        cout << args << " is " << path_command << endl;
      } else {
        cout << args << ":" << not_found;
      }
      break;

    case program:
      system(command.c_str());

      break;
    case quit:
      return 0;
      break;

    default:
      cout << command << ": command" << not_found;
      break;
    }
  }
  return 0;
}
