import std;
import liboai;

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyFile("C:/some/folder/key.dat")) {
    // ...
  }
}
