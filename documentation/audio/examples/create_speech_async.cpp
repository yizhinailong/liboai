#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto fut = oai.Audio->SpeechAsync(
            "tts-1",
            "alloy",
            "Today is a wonderful day to build something people love!"
        );
        // do other work...

        // check if the future is ready
        fut.wait();

        // get the contained response
        auto res = fut.get();
        if (res) {
            std::ofstream ocout("demo.mp3", std::ios::binary);
            ocout << res.value().content;
            ocout.close();
            std::cout << res.value().content.size() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
