#pragma once
#include <cinttypes>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <cstring>


struct int2023_t {
    static const size_t kByteSize = 253;
    uint8_t digit[kByteSize];
    int2023_t();

    int2023_t(const int2023_t& other);

    ~int2023_t();

    bool IsPositive() const;

    static int2023_t Abs(const int2023_t& digit);

    int2023_t LeftByteShift(unsigned int k);
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t ConvertFromInt(int32_t i);

int2023_t ConvertFromString(const char* buff);

int2023_t operator-(const int2023_t& value);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int32_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

bool operator>=(const int2023_t& lhs, const int2023_t& rhs);

bool operator>(const int2023_t& lhs, const int2023_t& rhs);

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);

static const int32_t kBase = 256;
static const size_t kByteShift = 8;
static const size_t kSignBit = 6;
