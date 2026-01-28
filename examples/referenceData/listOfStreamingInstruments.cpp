#include <iostream>
#include <cstdlib>
#include "../../tradermade.cpp"

int main() {
    const char* apiKey = std::getenv("TRADERMADE_API_KEY");
    if(!apiKey)
    {
        std::cerr<<"Please set your TraderMade api key using environment variable."<<std::endl;
        return 1;
    }

    TraderMade tm;
    tm.setRestApiKey(apiKey);

    std::cout<<"---List of Streaming Instrument---"<<std::endl;   
    std::cout<<tm.getStreamingCurrencyList()<<std::endl;

    return 0;
}