#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetAzureKeyFile("C:/some/folder/key.dat")) {
    // ...
  }
}
