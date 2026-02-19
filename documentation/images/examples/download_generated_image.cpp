#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.Image->create("a siamese cat!");
        if (response) {
            Network::Download(
                "C:/some/folder/file.png",                             // to
                response.value()["data"][0]["url"].get<std::string>(), // from
                oai.auth.GetAuthorizationHeaders()
            );
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
