import std;
import liboai;

using namespace liboai;

int main() {
    OpenAI oai;

    // create a conversation
    Conversation convo;

    // add a message to the conversation
    convo.AddUserData("What is the point of taxes?");

    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto fut = oai.ChatCompletion->CreateAsync("gpt-3.5-turbo", convo);

        // do other work...

        // check if the future is ready
        fut.wait();

        // get the contained response
        auto response = fut.get();

        if (response) {
            // update our conversation with the response
            convo.Update(response.value());

            // print the response
            auto lastResponse = convo.GetLastResponse();
            if (lastResponse) {
                std::cout << lastResponse.value() << std::endl;
            }
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
