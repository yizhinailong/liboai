import liboai;

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto res = oai.Azure->RequestImageGeneration(
            "resource",
            "api_version",
            "A snake in the grass!",
            1,
            "512x512"
        );

        // output the response
        if (res) {
            std::cout << res.value()["data"][0]["url"].get<std::string>() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
