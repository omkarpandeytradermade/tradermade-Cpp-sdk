#ifndef TRADERMADE_SDK_H
#define TRADERMADE_SDK_H

#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp> // <--- NEW: Required for JSON types

class Client;

class TraderMade {
public:
    TraderMade();
    ~TraderMade(); 
    // Set REST API key
    void setRestApiKey(const std::string& key);

    // Get REST API key
    std::string getRestApiKey() const;

    // --- RETURN TYPES CHANGED TO nlohmann::json BELOW ---

    // 1. Live Rates
    nlohmann::json getLiveRates(const std::string& currency);

    // 2. Reference Data
    nlohmann::json getLiveCurrencyList();
    nlohmann::json getStreamingCurrencyList();
    nlohmann::json getCryptoList();
    nlohmann::json getHistoricalCurrencyList();
    nlohmann::json getCfdList();

    // 3. Get Historical Exchange Rates
    nlohmann::json getHistoricalRates(const std::string& date, const std::string& symbol);
    nlohmann::json getHourlyHistoricalData(const std::string& date_time, const std::string& symbol);
    nlohmann::json getMinuteHistoricalData(const std::string& date_time, const std::string& symbol);

    // 4. Tick Historical Data
    nlohmann::json getTickHistoricalData(const std::string& symbol,
                                         const std::string& startDate,
                                         const std::string& endDate,
                                         const std::string& format = "");

    nlohmann::json getTickHistoricalDataSample(const std::string& symbol,
                                               const std::string& startDate,
                                               const std::string& endDate,
                                               const std::string& format);

    // 5. Time Series Data
    nlohmann::json getTimeSeriesData(const std::string& currency,
                                     const std::string& startDate,
                                     const std::string& endDate,
                                     const std::string& interval,
                                     const std::string& period,
                                     const std::string& format);

    // 6. Market Status
    nlohmann::json getOpenMarketStatus();
    nlohmann::json getMarketOpenTiming();

    // 7. Currency Conversion
    nlohmann::json getCurrencyConversion(const std::string& from,
                                         const std::string& to,
                                         double amount);

    // 8. Pandas DataFrame Export
    nlohmann::json getDataAsPandasDataFrame(const std::string& symbol,
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