#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto res = oai.Azure->create_embedding(
            "resource",
            "deploymentID",
            "api_version",
            "String to get embedding for"
        );

        if (res) {
            std::cout << res.value() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
