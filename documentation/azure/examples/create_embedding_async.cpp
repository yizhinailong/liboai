#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto fut = oai.Azure->create_embedding_async(
            "resource",
            "deploymentID",
            "api_version",
            "String to get embedding for"
        );

        // do other work

        // wait for the future to complete
        auto res = fut.get();

        // output the response
        if (res) {
            std::cout << res.value() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
