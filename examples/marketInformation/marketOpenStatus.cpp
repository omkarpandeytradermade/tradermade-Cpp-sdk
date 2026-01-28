#include <iostream>
#include <cstdlib>
#include "../../tradermade.cpp"

int main() {
    const char* apiKey = std::getenv("TRADERMADE_API_KEY");
    if(!apiKey)
    {
        std::cerr<<"Please set your TraderMade api key using environment variable."<<std::endl;
        return 1;
    };
    TraderMade tm;
    tm.setRestApiKey(apiKey);

    std::cout<<"---Here is requested market open status---"<<std::endl;
    std::cout<<tm.getOpenMarketStatus()<<std::endl;
    return 0;
}