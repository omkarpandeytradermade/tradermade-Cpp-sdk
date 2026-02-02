#include "TraderMadeSDK.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <cstdio>
#include <array>
#include <cctype>
#include <nlohmann/json.hpp> 


// --- Internal Implementation Details ---

namespace Constants {
    const std::string DEFAULT_BASE_URL = "https://marketdata.tradermade.com/api/v1";

    const std::vector<std::string> TIME_SERIES_FORMAT = {
        "records", "csv", "index", "columns", "split"
    };

    const std::vector<std::string> TIME_SERIES_INTERVAL = {
        "daily", "hourly", "minute"
    };

    std::map<std::string, std::vector<int>> createPeriodMap() {
        std::map<std::string, std::vector<int>> m;
        m["daily"]  = {1};
        m["hourly"] = {1, 2, 4, 6, 8, 24};
        m["minute"] = {1, 5, 10, 15, 30};
        return m;
    }

    const std::map<std::string, std::vector<int>> TIME_SERIES_PERIOD = createPeriodMap();

    const std::vector<std::string> DATA_EXPORTS_PANDAS_DF_FIELDS = {
        "close", "ohlc"
    };

    const std::vector<std::string> DATA_EXPORTS_PANDAS_DF_FORMAT = {
        "records", "columns", "index", "split"
    };
}

// trim whitespace (for API key check)
inline std::string trim(const std::string &s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

// URL encoder (similar to encodeURIComponent)
std::string urlEncode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (unsigned char c : value) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int(c);
            escaped << std::nouppercase;
        }
    }
    return escaped.str();
}

// CLIENT CLASS IMPLEMENTATION
class Client {
private:
    std::string apiKey;

