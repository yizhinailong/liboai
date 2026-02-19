import liboai;

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.Model->List();
        if (response) {
            std::cout << response.value()["data"] << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
