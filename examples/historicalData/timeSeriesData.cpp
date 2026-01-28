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

    std::cout<<"---Here is your Timeseries Data---"<<std::endl;
    std::cout<<tm.getTimeSeriesData("EURUSD","2026-01-08","2026-01-09","daily","1","index")<<std::endl;

    return 0;
}