    std::string exec(const char* cmd) {
        std::array<char, 256> buffer{};
        std::string result;

    #ifdef _WIN32
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    #else
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    #endif

        if (!pipe) {
            throw std::runtime_error("Failed to run curl command.");
        }
        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

public:
    explicit Client(const std::string& key) : apiKey(key) {}

    std::string get(const std::string& endpoint,
                    const std::map<std::string, std::string>& params = {}) {
        std::string fullUrl = Constants::DEFAULT_BASE_URL + endpoint + "?api_key=" + urlEncode(apiKey);

        for (const auto& kv : params) {
            fullUrl += "&" + urlEncode(kv.first) + "=" + urlEncode(kv.second);
        }

        // Use -s for silent. Remove -k to enforce SSL validation.
        std::string command = "curl -s \"" + fullUrl + "\"";
        return exec(command.c_str());
    }
};

// TRADERMADE CLASS IMPLEMENTATION

TraderMade::TraderMade() = default;
TraderMade::~TraderMade() = default; // Defined here where Client is fully known

void TraderMade::validateApiKey(const std::string& key) {
    std::string t = trim(key);
    if (t.empty()) {
        throw std::invalid_argument("Api key must be a non empty string.");
    }
}

void TraderMade::ensureClient() const {
    if (!client) {
        throw std::runtime_error("API key not set. Call setRestApiKey() first.");
    }
}

void TraderMade::setRestApiKey(const std::string& key) {
    validateApiKey(key);
    this->apiKey = trim(key);
    this->client = std::make_unique<Client>(this->apiKey);
}

std::string TraderMade::getRestApiKey() const {
    return this->apiKey;
}

// --- CHANGED FUNCTIONS START HERE ---
// All return types changed to nlohmann::json
// All return statements wrapped in nlohmann::json::parse()

nlohmann::json TraderMade::getLiveRates(const std::string& currency) {
    ensureClient();
    if (currency.empty()) {
        throw std::invalid_argument("currency is required.");
    }
    return nlohmann::json::parse(client->get("/live", {{"currency", currency}}));
}

nlohmann::json TraderMade::getLiveCurrencyList() {
    ensureClient();
    return nlohmann::json::parse(client->get("/live_currencies_list"));
}

nlohmann::json TraderMade::getStreamingCurrencyList() {
    ensureClient();
    return nlohmann::json::parse(client->get("/streaming_currencies_list"));
}

nlohmann::json TraderMade::getCryptoList() {
    ensureClient();
    return nlohmann::json::parse(client->get("/live_crypto_list"));
}

nlohmann::json TraderMade::getHistoricalCurrencyList() {
    ensureClient();
    return nlohmann::json::parse(client->get("/historical_currencies_list"));
}

nlohmann::json TraderMade::getCfdList() {
    ensureClient();
    return nlohmann::json::parse(client->get("/cfd_list"));
}

nlohmann::json TraderMade::getHistoricalRates(const std::string& date, const std::string& symbol) {
    ensureClient();
    if (date.empty() || symbol.empty()) {
        throw std::invalid_argument("date and symbol are required.");
    }
    return nlohmann::json::parse(client->get("/historical", {{"currency", symbol}, {"date", date}}));
}

nlohmann::json TraderMade::getHourlyHistoricalData(const std::string& date_time, const std::string& symbol) {
    ensureClient();
    if (date_time.empty() || symbol.empty()) {
        throw std::invalid_argument("date_time and symbol are required.");
    }
    return nlohmann::json::parse(client->get("/hour_historical", {
        {"date_time", date_time},
        {"currency",  symbol}
    }));
}

nlohmann::json TraderMade::getMinuteHistoricalData(const std::string& date_time, const std::string& symbol) {
    ensureClient();
    if (date_time.empty() || symbol.empty()) {
        throw std::invalid_argument("date_time and symbol are required.");
    }
    return nlohmann::json::parse(client->get("/minute_historical", {
        {"date_time", date_time},
        {"currency",  symbol}
    }));
}

nlohmann::json TraderMade::getTickHistoricalData(const std::string& symbol,
                                                 const std::string& startDate,
                                                 const std::string& endDate,
                                                 const std::string& format) {
    ensureClient();
    if (symbol.empty() || startDate.empty() || endDate.empty()) {
        throw std::invalid_argument("symbol, startDate and endDate are required.");
    }
    std::string start = urlEncode(startDate);
    std::string end   = urlEncode(endDate);
    std::string endpoint = "/tick_historical/" + symbol + "/" + start + "/" + end;
    std::map<std::string, std::string> params;
    if (!format.empty()) {
        params["format"] = format;
    }
    return nlohmann::json::parse(client->get(endpoint, params));
}

nlohmann::json TraderMade::getTickHistoricalDataSample(const std::string& symbol,
                                                       const std::string& startDate,
                                                       const std::string& endDate,
                                                       const std::string& format) {
    ensureClient();
    if (symbol.empty() || startDate.empty() || endDate.empty() || format.empty()) {
        throw std::invalid_argument("symbol, startDate, endDate and format are required.");
    }
    std::string start = urlEncode(startDate);
    std::string end   = urlEncode(endDate);
    std::string endpoint = "/tick_historical_sample/" + symbol + "/" + start + "/" + end;
    return nlohmann::json::parse(client->get(endpoint, {{"format", format}}));
}

nlohmann::json TraderMade::getTimeSeriesData(const std::string& currency,
                                             const std::string& startDate,
                                             const std::string& endDate,
                                             const std::string& interval,
                                             const std::string& period,
                                             const std::string& format) {
    ensureClient();

    // Validate format
    if (std::find(Constants::TIME_SERIES_FORMAT.begin(),
                  Constants::TIME_SERIES_FORMAT.end(),
                  format) == Constants::TIME_SERIES_FORMAT.end()) {
        throw std::invalid_argument("Invalid format. Use one of: records,csv,index,columns,split.");
    }

    // Validate interval
    if (std::find(Constants::TIME_SERIES_INTERVAL.begin(),
                  Constants::TIME_SERIES_INTERVAL.end(),
                  interval) == Constants::TIME_SERIES_INTERVAL.end()) {
        throw std::invalid_argument("Invalid interval. Use one of: daily,hourly,minute.");
    }

    // Validate period
    int periodNum = 0;
    try {
        periodNum = std::stoi(period);
    } catch (...) {
        throw std::invalid_argument("period must be numeric.");
    }

    auto it = Constants::TIME_SERIES_PERIOD.find(interval);
    if (it == Constants::TIME_SERIES_PERIOD.end()) {
        throw std::invalid_argument("No valid periods defined for interval: " + interval);
    }

    const auto& validPeriods = it->second;
    if (std::find(validPeriods.begin(), validPeriods.end(), periodNum) == validPeriods.end()) {
        std::ostringstream oss;
        oss << "Invalid period for interval " << interval << ". Valid values: ";
        for (size_t i = 0; i < validPeriods.size(); ++i) {
            oss << validPeriods[i];
            if (i + 1 < validPeriods.size()) oss << ", ";
        }
        throw std::invalid_argument(oss.str());
    }

    return nlohmann::json::parse(client->get("/timeseries", {
        {"currency",   currency},
        {"start_date", startDate},
        {"end_date",   endDate},
        {"interval",   interval},
        {"period",     std::to_string(periodNum)},
        {"format",     format}
    }));
}

nlohmann::json TraderMade::getOpenMarketStatus() {
    ensureClient();
    return nlohmann::json::parse(client->get("/market_open_status"));
}

nlohmann::json TraderMade::getMarketOpenTiming() {
    ensureClient();
    return nlohmann::json::parse(client->get("/market_opening_times"));
}

nlohmann::json TraderMade::getCurrencyConversion(const std::string& from,
                                                 const std::string& to,
                                                 double amount) {
    ensureClient();
    return nlohmann::json::parse(client->get("/convert", {
        {"from",   from},
        {"to",     to},
        {"amount", std::to_string(amount)}
    }));
}

nlohmann::json TraderMade::getDataAsPandasDataFrame(const std::string& symbol,
                                                    const std::string& startDate,
                                                    const std::string& endDate,
                                                    const std::string& format,
                                                    const std::string& fields) {
    ensureClient();

    if (std::find(Constants::DATA_EXPORTS_PANDAS_DF_FORMAT.begin(),
                  Constants::DATA_EXPORTS_PANDAS_DF_FORMAT.end(),
                  format) == Constants::DATA_EXPORTS_PANDAS_DF_FORMAT.end()) {
        throw std::invalid_argument("Invalid format for pandasDF.");
    }

    if (std::find(Constants::DATA_EXPORTS_PANDAS_DF_FIELDS.begin(),
                  Constants::DATA_EXPORTS_PANDAS_DF_FIELDS.end(),
                  fields) == Constants::DATA_EXPORTS_PANDAS_DF_FIELDS.end()) {
        throw std::invalid_argument("Invalid fields for pandasDF.");
    }

    return nlohmann::json::parse(client->get("/pandasDF", {
        {"currency",   symbol},
        {"start_date", startDate},
        {"end_date",   endDate},
        {"format",     format},
        {"fields",     fields}
    }));
}