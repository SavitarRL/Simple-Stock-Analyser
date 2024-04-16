#include <gtest/gtest.h>
#include <string>
#include "analyser.cpp"

// Fixture for StockAnalyser tests
class StockAnalyserTest : public testing::Test {};

// Tests for LoadDataFromFile and hence the constructor
// We may assume the input files are of `*.txt` files in the correct format and data types

// Test loading data from exisistent file
TEST_F(StockAnalyserTest, LoadDataFromFile_PresentFile) {
    // Load data from the file
    ASSERT_NO_THROW(StockAnalyser analyser("../datafiles/input.txt")) << "Failed to load data from input.txt";
    ASSERT_NO_THROW(StockAnalyser analyser("../datafiles/testfile.txt")) << "Failed to load data from testfile.txt";
}

// Test case: Load data from a non-existent file
TEST_F(StockAnalyserTest, LoadDataFromFile_NonExistentFile) {
    ASSERT_THROW(StockAnalyser analyser("non_existent_file.txt"), std::runtime_error) << "Expected exception for non-existent file not thrown";
    ASSERT_THROW(StockAnalyser analyser("../datafiles/non_existent_file.txt"), std::runtime_error) << "Expected exception for non-existent file not thrown";
}

// Test case: Load data from an existent file but incorrect directory
TEST_F(StockAnalyserTest, LoadDataFromFile_IncorrectDirectory) {
    ASSERT_THROW(StockAnalyser analyser("input.txt"), std::runtime_error) << "Expected exception for incorrect directory not thrown";
    ASSERT_THROW(StockAnalyser analyser("testfile.txt"), std::runtime_error) << "Expected exception for incorrect directory not thrown";
}

// Tests for GetVolumeTradedPerStock
// Test case: Check if the total_volumes output is valid for test data 2
TEST_F(StockAnalyserTest, TotalVolumesOutput_Valid_TestData) {
    std::vector<StockData> test_data = {
        {"AAPL", 1, 850, 170.0f, 140.0f},
        {"AAPL", 1, 301, 180.0f, 150.0f},
        {"MSFT", 2, 2040, 270.0f, 250.0f},
        {"GOOG", 1, 399, 1300.0f, 1200.0f},
        {"MSFT", 1, 3000, 270.0f, 250.0f}
    };
    
    StockAnalyser testdata(test_data);
    std::unordered_map<std::string, float> total_volumes = testdata.GetVolumeTradedPerStock();

    // Check if the total volumes are calculated correctly for each stock
    ASSERT_EQ(total_volumes["AAPL"], 1151) << "Total volume for AAPL is incorrect";
    ASSERT_EQ(total_volumes["GOOG"], 399) << "Total volume for GOOG is incorrect";
    ASSERT_EQ(total_volumes["MSFT"], 5040) << "Total volume for MSFT is incorrect";
}


// Test case: Check if the total_volumes output is valid for single data point set
TEST_F(StockAnalyserTest, TotalVolumesOutput_Valid_SingleDataPointSet) {
    std::vector<StockData> single_data_point_set = {{"AAPL", 1, 100, 24.0f, 10.0f}};
    StockAnalyser stock_single(single_data_point_set);
    std::unordered_map<std::string, float> total_volumes = stock_single.GetVolumeTradedPerStock();

    ASSERT_EQ(total_volumes["AAPL"], 100) << "Total volume for single data point set is incorrect";
}

// Test case: Check if the total_volumes output is valid for data from the input.txt file
TEST_F(StockAnalyserTest, TotalVolumesOutput_Valid_InputFile) {
    StockAnalyser analyser("../datafiles/input.txt");
    std::unordered_map<std::string, float> total_volumes = analyser.GetVolumeTradedPerStock();

    ASSERT_EQ(total_volumes["VOD.L"], 100 + 25) << "Total volume for VOD.L from input.txt is incorrect"; 
    ASSERT_EQ(total_volumes["BT.LN"], 300 + 900) << "Total volume for BT.LN from input.txt is incorrect"; 
}

