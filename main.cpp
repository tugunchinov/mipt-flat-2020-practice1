/*
  19. Даны \alpha и буква x. Найти максимальное k, такое что в L есть слова,
      заканчивающиеся на x^k.
  Examples:
  1. Input: ab+c.aba.*.bac.+.+* b
     Output: 1
  2. Input: acb..bab.c.*.ab.ba.+.+*a. b
     Output: 0
 */

#include <iostream>
#include <stack>
#include <string>
#include <string_view>

int FindMaxCharEndingLength(std::string_view regex, char x);

int main() {
  std::string regex;
  std::cin >> regex;
  char x;
  std::cin >> x;

  auto result = FindMaxCharEndingLength(regex, x);
  if (result != -1) {
    std::cout << result << std::endl;
  } else {
    std::cout << "INF" << std::endl;
  }

  return 0;
}

struct RegexProperties {
  int max_end_length;
  bool starts_from_beginning;
  bool has_empty_word;
  bool has_word_in_char_only;
};

void ProcessEmptyWord(std::stack<RegexProperties>& stack);
void ProcessSymbol(char x, char ch, std::stack<RegexProperties>& stack);
void ProcessPlus(std::stack<RegexProperties>& stack);
void ProcessDot(std::stack<RegexProperties>& stack);
void ProcessKleeneStar(std::stack<RegexProperties>& stack);
void HandleError();

int FindMaxCharEndingLength(std::string_view regex, char x) {
  std::stack<RegexProperties> stack;
  for (auto ch : regex) {
    if (ch == '1') {
      ProcessEmptyWord(stack);
    } else if (isalpha(ch)) {
      ProcessSymbol(x, ch, stack);
    } else if (ch == '+') {
      if (stack.size() < 2) {
        HandleError();
      }
      ProcessPlus(stack);
    } else if (ch == '.') {
      if (stack.size() < 2) {
        HandleError();
      }
      ProcessDot(stack);
    } else if (ch == '*') {
      if (stack.empty()) {
        HandleError();
      }
      ProcessKleeneStar(stack);
    } else {
      HandleError();
    }
  }
  if (stack.size() > 1) {
    HandleError();
  }
  return stack.top().max_end_length;
}

void HandleError() {
  std::cerr << "ERROR" << std::endl;
  exit(1);
}

void ProcessKleeneStar(std::stack<RegexProperties>& stack) {
  auto regex = stack.top();
  stack.pop();
  if (regex.has_word_in_char_only) {
    stack.push({-1, true, true, true});
  } else {
    stack.push({regex.max_end_length,
                regex.starts_from_beginning,
                true,
                false});
  }
}

void ProcessDot(std::stack<RegexProperties>& stack) {
  auto regex2 = stack.top();
  stack.pop();
  auto regex1 = stack.top();
  stack.pop();
  bool has_empty_word =  regex1.has_empty_word && regex2.has_empty_word;
  bool has_word_in_char_only =
      (regex2.has_empty_word && regex1.has_word_in_char_only) ||
      (regex1.has_empty_word && regex2.has_word_in_char_only) ||
      (regex1.has_word_in_char_only && regex2.has_word_in_char_only);
  if (regex2.starts_from_beginning) {
    bool starts_from_beginning = regex1.starts_from_beginning;
    stack.push({regex1.max_end_length + regex2.max_end_length,
                starts_from_beginning,
                has_empty_word,
                has_word_in_char_only});
  } else if (regex2.has_empty_word) {
    bool starts_from_beginning =
        (regex1.max_end_length >= regex2.max_end_length) ?:
        regex1.starts_from_beginning;
    stack.push({std::max(regex1.max_end_length, regex2.max_end_length),
                starts_from_beginning,
                has_empty_word,
                has_word_in_char_only});
  } else {
      stack.push({regex2.max_end_length,
                  false,
                  has_empty_word,
                  has_word_in_char_only});
  }
}

void ProcessPlus(std::stack<RegexProperties>& stack) {
  auto regex2 = stack.top();
  stack.pop();
  auto regex1 = stack.top();
  stack.pop();
  int max_end_length = std::max(regex1.max_end_length, regex2.max_end_length);
  bool starts_from_beginning = max_end_length == regex1.max_end_length ?
      regex1.starts_from_beginning : regex2.starts_from_beginning;
  bool has_empty_word = max_end_length == regex1.max_end_length ?
      regex1.has_empty_word : regex2.has_empty_word;
  bool has_word_in_char_only =
      regex1.has_word_in_char_only || regex2.has_word_in_char_only;
  stack.push({max_end_length,
              starts_from_beginning,
              has_empty_word,
              has_word_in_char_only});
}

void ProcessSymbol(char x, char ch, std::stack<RegexProperties>& stack) {
  if (ch == x) {
    stack.push({1, true, false, true});
  } else {
    stack.push({0, false, false, false});
  }
}

void ProcessEmptyWord(std::stack<RegexProperties>& stack) {
  stack.push({0, false, true, false});
}
