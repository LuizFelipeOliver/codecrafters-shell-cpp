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

void replace_char(string &str, const char replace_char) {

  size_t pos = str.find(replace_char);

  while (pos != string::npos) {
    str.erase(pos, 1);
    pos = str.find(replace_char, pos);
  };
}

enum commands { pwd, type, echo, cd, quit, invalid };

unordered_map<string, commands> command_map = {
    {"type", type}, {"echo", echo}, {"cd", cd}, {"exit", quit}, {"pwd", pwd}};

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
    string arg;
    if (input.size() > command.size() + 1)
      arg = input.substr(command.size() + 1);

    string not_found = " not found\n";

    switch (string_to_commands(command)) {
    case echo: {

      if (arg.front() == '\'' && arg.back() == '\'') {

        replace_char(arg, '\'');

        stringstream ss(arg);
        string word;
        string result;

        bool first_word = true;

        while (ss >> word) {
          if (!first_word) {
            result += " ";
          }
          result += word;
          first_word = false;
        }
        cout << result << "\n";
      }

      replace_char(arg, '\"');

      cout << arg << "\n";
      break;
    }
    case type: {
      string path_command = get_path(arg);

      if (string_to_commands(arg) != invalid) {
        cout << arg << " is a shell builtin\n";
      } else if (!path_command.empty()) {
        cout << arg << " is " << path_command << std::endl;
      } else {
        cout << arg << ":" << not_found;
      }
      break;
    }
    case quit: {
      return 0;
      break;
    }
    case invalid: {
      if (!get_path(command).empty()) {
        string full_command = command + " " + arg;
        int result = system(full_command.c_str());
      }

      if (get_path(command).empty())
        cout << command << ": command" << not_found;
      break;
    }
    case pwd: {
      cout << filesystem::current_path().string() << endl;
      break;
    }
    case cd: {
      if (arg == "~") {
        const char *home_dir = getenv("HOME");
        if (chdir(home_dir) != 0) {
        }
      } else {

        if (chdir(arg.c_str()) != 0) {
          cout << arg << ": No such file or directory\n";
        }
      }
      break;
    }
    default: {
      cout << command << ": command" << not_found;
      break;
    }
    }
  }
  return 0;
}
