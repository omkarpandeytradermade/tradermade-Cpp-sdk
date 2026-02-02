---
title: "How to get market data with C++ SDK"
id: 325
owner: "tutorial"
type: tutorials
slug: "market-data-with-c++-sdk"
published: true
timestamp: 2026-01-19 18:30:00
meta_title: "Get Live & Historical Market Data with C++ SDK | TraderMade"
meta_description: "A complete guide to using the TraderMade C++ SDK to fetch live FX rates, stock cfds, and retrieve historical time-series market data."
titleImage: "/static/uploads/CPP-SDK.webp"
tags: "c++, sdk, market data, forex, api"
related: "api, c++, backend, trading"
---

---

# TraderMade C++ SDK Tutorial

Welcome to the TraderMade C++ SDK! This library allows you to easily interact with the TraderMade Forex and CFD Data API using C++.

**Note:** This SDK automatically parses responses into JSON objects for easy access.

## Prerequisites

Before you begin, ensure you have the following environments set up:

1. **C++ Compiler**: A compiler that supports C++14 or higher (e.g., G++, Clang, MSVC).
2. **TraderMade API Key**: You can get one by signing up at [TraderMade](https://tradermade.com/).
3. **JSON Library**: This SDK requires the [nlohmann/json](https://github.com/nlohmann/json) library.
4. **cURL**: **Important!** This SDK relies on the system's `curl` command.

**Verify cURL is installed:**
Open your terminal and run:

```bash
curl --version
```

* **Windows**: If missing, download from [curl.se](https://curl.se/windows/) or run `winget install curl`.
* **Linux/Mac**: Usually pre-installed. If missing, use `sudo apt install curl` or `brew install curl`.

---

## 2. Installation

You can integrate this SDK using CMake or by manually downloading the files.

### Option A: CMake (Recommended)

If you are using CMake, create a `CMakeLists.txt` in your project root. This configuration will automatically download the required JSON library and build the SDK.

```cmake
cmake_minimum_required(VERSION 3.14)
project(TraderMadeSDK LANGUAGES CXX)

# 1. Fetch the JSON library automatically
include(FetchContent)

FetchContent_Declare(
    json
    URL [https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz](https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
)
FetchContent_MakeAvailable(json)

# 2. Define the SDK Library
add_library(tradermade_sdk STATIC TraderMadeSDK.cpp TraderMadeSDK.h)

target_compile_features(tradermade_sdk PUBLIC cxx_std_14)

# 3. Link JSON to the SDK
target_link_libraries(tradermade_sdk PUBLIC nlohmann_json::nlohmann_json)

target_include_directories(tradermade_sdk PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 4. Build your App (main.cpp)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE tradermade_sdk)

```

### Option B: Manual Git Clone

If you prefer to compile manually without CMake:

1. **Clone the SDK:**
```bash
git clone [https://github.com/tradermade/cpp-sdk.git](https://github.com/tradermade/cpp-sdk.git)
```


2. **Download json.hpp:**
Create a folder named `nlohmann` and place the `json.hpp` file inside it.
**PowerShell (Windows):**
```powershell
New-Item -ItemType Directory -Force -Name nlohmann
Invoke-WebRequest -Uri "[https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp](https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp)" -OutFile "nlohmann/json.hpp"
```


**Bash (Linux/Mac):**
```bash
mkdir -p nlohmann
wget -O nlohmann/json.hpp [https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp](https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp)
```



---

## 3. Setting Your Environment Variable

**Security Note:** It is best practice to keep your API key out of your source code. Use an Environment Variable instead.

### Setting Key in PowerShell

**Method 1: Current Session Only (Temporary)**
Use this if you just want to run the code once. The key will be lost when you close the window.

```powershell
$env:TRADERMADE_API_KEY = "YOUR_ACTUAL_API_KEY_HERE"
```

**Method 2: Permanent (User Profile)**
Use this to set the key permanently so you don't have to type it every time.

1. Open PowerShell.
2. Run the following command to edit your user environment variables:

```powershell
[System.Environment]::SetEnvironmentVariable('TRADERMADE_API_KEY', 'YOUR_ACTUAL_API_KEY_HERE', 'User')
```

3. **Restart PowerShell** for the changes to take effect.

---

## Quick Start

Here is a minimal example to get you started. This example initializes the SDK, sets your API key, and fetches live rates.

```cpp
#include <iostream>
#include <nlohmann/json.hpp> 
#include "TraderMadeSDK.h" // Use the header file, not .cpp

using json = nlohmann::json;

int main() {
    try {
        // 1. Initialize the SDK
        TraderMade tm;

        // 2. Set your API Key
        // Replace "YOUR_API_KEY" with your actual key
        tm.setRestApiKey("YOUR_API_KEY");

        // 3. Get Live Rates
        // Returns a JSON object
        json liveData = tm.getLiveRates("EURUSD,GBPUSD");
        std::cout << "Live Data: " << liveData.dump(4) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

```

---

## Usage Guide

### 1. Reference Data

Retrieve lists of available currencies, cryptocurrencies, and CFDs to know what symbols you can query.

```cpp
// Get list of all available currencies
std::cout << tm.getLiveCurrencyList().dump(4) << std::endl;

// Get list of cryptocurrencies
std::cout << tm.getCryptoList().dump(4) << std::endl;

// Get list of available CFDs
std::cout << tm.getCfdList().dump(4) << std::endl;

```

### 2. Live Market Data

Fetch real-time data for specific currency pairs.

```cpp
// Fetch live data for a single pair or multiple pairs (comma separated)
json rates = tm.getLiveRates("EURUSD,USDJPY");
std::cout << rates.dump(4) << std::endl;

```

### 3. Historical Data

Get historical exchange rates for a specific date.

```cpp
// Format: Date (YYYY-MM-DD), Symbol
json history = tm.getHistoricalRates("2023-10-25", "EURUSD");
std::cout << history.dump(4) << std::endl;

```

### 4. Hourly and Minute Data

Retrieve granular historical data.

```cpp
// Hourly Data (Date + Hour)
// Format: "YYYY-MM-DD-HH:MM", Symbol
json hourly = tm.getHourlyHistoricalData("2023-10-25-10:00", "EURUSD");

// Minute Data (Date + Hour + Minute)
// Format: "YYYY-MM-DD-HH:MM", Symbol
json minute = tm.getMinuteHistoricalData("2023-10-25-10:30", "GBPUSD");

```

### 5. Time Series Data

Fetch a range of data points (OHLC) over a period.

* **Intervals**: `daily`, `hourly`, `minute`
* **Formats**: `records`, `csv`, `split`

```cpp
try {
    json timeSeries = tm.getTimeSeriesData(
        "EURUSD",       // Symbol
        "2023-10-01",   // Start Date
        "2023-10-05",   // End Date
        "daily",        // Interval
        "1",            // Period (e.g., 1 day)
        "records"       // Format
    );
    std::cout << timeSeries.dump(4) << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Time Series Error: " << e.what() << std::endl;
}

```

### 6. Tick Data

Get high-frequency tick data (Bid/Ask) for analysis.

```cpp
// Fetch tick data
// Note: We use "json" format here so the SDK can parse it safely
json tickData = tm.getTickHistoricalData(
    "EURUSD", 
    "2023-10-25-10:00", // Start Timestamp
    "2023-10-25-10:05", // End Timestamp
    "json"              // Format
);
std::cout << tickData.dump(4) << std::endl;

```

### 7. Currency Conversion

Convert an amount from one currency to another using real-time rates.

```cpp
// Convert 1000 EUR to USD
json conversion = tm.getCurrencyConversion("EUR", "USD", 1000.0);
std::cout << conversion.dump(4) << std::endl;

```

---

---

## 4. Usage Example

Save the following code as `main.cpp`. It demonstrates how to initialize the SDK using the environment variable you just set.

```cpp
#include <iostream>
#include <cstdlib> // Required for std::getenv
#include <nlohmann/json.hpp> 
#include "TraderMadeSDK.h" 

using json = nlohmann::json;

int main() {
    try {
        // 1. Retrieve API Key from Environment Variable
        const char* envKey = std::getenv("TRADERMADE_API_KEY");
        if (!envKey) {
            std::cerr << "Error: TRADERMADE_API_KEY environment variable not set." << std::endl;
            return 1;
        }

        // 2. Initialize SDK
        TraderMade tm;
        tm.setRestApiKey(std::string(envKey));

        // 3. Get Live Rates
        std::cout << "Fetching Live Rates for EURUSD..." << std::endl;
        json liveData = tm.getLiveRates("EURUSD");
        std::cout << "Response: " << liveData.dump(4) << std::endl;

        // 4. Get Historical Data
        std::cout << "\nFetching Historical Data (2023-10-25)..." << std::endl;
        json history = tm.getHistoricalRates("2023-10-25", "GBPUSD");
        std::cout << "Response: " << history.dump(4) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "SDK Error: " << e.what() << std::endl;
    }

    return 0;
}

```

---

## Compilation

You can compile the project using the method you chose in Step 2.

**Option A: Building with CMake**

```bash
mkdir build
cd build
cmake ..
cmake --build .
# Run the app
./app       # Linux/Mac
.\Debug\app # Windows (often in Debug folder)

```

**Option B: Manual Compilation (g++)**
Because you are not using CMake, you must manually tell the compiler to build both your `main.cpp` and the `TraderMadeSDK.cpp`.

```bash
g++ main.cpp TraderMadeSDK.cpp -o app -I . -std=c++14

# Run the app
./app       # Linux/Mac
app.exe     # Windows

```

