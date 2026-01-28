#include <iostream>
#include <cstdlib>
#include "../../tradermade.cpp"

int main() {

    const char* apiKey = std::getenv("TRADERMADE_API_KEY");
    if(!apiKey)
    {
        std::cout<<"Please set your TraderMade api key using environment variable."<<std::endl;
        return 1;
    }

    TraderMade tm;
    tm.setRestApiKey(apiKey);

    std::cout<<"---Here is your sample Historical tick data---"<<std::endl;
    std::cout<<tm.getTickHistoricalDataSample("EURUSD","2026-01-08 15:00","2026-01-08 15:30","csv")<<std::endl;

    return 0;
}