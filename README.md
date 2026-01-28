
---

# TraderMade C++ SDK

Official C++ client library for the TraderMade Data API.

## 🚀 Installation


Unlike Node.js, C++ does not have a central package manager like npm for every project. To use this SDK:

1. **Create a `src` folder** for your project.

### Option 1: CMake (Recommended)

If you are using CMake, you can fetch the SDK automatically. Add this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    tradermade_sdk
    GIT_REPOSITORY https://github.com/YOUR_GITHUB_USERNAME/cpp-sdk.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(tradermade_sdk)

# Link the SDK to your target
target_link_libraries(your_app_name PRIVATE tradermade_sdk)

```

### Option 2: Manual Download

1. **Create a `src` folder** for your project.
2. **Download/Copy** the `TraderMadeSDK.h` and `TraderMadeSDK.cpp` files into it.
3. **Install cURL** (Required for API requests).

**Linux (Debian/Ubuntu):**

```bash
sudo apt-get install curl

```

**Mac:**

```bash
brew install curl

```

**Windows:**
Ensure `curl` is installed and available in your system PATH (Standard on Windows 10/11).

## 🛠️ Project Setup

Before running the code, set up your project files inside the `src` folder.

**1. Set your Environment Variable**
Instead of a `.env` file (which requires extra libraries in C++), set your API key in your terminal before running the program:

* **Linux/Mac:** `export TRADERMADE_API_KEY=your_api_key_here`
* **Windows (PowerShell):** `$env:TRADERMADE_API_KEY="your_api_key_here"`

**2. Create your script**
Create a file named `main.cpp` inside `src` and paste the code examples below.

**3. Compile and Run**
Run the compiler from your terminal. **Note:** You must compile both the main file and the SDK file.

```bash
g++ main.cpp TraderMadeSDK.cpp -o trader_app
./trader_app

```

## 🔑 Getting Started

First, include the library and initialize the client.

```cpp
#include <iostream>
#include <cstdlib> // For std::getenv
#include "TraderMadeSDK.h" // UPDATED: Include the header file

int main() {
    TraderMade tm;
    
    // securely get key from environment variable
    const char* apiKey = std::getenv("TRADERMADE_API_KEY");
    if (apiKey) {
        tm.setRestApiKey(apiKey);
    } else {
        std::cerr << "Please set TRADERMADE_API_KEY environment variable" << std::endl;
        return 1;
    }

    // ... code examples go here ...

    return 0;
}

```

## 📚 Usage Examples

Looking for more? > For a comprehensive list of examples covering more endpoints and advanced usage, please refer to our **GitHub Examples Directory**.

After creating the client, making calls to the TraderMade API is easy. Below are examples for fetching different types of market data.

### 1. Get Live Rates

Fetch real-time data for multiple currencies or instruments at once.

```cpp
#include <iostream>
#include "TraderMadeSDK.h"

void liveRates(TraderMade& tm) {
    try {
        // Pass multiple currencies as a comma-separated string
        std::string data = tm.getLiveRates("GBPUSD,UK100,USDJPY");
        std::cout << data << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

```

### 2. Convert Currency

Convert a specific amount from one currency to another using live exchange rates.

```cpp
#include <iostream>
#include "TraderMadeSDK.h"

void currencyConversion(TraderMade& tm) {
    try {
        std::string data = tm.getCurrencyConversion(
            "EUR",   // From Currency
            "GBP",   // To Currency
            1000.0   // Amount (double)
        );
        std::cout << "Conversion Result: " << data << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

```

### 3. Get Historical Tick Data

Fetch raw tick data for a specific currency pair and time range.

```cpp
#include <iostream>
#include "TraderMadeSDK.h"

void tickHistoricalData(TraderMade& tm) {
    try {
        std::string data = tm.getTickHistoricalData(
            "GBPUSD",
            "2026-01-12 15:00",
            "2026-01-12 15:30"
        );
        std::cout << data << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

```

### 4. Get Time Series Data

Fetch daily time series data for analysis.

```cpp
#include <iostream>
#include "TraderMadeSDK.h"

void timeSeriesData(TraderMade& tm) {
    try {
        std::string data = tm.getTimeSeriesData(
            "EURUSD",
            "2026-01-08",
            "2026-01-10",
            "daily",
            "1",
            "records"
        );
        std::cout << data << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

```

---

### Example `main.cpp` combining everything:

```cpp
#include <iostream>
#include <cstdlib>
#include "TraderMadeSDK.h"

int main() {
    TraderMade tm;
    const char* envKey = std::getenv("TRADERMADE_API_KEY");
    
    if (!envKey) {
        std::cout << "API Key not found!" << std::endl;
        return 1;
    }
    
    tm.setRestApiKey(envKey);

    std::cout << "--- Live Rates ---" << std::endl;
    std::cout << tm.getLiveRates("EURUSD") << std::endl;

    std::cout << "\n--- Conversion ---" << std::endl;
    std::cout << tm.getCurrencyConversion("EUR", "USD", 100) << std::endl;

    return 0;
}

```