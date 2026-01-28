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

    std::cout<<"---Data As Pandas DataFrame---"<<std::endl;
    std::cout<<tm.getDataAsPandasDataFrame("EURUSD","2025-04-01","2025-05-09","index","close")<<std::endl;

    return 0;
}