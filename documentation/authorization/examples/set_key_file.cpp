#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyFile("C:/some/folder/key.dat")) {
    // ...
  }
}