// Test case: Check if the total_volumes output is valid for data from the testfile.txt file
TEST_F(StockAnalyserTest, TotalVolumesOutput_Valid_TestFile) {
    StockAnalyser analyser("../datafiles/testfile.txt");
    std::unordered_map<std::string, float> total_volumes = analyser.GetVolumeTradedPerStock();

    ASSERT_EQ(total_volumes["AAPL"], 307 + 203) << "Total volume for AAPL from testfile.txt is incorrect"; 
    ASSERT_EQ(total_volumes["TSLA"], 1013) << "Total volume for TSLA from testfile.txt is incorrect"; 
    ASSERT_EQ(total_volumes["MSFT"], 2517 + 1817) << "Total volume for MSFT from testfile.txt is incorrect"; 
    ASSERT_EQ(total_volumes["GOOG"], 1509 + 1223) << "Total volume for GOOG from testfile.txt is incorrect"; 
    ASSERT_EQ(total_volumes["NVDA"], 812) << "Total volume for NVDA from testfile.txt is incorrect"; 
    ASSERT_EQ(total_volumes["VOD.L"], 105 + 29) << "Total volume for VOD.L from testfile.txt is incorrect"; 
}

// Tests for CalculateVolumePercentage
// Test case: Check if CalculateVolumePercentage returns correct percentage for single data point set
TEST_F(StockAnalyserTest, CalculateVolumePercentage_SingleDataPointSet) {
    std::vector<StockData> single_data_point_set = {{"AAPL", 1, 100, 24.0f, 10.0f}};
    StockAnalyser stock_single(single_data_point_set);
    std::unordered_map<std::string, float> total_volumes = stock_single.GetVolumeTradedPerStock();

    // Calculate volume percentage
    float volume_percentage = stock_single.CalculateVolumePercentage(single_data_point_set[0], total_volumes["AAPL"]);

    // Expected volume percentage for single data point should be 100%
    ASSERT_FLOAT_EQ(volume_percentage, 100.0f) << "Volume percentage for single data point set is incorrect.";
}

// Test case: Check if CalculateVolumePercentage returns correct percentage for data from the input.txt file
TEST_F(StockAnalyserTest, CalculateVolumePercentage_InputFile) {
    StockAnalyser analyser("../datafiles/input.txt");
    std::unordered_map<std::string, float> total_volumes = analyser.GetVolumeTradedPerStock();

    // Define expected volume percentages
    std::unordered_map<std::string, std::vector<float>> expected;
    expected["VOD.L"] = {80.0, 20.0};
    expected["BT.LN"] = {25.0, 75.0};

    // Calculate volume percentages for each stock in the input data
    std::unordered_map<std::string, std::vector<float>> actual;
    for(auto &data : analyser.GetStockData()) {
        float vol_percentage = analyser.CalculateVolumePercentage(data, total_volumes[data.stock_name]);
        actual[data.stock_name].push_back(vol_percentage);
    }

    // Compare actual and expected volume percentages
    // Not optimal due to quadratic scaling, but okay for small datasets
    for(auto &volumes : expected){
        for(int i = 0; i < expected[volumes.first].size(); i++ ) {
            EXPECT_NEAR(actual[volumes.first][i], expected[volumes.first][i], 0.001)
                << "Volume percentage for stock " << volumes.first << " is incorrect.";
        }
    }
}

// Test case: Check if CalculateVolumePercentage returns correct percentage for data from the testfile.txt file
TEST_F(StockAnalyserTest, CalculateVolumePercentage_TestFile) {
    StockAnalyser analyser("../datafiles/testfile.txt");
    std::unordered_map<std::string, float> total_volumes = analyser.GetVolumeTradedPerStock();

    // Define expected volume percentages
    std::unordered_map<std::string, std::vector<float>> expected;
    expected["AAPL"] = {307.0 / (307.0 + 203.0) * 100, 203.0 / (307.0 + 203.0) * 100};
    expected["TSLA"] = {100.0};
    expected["MSFT"] = {2517.0 / (2517.0 + 1817.0) * 100, 1817.0 / (2517.0 + 1817.0) * 100};
    expected["GOOG"] = {1509.0 / (1509.0 + 1223.0) * 100, 1223.0 / (1509.0 + 1223.0) * 100};
    expected["NVDA"] = {100.0};
    expected["VOD.L"] = {105.0 / (105.0 + 29.0) * 100, 29.0 / (105.0 + 29.0) * 100};

    // Calculate volume percentages for each stock in the test data
    std::unordered_map<std::string, std::vector<float>> actual;
    for(auto &data : analyser.GetStockData()) {
        float vol_percentage = analyser.CalculateVolumePercentage(data, total_volumes[data.stock_name]);
        actual[data.stock_name].push_back(vol_percentage);
    }

    // Compare actual and expected volume percentages
    // Not optimal due to quadratic scaling, but okay for small datasets
    for(auto &volumes : expected){
        for(int i = 0; i < expected[volumes.first].size(); i++ ) {
            EXPECT_NEAR(actual[volumes.first][i], expected[volumes.first][i], 0.001)
                << "Volume percentage for stock " << volumes.first << " is incorrect.";
        }
    }
}


