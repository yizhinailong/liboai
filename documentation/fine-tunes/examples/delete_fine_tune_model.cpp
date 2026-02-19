#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.FineTune->Remove("curie:ft-acmeco-2021-03-03-21-44-20");
        if (response) {
            std::cout << response.value()["deleted"].get<bool>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
