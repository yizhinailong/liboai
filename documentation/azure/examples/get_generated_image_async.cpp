import liboai;

using namespace liboai;

int main() {
    OpenAI oai;

    if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
        auto fut = oai.Azure->GetGeneratedImageAsync(
            "resource",
            "api_version",
            "f508bcf2-e651-4b4b-85a7-58ad77981ffa"
        );

        // do other work

        // wait for the future to complete
        fut.wait();

        // get the result
        auto res = fut.get();

        // output the response
        if (res) {
            std::cout << res.value() << std::endl;
        } else {
            std::cout << res.error().message << std::endl;
        }
    }
}
