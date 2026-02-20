import std;
import liboai;

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.Image->CreateEdit(
            "C:/some/folder/otter.png",
            "A cute baby sea otter wearing a beret",
            "C:/some/folder/mask.png"
        );
        if (response) {
            std::cout << response.value()["data"][0]["url"].get<std::string>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
