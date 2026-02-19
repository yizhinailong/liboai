#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto res = oai.Azure->CreateCompletion(
            "resource",
            "deploymentID",
            "api_version",
            "Write a short poem about a snowman."
        );

        if (res) {
            std::cout << res.value()["choices"][0]["text"].get<std::string>() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
