#include "number.h"


bool int2023_t::IsPositive() const {
    return ((digit[0] >> kSignBit) & 1) == 0;
}

int2023_t int2023_t::Abs(const int2023_t& number){
    if (number.IsPositive()) {
        return number;
    } else {
        return -number;
    }
}

int2023_t::int2023_t() {
    std::fill_n(digit, int2023_t::kByteSize, 0);
}

int2023_t::int2023_t(const int2023_t& other) {
    std::memcpy(digit, other.digit, int2023_t::kByteSize);
}

int2023_t operator-(const int2023_t& value){
    int2023_t result;

    for(size_t i = 0; i < int2023_t::kByteSize; ++i){
        result.digit[i] = ~value.digit[i];
    }
    int2023_t one = int2023_t();
    one.digit[int2023_t::kByteSize - 1] = 1;
    return result + one;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    uint16_t cur = 0;
    for (int i = int2023_t::kByteSize - 1; i != -1; --i) {
        cur = cur + static_cast<uint16_t>(lhs.digit[i]) + static_cast<uint16_t>(rhs.digit[i]);
        result.digit[i] = static_cast<uint8_t>(cur);
        cur >>= kByteShift;
    }
    
    return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    return lhs + (-rhs);
}

int2023_t ConvertFromInt(int32_t num) {
    bool is_negative = (num < 0);
    num = abs(num);
    int2023_t result = int2023_t();
    result.digit[int2023_t::kByteSize-1] = static_cast<uint8_t>(num);
    result.digit[int2023_t::kByteSize-2] = static_cast<uint8_t>(num >>= 8);
    result.digit[int2023_t::kByteSize-3] = static_cast<uint8_t>(num >>= 8);
    result.digit[int2023_t::kByteSize-4] = static_cast<uint8_t>(num >>= 8);
    return (is_negative)? -result: result;
}

int2023_t ConvertFromString(const char* str) {
    if (std::strlen(str) == 0) {
        return int2023_t();
    }
    int2023_t result = int2023_t();

    if (str[0] == '-') {
        for (int i = 1; i != std::strlen(str); ++i) {
            result = result * ConvertFromInt(10);
            result = result + ConvertFromInt(str[i] - '0');
        }
        result = -result;

    } else if (str[0] == '+') {
        for (int i = 1; i != std::strlen(str); ++i) {
            result = result * ConvertFromInt(10);
            result = result + ConvertFromInt(str[i] - '0');
        }

    } else {
        for (int i = 0; i != std::strlen(str); ++i) {
            result = result * ConvertFromInt(10);
            result = result + ConvertFromInt(str[i] - '0');
        }
    }

    return result;
}

int2023_t int2023_t::LeftByteShift(unsigned int shift) {
    if (shift == 0) 
        return *this;
    int2023_t result;
    
    for (int i = 1; i != int2023_t::kByteSize - shift; ++i) {
        result.digit[i] = this->digit[i + shift];
    }
    
    result.digit[0] &= 0b00111111;
    std::fill(result.digit + int2023_t::kByteSize - shift, result.digit + int2023_t::kByteSize, 0);
    return result;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result = int2023_t();
    const int2023_t abs_lhs = int2023_t::Abs(lhs);
    const int2023_t abs_rhs = int2023_t::Abs(rhs);
    unsigned int shift = 0;

    for (int i = int2023_t::kByteSize - 1; i != -1; --i) {
        if (abs_rhs.digit[i] == 0) {
            ++shift;
            continue;
        }
        
        int2023_t cur;
        uint16_t remainder = 0;
        for (int j = int2023_t::kByteSize - 1; j != -1; --j) {
            remainder = static_cast<uint16_t>(abs_lhs.digit[j]) * static_cast<uint16_t>(abs_rhs.digit[i]) + remainder;
            cur.digit[j] = static_cast<uint8_t>(remainder);
            remainder >>= kByteShift;
        }
        result = result + (cur.LeftByteShift(shift++));
    }
    return (lhs.IsPositive() ^ rhs.IsPositive())? -result: result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    if (rhs == int2023_t()){
        std::cerr << "Error: Integer division by zero.";
        exit(0);
    }
    if (std::count(rhs.digit, rhs.digit + int2023_t::kByteSize, 0) == int2023_t::kByteSize) {
        return int2023_t();
    }

    int2023_t abs_lhs = int2023_t::Abs(lhs);
    int2023_t abs_rhs = int2023_t::Abs(rhs);
    if (abs_rhs > abs_lhs) {
        return int2023_t();
    }

    int2023_t result = int2023_t();
    int i = 0;
    for (int j = 0; j != int2023_t::kByteSize; ++j) {
        if (abs_lhs.digit[j] != 0) {
            i = j;
            break;
        }
    }

    for (; i != int2023_t::kByteSize; ++i) {
        int16_t left = -1;
        int16_t right = kBase;
        while (right - left > 1) {
            result.digit[i] = (left + right) / 2;
            int2023_t cur = result * abs_rhs;
            if (cur == abs_lhs) {
                return (lhs.IsPositive() ^ rhs.IsPositive())? -result: result;
            }
            if (cur > abs_lhs || !cur.IsPositive()) {
                right = result.digit[i];
            } else {
                left = result.digit[i];
            }
        }
        result.digit[i] = left;
    }
    return (lhs.IsPositive() ^ rhs.IsPositive())? -result: result;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    return std::equal(lhs.digit, lhs.digit + int2023_t::kByteSize, rhs.digit);
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
    return (lhs - rhs).IsPositive();
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
    return lhs >= rhs && lhs != rhs;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs >= rhs) ;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    if (!value.IsPositive()) {
        stream << "-";
        stream << -value;
        return stream;
    }

    stream << "0b";
    for (int bit = 5; bit != -1; --bit) {
        stream << ((value.digit[0] >> bit) & 1);
    }

    for (int i = 1; i != int2023_t::kByteSize; ++i) {
        for (int bit = 7; bit != -1; --bit) {
            stream << ((value.digit[i] >> bit) & 1);
        }
    }
    return stream;
}
