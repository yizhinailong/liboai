#include "liboai/liboai.hpp"

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        // call async method; returns a future
        auto fut = oai.FineTune->CreateAsync("file-XGinujblHPwGLSztz8cPS8XY");

        // do other work...

        // check if the future is ready
        fut.wait();

        // get the contained response
        auto response = fut.get();

        // print some response data
        if (response) {
            std::cout << response.value()["events"][0]["message"] << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
