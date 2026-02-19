import std;
import liboai;

using namespace liboai;

int main() {
    OpenAI oai;

    // create a conversation
    Conversation convo;

    // add a message to the conversation
    convo.AddUserData("Hello, how are you? What time is it for you?");

    if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
        auto response = oai.ChatCompletion->Create("gpt-3.5-turbo", convo);

        if (response) {
            // update the conversation with the response
            convo.Update(response.value());

            // print the conversation
            auto lastResponse = convo.GetLastResponse();
            if (lastResponse) {
                std::cout << lastResponse.value() << std::endl;
            }

            // pop (remove) the last response from the conversation
            convo.PopLastResponse();
        } else {
            std::cout << response.error().message << std::endl;
        }
    }
}
