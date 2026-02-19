import liboai;

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response =
            oai.Completion->Create("text-davinci-003", "Say this is a test", std::nullopt, 7);
        if (response) {
            std::cout << response.value()["choices"][0]["text"].get<std::string>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