// Tests for GetMaxBound() function

// Test case: verifies if GetMaxBound() returns the maximum high and low values for the stock are correctly determined
TEST_F(StockAnalyserTest, GetMaxBound_SameStock) {
    // Define sample data with same stock
    std::vector<StockData> same_stock_data = {
        {"GOOG", 2, 100, 200, 195},
        {"GOOG", 1, 20, 150.1, 175.2}
    };

    StockAnalyser same_stock(same_stock_data);

    // Get maximum bounds for the stock
    std::unordered_map<std::string, std::pair<float, float>> same_stock_bounds = same_stock.GetMaxBound();

    // Ensure only one stock's bounds are retrieved
    ASSERT_EQ(same_stock_bounds.size(), 1) << "Incorrect number of stocks";

    // Check the maximum bounds for the stock
    EXPECT_NEAR(same_stock_bounds["GOOG"].first, 200, 0.001 )
        << "Maximum high value for stock GOOG is incorrect.";
    EXPECT_NEAR(same_stock_bounds["GOOG"].second, 175.2, 0.001)
        << "Maximum low value for stock GOOG is incorrect.";
}

// Test case: verifies if GetMaxBound() returns maximum high and low values for the stock are correctly determined when the data has the same low and high values
TEST_F(StockAnalyserTest, GetMaxBound_SameStockLowHighData) {
    std::vector<StockData> same_stock_data = {
        {"GOOG", 2, 100, 200, 195},
        {"GOOG", 1, 25, 202.4, 190.9},
        {"GOOG", 2, 20, 202.4, 190.9}
    };

    StockAnalyser same_stock(same_stock_data);
    std::unordered_map<std::string, std::pair<float, float>> same_stock_bounds = same_stock.GetMaxBound();

    ASSERT_EQ(same_stock_bounds.size(), 1) << "Incorrect number of stocks";

    EXPECT_NEAR(same_stock_bounds["GOOG"].first, 202.4, 0.001 )
        << "Maximum high value for stock GOOG is incorrect.";
    EXPECT_NEAR(same_stock_bounds["GOOG"].second, 190.9, 0.001)
        << "Maximum low value for stock GOOG is incorrect.";
}

// Test case: verifies if GetMaxBound() returns the maximum high and low values are correctly determined for each stock when the data contains multiple stocks.
TEST_F(StockAnalyserTest, GetMaxBound_MixedStockNames) {
    std::vector<StockData> mixed_stock_data = {
        {"AAPL", 2, 100, 100.5, 97.6},
        {"GOOG",  2, 20, 90.7, 85.6},
        {"MSFT", 2, 251, 200.5, 200.8}
    };

    StockAnalyser mixed_stock(mixed_stock_data);

    std::unordered_map<std::string, std::pair<float, float>> mixed_stock_bounds = mixed_stock.GetMaxBound();

    ASSERT_EQ(mixed_stock_bounds.size(), 3) << "Incorrect number of stocks";

    EXPECT_NEAR(mixed_stock_bounds["AAPL"].first, 100.5, 0.001 )
        << "Maximum high value for stock AAPL is incorrect.";
    EXPECT_NEAR(mixed_stock_bounds["AAPL"].second, 97.6, 0.001)
        << "Maximum low value for stock AAPL is incorrect.";

    EXPECT_NEAR(mixed_stock_bounds["GOOG"].first, 90.7, 0.001 ) 
        << "Maximum high value for stock GOOG is incorrect.";
    EXPECT_NEAR(mixed_stock_bounds["GOOG"].second, 85.6, 0.001)
        << "Maximum low value for stock GOOG is incorrect.";
    
    EXPECT_NEAR(mixed_stock_bounds["MSFT"].first, 200.5, 0.001 )
        << "Maximum high value for stock MSFT is incorrect.";
    EXPECT_NEAR(mixed_stock_bounds["MSFT"].second, 200.8, 0.001)
        << "Maximum low value for stock MSFT is incorrect.";
}


