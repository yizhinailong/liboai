#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.Image->create_variation("C:/some/folder/otter.png");
        if (response) {
            std::cout << response.value()["data"][0]["url"].get<std::string>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
