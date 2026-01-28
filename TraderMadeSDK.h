#ifndef TRADERMADE_SDK_H
#define TRADERMADE_SDK_H

#include <string>
#include <memory>
#include <vector>


class Client;

class TraderMade {
public:
    TraderMade();
    ~TraderMade(); 
    // Set REST API key
    void setRestApiKey(const std::string& key);

    // Get REST API key
    std::string getRestApiKey() const;

    // 1. Live Rates
    std::string getLiveRates(const std::string& currency);

    // 2. Reference Data
    std::string getLiveCurrencyList();
    std::string getStreamingCurrencyList();
    std::string getCryptoList();
    std::string getHistoricalCurrencyList();
    std::string getCfdList();

    // 3. Get Historical Exchange Rates
    std::string getHistoricalRates(const std::string& date, const std::string& symbol);
    std::string getHourlyHistoricalData(const std::string& date_time, const std::string& symbol);
    std::string getMinuteHistoricalData(const std::string& date_time, const std::string& symbol);

    // 4. Tick Historical Data
    // Note: Default argument moved here from implementation
    std::string getTickHistoricalData(const std::string& symbol,
                                      const std::string& startDate,
                                      const std::string& endDate,
                                      const std::string& format = "");

    std::string getTickHistoricalDataSample(const std::string& symbol,
                                            const std::string& startDate,
                                            const std::string& endDate,
                                            const std::string& format);

    // 5. Time Series Data
    std::string getTimeSeriesData(const std::string& currency,
                                  const std::string& startDate,
                                  const std::string& endDate,
                                  const std::string& interval,
                                  const std::string& period,
                                  const std::string& format);

    // 6. Market Status
    std::string getOpenMarketStatus();
    std::string getMarketOpenTiming();

    // 7. Currency Conversion
    std::string getCurrencyConversion(const std::string& from,
                                      const std::string& to,
                                      double amount);

    // 8. Pandas DataFrame Export
    std::string getDataAsPandasDataFrame(const std::string& symbol,
                                         const std::string& startDate,
                                         const std::string& endDate,
                                         const std::string& format,
                                         const std::string& fields);

private:
    std::string apiKey;
    std::unique_ptr<Client> client;

    void validateApiKey(const std::string& key);
    void ensureClient() const;
};

#endif 