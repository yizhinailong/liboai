#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.Edit->Create(
            "text-davinci-edit-001",
            "What day of the wek is it?",
            "Fix the spelling mistakes"
        );
        if (response) {
            std::cout << response.value()["choices"][0]["text"].get<std::string>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
