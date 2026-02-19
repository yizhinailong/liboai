#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.Moderation->create("I want to kill them.");
        if (response) {
            std::cout << response.value()["results"][0]["flagged"].get<bool>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
