#pragma GCC optimize("O3")
#include <C:/Users/Alisa/github-classroom/is-itmo-c-23/labwork2-ashlixx/lib/number.h>
#include <iostream>



int main() {
    fast();
    int2023_t value1, value2, value;
    int32_t input_int1;
    std::cin >> input_int1;
    char input_string[kNumberSize];
    std::cin >> input_string;
    value1 = from_int(input_int1);
    value2 = from_string(input_string);
    value = value2 / value1;
    std::cout << value;
    return 0;
}
