#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto res = oai.Azure->delete_generated_image(
            "resource",
            "api_version",
            "f508bcf2-e651-4b4b-85a7-58ad77981ffa"
        );

        // output the response
        if (res) {
            std::cout << res.value() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
