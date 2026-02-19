import std;
import liboai;

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        // call async method; returns a future
        auto fut = oai.Edit->CreateAsync(
            "text-davinci-edit-001",
            "What day of the wek is it?",
            "Fix the spelling mistakes"
        );

        // do other work...

        // check if the future is ready
        fut.wait();

        // get the contained response
        auto response = fut.get();

        // print some response data
        if (response) {
            std::cout << response.value()["choices"][0]["text"].get<std::string>() << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
