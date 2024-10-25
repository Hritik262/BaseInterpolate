# Base Conversion & Lagrange Interpolation Tool

This project performs base conversion and Lagrange interpolation on data points from JSON input to solve polynomial interpolation problems. The tool is capable of handling data in various bases and includes robust error handling for various edge cases.

## Features

- **Base Conversion**: Converts numbers in any base (2 to 16) to decimal.
- **Lagrange Interpolation**: Calculates the interpolated result using high precision to handle up to `k` points.
- **Robust Error Handling**: Manages edge cases such as invalid base values, overflow in calculations, and division by zero.

## Project Structure

- **decodeBase**: Converts a number from a specified base to decimal.
- **lagrangeInterpolation**: Calculates the interpolated value using Lagrange polynomial interpolation.
- **processTestCase**: Reads JSON input, validates, converts bases, and performs interpolation.
  
## Requirements

- **C++ Standard**: Requires C++11 or later.
- **Dependencies**: JSONCPP library for parsing JSON data.

## Getting Started

### Prerequisites

Ensure you have the following installed:
- **C++ compiler** (e.g., GCC, Clang) that supports C++11 or later
- **JSONCPP** library for JSON parsing (install using package manager or build from source)
