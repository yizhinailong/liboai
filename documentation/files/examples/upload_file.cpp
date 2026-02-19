import liboai;

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.File->Create("C:/some/folder/file.jsonl", "fine-tune");
        if (response) {
            std::cout << response.value() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
