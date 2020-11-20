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

enum {
  INFINITY = -1,
  ERROR = -2,
  NOLEN = -3
};

int FindMaxCharEndingLength(std::string_view regex, char x);

int main() {
  std::string regex;
  std::cin >> regex;
  char x;
  std::cin >> x;

  auto result = FindMaxCharEndingLength(regex, x);
  if (result == ERROR) {
    std::cerr << "Error!" << std::endl;
    return 1;
  } else if (result == INFINITY) {
    std::cout << "Infinity" << std::endl;
  } else {
    std::cout << result << std::endl;
  }
  return 0;
}

int max(int lhs, int rhs) {
  if (lhs == INFINITY) {
    return lhs;
  }
  if (rhs == INFINITY) {
    return rhs;
  }
  return std::max(lhs, rhs);
}

struct RegexProperties {
  int max_end_len;
  int x_word_max_len;
};

void ProcessEmptyWord(std::stack<RegexProperties>& stack);
void ProcessSymbol(char x, char ch, std::stack<RegexProperties>& stack);
void ProcessPlus(std::stack<RegexProperties>& stack);
void ProcessDot(std::stack<RegexProperties>& stack);
void ProcessKleeneStar(std::stack<RegexProperties>& stack);

int FindMaxCharEndingLength(std::string_view regex, char x) {
  std::stack<RegexProperties> stack;
  for (auto ch : regex) {
    if (ch == '1') {
      ProcessEmptyWord(stack);
    } else if (isalpha(ch)) {
      ProcessSymbol(x, ch, stack);
    } else if (ch == '+') {
      if (stack.size() < 2) {
        return ERROR;
      }
      ProcessPlus(stack);
    } else if (ch == '.') {
      if (stack.size() < 2) {
        return ERROR;
      }
      ProcessDot(stack);
    } else if (ch == '*') {
      if (stack.empty()) {
        return ERROR;
      }
      ProcessKleeneStar(stack);
    } else {
      return ERROR;
    }
  }
  if (stack.size() > 1) {
    return ERROR;
  }
  return stack.top().max_end_len;
}

void ProcessKleeneStar(std::stack<RegexProperties>& stack) {
  auto regex = stack.top();
  stack.pop();
  if (regex.x_word_max_len > 0 || regex.x_word_max_len == INFINITY) {
    stack.push({INFINITY, INFINITY});
  } else {
    stack.push({regex.max_end_len, 0});
  }
}

void ProcessDot(std::stack<RegexProperties>& stack) {
  auto rhs = stack.top();
  stack.pop();
  auto lhs = stack.top();
  stack.pop();
  int x_word_max_len = NOLEN;
  if (lhs.x_word_max_len != NOLEN && rhs.x_word_max_len != NOLEN) {
    if (lhs.x_word_max_len == INFINITY || rhs.x_word_max_len == INFINITY) {
      x_word_max_len = INFINITY;
    } else {
      x_word_max_len = lhs.x_word_max_len + rhs.x_word_max_len;
    }
  }
  if (rhs.x_word_max_len == NOLEN) {
    stack.push(rhs);
  } else {
    if (lhs.max_end_len == INFINITY) {
      stack.push({INFINITY, x_word_max_len});
    } else {
      stack.push({max(lhs.max_end_len + rhs.x_word_max_len, rhs.max_end_len),
                 x_word_max_len});
    }
  }
}

void ProcessPlus(std::stack<RegexProperties>& stack) {
  auto rhs = stack.top();
  stack.pop();
  auto lhs = stack.top();
  stack.pop();
  int max_end_len = max(lhs.max_end_len, rhs.max_end_len);
  int x_word_max_len = max(lhs.x_word_max_len, rhs.x_word_max_len);
  stack.push({max_end_len, x_word_max_len});
}

void ProcessSymbol(char x, char ch, std::stack<RegexProperties>& stack) {
  if (ch == x) {
    stack.push({1, 1});
  } else {
    stack.push({0, NOLEN});
  }
}

void ProcessEmptyWord(std::stack<RegexProperties>& stack) {
  stack.push({0, 0});
}
