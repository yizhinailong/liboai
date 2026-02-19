#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto res = oai.Audio->Transcribe("C:/some/folder/audio.mp3", "whisper-1");
        if (res) {
            std::cout << res.value()["text"].get<std::string>() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
