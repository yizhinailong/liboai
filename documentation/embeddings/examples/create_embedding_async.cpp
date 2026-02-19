import liboai;

using namespace liboai;

int main() {
    OpenAI oai;
    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        // call async method; returns a future
        auto fut = oai.Embedding->CreateAsync(
            "text-embedding-ada-002",
            "The food was delicious and the waiter..."
        );

        // do other work...

        // check if the future is ready
        fut.wait();

        // get the contained response
        auto response = fut.get();

        // print some response data
        if (response) {
            std::cout << response.value()["data"][0]["embedding"] << std::endl;
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
