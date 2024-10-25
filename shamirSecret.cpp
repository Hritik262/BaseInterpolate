#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <json/json.h>

using namespace std;

// Base conversion function with error handling
long long decodeBase(const string& value, int base) {
    if (base < 2 || base > 16) {
        throw runtime_error("Base must be between 2 and 16");
    }

    long long decodedValue = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) {
            digit = c - '0';
        } else {
            digit = tolower(c) - 'a' + 10;
        }
        
        if (digit < 0 || digit >= base) {
            throw runtime_error("Invalid digit '" + string(1, c) + "' for base " + to_string(base));
        }
        
        // Check for overflow
        if (decodedValue > (LLONG_MAX - digit) / base) {
            throw runtime_error("Numeric overflow in base conversion");
        }
        
        decodedValue = decodedValue * base + digit;
    }
    return decodedValue;
}

//Lagrange interpolation with high precision
long long lagrangeInterpolation(const vector<pair<int, long long>>& points, int k) {
    long double result = 0.0;
    
    for (int i = 0; i < k; ++i) {
        if (i >= points.size()) {
            throw runtime_error("Not enough points for interpolation");
        }

        long double term = points[i].second;
        long double numerator = 1.0;
        long double denominator = 1.0;
        
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                numerator *= (-points[j].first);  // Evaluate at x = 0
                denominator *= (points[i].first - points[j].first);
                
                if (denominator == 0) {
                    throw runtime_error("Division by zero in Lagrange interpolation");
                }
            }
        }
        
        term *= (numerator / denominator);
        result += term;
    }
    
    // Round to nearest integer and check for overflow
    if (result > LLONG_MAX || result < LLONG_MIN) {
        throw runtime_error("Result overflow in Lagrange interpolation");
    }
    
    return llround(result);
}

// Process a single test case with comprehensive error checking
long long processTestCase(const Json::Value& root) {
    if (!root.isMember("keys")) {
        throw runtime_error("Missing 'keys' object in JSON");
    }
    
    int n = root["keys"]["n"].asInt();
    int k = root["keys"]["k"].asInt();
    
    if (k <= 0 || n < k) {
        throw runtime_error("Invalid n or k values");
    }
    
    vector<pair<int, long long>> points;
    points.reserve(n);  // Pre-allocate memory
    
    // Extract and validate points
    for (const auto& key : root.getMemberNames()) {
        if (key == "keys") continue;
        
        try {
            int x = stoi(key);
            if (!root[key].isMember("base") || !root[key].isMember("value")) {
                throw runtime_error("Missing base or value for point " + key);
            }
            
            int base = stoi(root[key]["base"].asString());
            string value = root[key]["value"].asString();
            
            if (value.empty()) {
                throw runtime_error("Empty value string for point " + key);
            }
            
            long long y = decodeBase(value, base);
            points.push_back({x, y});
        } catch (const exception& e) {
            throw runtime_error("Error processing point " + key + ": " + e.what());
        }
    }
    
    if (points.size() < k) {
        throw runtime_error("Not enough valid points. Required: " + to_string(k) + 
                          ", Found: " + to_string(points.size()));
    }
    
    // Sort points for consistency
    sort(points.begin(), points.end());
    
    return lagrangeInterpolation(points, k);
}

int main() {
    try {
        // First test case
        string json_input1 = R"({
            "keys": {"n": 4, "k": 3},
            "1": {"base": "10", "value": "4"},
            "2": {"base": "2", "value": "111"},
            "3": {"base": "10", "value": "12"},
            "6": {"base": "4", "value": "213"}
        })";

        // Second test case
        string json_input2 = R"({
            "keys": {"n": 10, "k": 7},
            "1": {"base": "6", "value": "13444211440455345511"},
            "2": {"base": "15", "value": "aed7015a346d63"},
            "3": {"base": "15", "value": "6aeeb69631c227c"},
            "4": {"base": "16", "value": "e1b5e05623d881f"},
            "5": {"base": "8", "value": "316034514573652620673"},
            "6": {"base": "3", "value": "2122212201122002221120200210011020220200"},
            "7": {"base": "3", "value": "20120221122211000100210021102001201112121"},
            "8": {"base": "6", "value": "20220554335330240002224253"},
            "9": {"base": "12", "value": "45153788322a1255483"},
            "10": {"base": "7", "value": "1101613130313526312514143"}
        })";

        // Parse and process both test cases
        Json::Value root1, root2;
        Json::CharReaderBuilder reader;
        string errors;
        
        istringstream iss1(json_input1);
        if (!Json::parseFromStream(reader, iss1, &root1, &errors)) {
            throw runtime_error("Failed to parse first JSON: " + errors);
        }
        
        istringstream iss2(json_input2);
        if (!Json::parseFromStream(reader, iss2, &root2, &errors)) {
            throw runtime_error("Failed to parse second JSON: " + errors);
        }
        
        long long secret1 = processTestCase(root1);
        long long secret2 = processTestCase(root2);
        
        cout << "Secret for test case 1: " << secret1 << endl;
        cout << "Secret for test case 2: " << secret2 << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}