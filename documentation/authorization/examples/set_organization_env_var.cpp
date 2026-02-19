#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY") && oai.auth.SetOrganizationEnv("OPENAI_ORG_ID")) {
        // ...
    }
}
