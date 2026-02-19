#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.FineTune->cancel("ft-AF1WoRqd3aJAHsqc9NY7iL8F");
        if (response) {
            std::cout << response.value()["status"].get<std::string>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
