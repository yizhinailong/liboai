#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        // call async method; returns a future
        auto fut =
            oai.File->DownloadAsync("file-XjGxS3KTG0uNmNOK362iJua3", "C:/some/folder/file.jsonl");

        // do other work...

        // check if the future is ready
        fut.wait();

        // check if downloaded successfully
        auto result = fut.get();
        if (result) {
            if (result.value()) {
                std::cout << "File downloaded successfully!" << std::endl;
            } else {
                std::cout << "File download failed!" << std::endl;
            }
        } else {
            std::cout << result.error().message << std::endl;
        }
    }
}
