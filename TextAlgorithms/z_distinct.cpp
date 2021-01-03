#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <tuple>

int z_func(std::string s) {
    int n = s.length();
    std::vector<int> z(n);
    int z_max = 0;

    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r)
            z[i] = std::min(r - i + 1, z[i - l]);

        while (i + z[i] < n && s[i+z[i]] == s[z[i]])
            z[i]++;

        if (i+z[i]-1 > r) {
            r = i + z[i] - 1;
            l = i;
        }

        if (z_max < z[i]) {
            z_max = z[i];
        }
    }

    return z_max;
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    std::string input_str;
    std::cin >> input_str;

    int res = 0;

    std::string processed_str, cur_str;
    for (int i = 0; i < (int)input_str.length(); i++) {
        char cur_symbol = input_str[i];
        cur_str = processed_str + cur_symbol;
        std::reverse(cur_str.begin(), cur_str.end());
        int z_max = z_func(cur_str);
        res += (i + 1 - z_max);

        processed_str += cur_symbol;
    }

    std::cout << res;
    return 0;
}