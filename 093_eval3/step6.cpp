#include <stdexcept>

int compare(int a, int b) {
  if (a < 0 || b < 0) {
    throw std::invalid_argument("received negative value");
  }
  return a + b;
}

int main() {
  try {
    compare(-1, 3);
  }
  catch (const std::invalid_argument & e) {
    // do stuff with exception...
  }
}
