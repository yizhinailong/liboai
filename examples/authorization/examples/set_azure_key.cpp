import std;
import liboai;

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetAzureKey("hard-coded-key")) { // NOT recommended
    // ...
  }
}
