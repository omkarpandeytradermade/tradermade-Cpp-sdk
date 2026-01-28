#include <iostream>
#include <cstdlib>
#include "../../tradermade.cpp"

int main() {

    const char* apiKey = std::getenv("TRADERMADE_API_KEY");

    if(!apiKey)
    {
        std::cerr<<"Set your api key using environment variable."<<std::endl;
        return 1;
    };

    TraderMade tm;
    tm.setRestApiKey(apiKey);

    std::cout<<"---Hourly historical data---";
    std::cout<<tm.getHourlyHistoricalData("2025-12-12-14:00","GBPUSD")<<std::endl;

    return 0;
}