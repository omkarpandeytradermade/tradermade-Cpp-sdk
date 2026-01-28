#include <iostream>
#include <cstdlib>
#include "../../tradermade.cpp"

int main()
{
    const char* apiKey = std::getenv("TRADERMADE_API_KEY");

    if(!apiKey) {
        std::cerr<<"Please set your TraderMade api key using environment variable."<<std::endl;
        return 1;
    }

    TraderMade tm;
    tm.setRestApiKey(apiKey);

    std::cout<<"---Minute historical data---"<<std::endl;
    std::cout<<tm.getMinuteHistoricalData("2026-01-09-14:59","EURUSD")<<std::endl;

    return 0;
}