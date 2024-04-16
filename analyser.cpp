#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <limits>

// Struct to hold stock data
struct StockData {
        std::string stock_name;
        int interval;
        int volume_traded;
        float high;
        float low;
    };

class StockAnalyser {
  private:

    // Function to load data from file
    void LoadDataFromFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            StockData data;
            std::istringstream iss(line);
            // Extract data from the line and parse it to struct
            iss >> data.stock_name >> data.interval >> data.volume_traded >> data.high >> data.low;
            input_stock_data.push_back(data);
        }
        file.close();
    }

    std::vector<StockData> input_stock_data;

  public:
    // Constructor to read the input file and populate input_stock_data
    StockAnalyser(const std::string& filename) {
        LoadDataFromFile(filename);
    }
    
    // Constructor to accept pre-loaded or example data for unit tests
    StockAnalyser(const std::vector<StockData>& data) : input_stock_data(data) {}

    // Function to see the input stock data
    std::vector<StockData> GetStockData() {
        return input_stock_data;
    }

    // Function to calculate total volume traded per stock
    std::unordered_map<std::string, float> GetVolumeTradedPerStock() {
        std::unordered_map<std::string, float> total_volumes;
        for (auto& data : input_stock_data) {
            total_volumes[data.stock_name] += data.volume_traded;
        }
        return total_volumes;
    }

    // Function to calculate the percentage of volume traded relative to the total volume
    float CalculateVolumePercentage(const StockData& data, float total_volume) const {
        return (data.volume_traded / total_volume) * 100;
    }

    // Function to calculate maximum high and minimum low for each stock
    std::unordered_map<std::string, std::pair<float, float>> GetMaxBound() {
        std::unordered_map<std::string, std::pair<float, float>> bound_map;
        for (auto& data : input_stock_data) {
            // Check if stock exists in the map
            if (bound_map.count(data.stock_name) == 0) {
                // If not, initialize max and min bounds
                bound_map[data.stock_name] = {std::numeric_limits<float>::min(), std::numeric_limits<float>::max()};
            }

            // Update max high and min low
            float& max_high = bound_map[data.stock_name].first;
            float& min_low = bound_map[data.stock_name].second;
            max_high = std::max(max_high, data.high);
            min_low = std::min(min_low, data.low);
        }
        return bound_map;
    }

    // Function to print out the required output to the console
    void PrintOutput() {
        // Write the total volume traded per Stock&Interval as a percentage of the total volume traded per Stock
        std::unordered_map<std::string, float> total_volumes = GetVolumeTradedPerStock();
        for (auto& data : input_stock_data) {
            std::cout << data.stock_name << "," << data.interval << "," <<
                CalculateVolumePercentage(data, total_volumes[data.stock_name]) << std::endl;
        }

        // Write the delimiter '#'
        std::cout << "#" << std::endl;

        // Write the maximum High and minimum Low for each Stock
        std::unordered_map<std::string, std::pair<float, float>> bound_map = GetMaxBound();
        for (const auto& entry : bound_map) {
            std::cout << entry.first << "," << entry.second.first << "," <<
                entry.second.second << std::endl;
        }
    }
};