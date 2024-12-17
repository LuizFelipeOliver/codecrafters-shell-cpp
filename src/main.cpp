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
      bool inside_quotes;
      if ((arg.front() == '\'' || arg.front() == '\"') &&
          (arg.back() == '\'' || arg.back() == '\"')) {
        inside_quotes = true;
      }
      char replace_quotes = '\'';

      string replace_by = "";

      size_t pos = arg.find(replace_quotes);

      while (pos != string::npos) {
        arg.replace(pos, 1, replace_by);
        pos = arg.find(replace_quotes, pos + 1);
      };

      if (inside_quotes) {

        stringstream ss(arg);
        string word;
        string result;
        while (ss >> word) {
          if (!result.empty()) {
            result += " ";
          }
          result += word;
        }
        cout << result << "\n";
      } else {
        cout << arg << "\n";
      }
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
