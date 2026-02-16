#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY") && oai.auth.SetOrganizationFile("C:/some/folder/org.dat")) {
    // ...
  }
}
