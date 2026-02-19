#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.Embedding->create(
            "text-embedding-ada-002",
            "The food was delicious and the waiter..."
        );
        if (response) {
            std::cout << response.value()["data"][0]["embedding"] << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
