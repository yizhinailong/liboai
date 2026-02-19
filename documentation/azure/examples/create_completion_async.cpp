import std;
import liboai;

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto fut = oai.Azure->CreateCompletionAsync(
            "resource",
            "deploymentID",
            "api_version",
            "Write a short poem about a snowman."
        );

        // do other stuff

        // wait for the future to be ready
        fut.wait();

        // get the result
        auto res = fut.get();

        if (res) {
            std::cout << res.value()["choices"][0]["text"].get<std::string>() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
