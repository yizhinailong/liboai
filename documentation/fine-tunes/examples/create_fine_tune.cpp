#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.FineTune->Create("file-XGinujblHPwGLSztz8cPS8XY");
        if (response) {
            std::cout << response.value()["events"][0]["message"] << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
