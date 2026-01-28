#include <iostream>
#include <cstdlib>
#include "../../tradermade.cpp"

int main() {
    const char* apiKey = std::getenv("TRADERMADE_API_KEY");
    
    if(!apiKey)
    {
        std::cerr<<"Please set your apiKey using environment variable."<<std::endl;
        return 1;
    }

    TraderMade tm;
    tm.setRestApiKey(apiKey);
    std::cout<<"---Historical Rates---"<<std::endl;
    std::cout<<tm.getHistoricalRates("2021-04-22", "EURUSD,UK100")<<std::endl;
    return 0;
}