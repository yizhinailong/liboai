#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        try {
            Response response = oai.Image->create("a siamese cat!");
            Network::Download(
                "C:/some/folder/file.png",                     // to
                response["data"][0]["url"].get<std::string>(), // from
                oai.auth.GetAuthorizationHeaders()
            );
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