// Test case: check if GetMaxBound returns correct maximum bounds for stock data including zero values.
TEST_F(StockAnalyserTest, GetMaxBound_LowHighWithZero) {

    std::vector<StockData> stock_data_with_zero = {
        {"GOOG", 2, 75, 0, 277.1},
        {"GOOG",  1, 58, 20.0, 88.4},
        {"GOOG", 3, 222, 200.5, 2.5},
        {"GOOG", 2, 44, 100, 0},
    };

    StockAnalyser stock_with_zero(stock_data_with_zero);

    std::unordered_map<std::string, std::pair<float, float>> with_zero_stock_bounds = stock_with_zero.GetMaxBound();

    ASSERT_EQ(with_zero_stock_bounds.size(), 1) << "Incorrect number of stocks";

    EXPECT_NEAR(with_zero_stock_bounds["GOOG"].first, 200.5, 0.001 ) << "Incorrect maximum high value for stock GOOG.";
    EXPECT_NEAR(with_zero_stock_bounds["GOOG"].second, 0.0, 0.001) << "Incorrect maximum low value for stock GOOG.";
}

// Test case: 
TEST_F(StockAnalyserTest, GetMaxBound_InputData) {

    StockAnalyser analyser("../datafiles/input.txt");

    std::unordered_map<std::string, std::pair<float, float>> stock_bounds = analyser.GetMaxBound();

    ASSERT_EQ(stock_bounds.size(), 2) << "Incorrect number of stocks with maximum bounds retrieved. Expected: 6, Actual: " << stock_bounds.size();;
    
    EXPECT_NEAR(stock_bounds["BT.LN"].first, 449.8, 0.001 ) << "Incorrect maximum high value for stock BT.LN.";
    EXPECT_NEAR(stock_bounds["BT.LN"].second, 448.2, 0.001) << "Incorrect maximum low value for stock BT.LN.";
    
    EXPECT_NEAR(stock_bounds["VOD.L"].first, 184.1, 0.001 ) << "Incorrect maximum high value for stock VOD.L.";
    EXPECT_NEAR(stock_bounds["VOD.L"].second, 182.4, 0.001) << "Incorrect maximum low value for stock VOD.L.";
}

// Test case: check if GetMaxBound returns correct maximum bounds for test data
TEST_F(StockAnalyserTest, GetMaxBound_TestData) {

    StockAnalyser analyser("../datafiles/testfile.txt");

    std::unordered_map<std::string, std::pair<float, float>> stock_bounds = analyser.GetMaxBound();

    ASSERT_EQ(stock_bounds.size(), 6) << "Incorrect number of stocks with maximum bounds retrieved. Expected: 6, Actual: " << stock_bounds.size();

    EXPECT_NEAR(stock_bounds["AAPL"].first, 170.6, 0.001 ) << "Maximum high value for stock AAPL is incorrect.";
    EXPECT_NEAR(stock_bounds["AAPL"].second, 160.7, 0.001) << "Maximum low value for stock AAPL is incorrect.";

    EXPECT_NEAR(stock_bounds["TSLA"].first, 750.1, 0.001 ) << "Maximum high value for stock TSLA is incorrect.";
    EXPECT_NEAR(stock_bounds["TSLA"].second, 721.9, 0.001) << "Maximum low value for stock TSLA is incorrect.";

    EXPECT_NEAR(stock_bounds["MSFT"].first, 260.7, 0.001 ) << "Maximum high value for stock MSFT is incorrect.";
    EXPECT_NEAR(stock_bounds["MSFT"].second, 231.9, 0.001) << "Maximum low value for stock MSFT is incorrect.";

    EXPECT_NEAR(stock_bounds["GOOG"].first, 1305.9, 0.001 ) << "Maximum high value for stock GOOG is incorrect.";
    EXPECT_NEAR(stock_bounds["GOOG"].second, 1102.4, 0.001) << "Maximum low value for stock GOOG is incorrect.";

    EXPECT_NEAR(stock_bounds["NVDA"].first, 200.0, 0.001 ) << "Maximum high value for stock NVDA is incorrect.";
    EXPECT_NEAR(stock_bounds["NVDA"].second, 195.7, 0.001) << "Maximum low value for stock NVDA is incorrect.";

    EXPECT_NEAR(stock_bounds["VOD.L"].first, 184.4, 0.001 ) << "Maximum high value for stock VOD.L is incorrect.";
    EXPECT_NEAR(stock_bounds["VOD.L"].second, 182.8, 0.001) << "Maximum low value for stock VOD.L is incorrect.";
}