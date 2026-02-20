import std;
import liboai;

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto res = oai.Audio->Translate("C:/some/folder/file.mp3", "whisper-1");
        if (res) {
            std::cout << res.value()["text"] << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
