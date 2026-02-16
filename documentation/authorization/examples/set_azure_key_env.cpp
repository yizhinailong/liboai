#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    // ...
  }
}
