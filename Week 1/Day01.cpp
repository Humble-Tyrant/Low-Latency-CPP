#include <iostream>
using namespace std;
class Calculator {
  int a;
  int b;

public:
  Calculator(int a, int b) {
    this->a = a;
    this->b = b;
  }
  int add() { return a + b; }
  int sub() { return a - b; }
};

int main() {
  Calculator c(5, 6);
  cout << c.add() << endl;
  cout << c.sub();
  return 0;
}