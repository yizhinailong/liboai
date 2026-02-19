#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto result =
            oai.File->Download("file-XjGxS3KTG0uNmNOK362iJua3", "C:/some/folder/file.jsonl");
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
