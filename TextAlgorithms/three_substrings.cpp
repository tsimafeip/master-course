#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <tuple>
#include <iterator>

std::tuple<std::vector<int>, int> z_func(std::string s) {
    int n = s.length();
    std::vector<int> z(n);

    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r)
            z[i] = std::min(r - i + 1, z[i - l]);

        while (i + z[i] < n && s[i + z[i]] == s[z[i]])
            z[i]++;

        if (i + z[i] - 1 > r) {
            r = i + z[i] - 1;
            l = i;
        }
    }

    // to avoid overlays
    int max_item = 0;
    for (int j = 1; j < n; j++){
        z[j] = std::min(z[j], j);
        if (max_item < z[j])
            max_item = z[j];
    }

    return std::make_tuple(z, max_item);
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    std::string input_str;
    std::cin >> input_str;

    int n = input_str.length();
    std::vector<std::tuple<std::vector<int>, int>> z_vector(n);
    for (int i = 0; i < n; i++) {
        z_vector[i] = z_func(input_str.substr(i));
//        copy(std::get<0>(z_vector[i]).begin(), std::get<0>(z_vector[i]).end(),
//                std::ostream_iterator<int>(std::cout, " "));
//
//        std::cout << "***" << std::get<1>(z_vector[i]) << std::endl;
    }

    int max_index = 0, max_substr_len = 0, cur_len = 0;
    for (int i = 0; i < n+1; i++){
        for (int j = 1; j + i < n; j++){
             cur_len = std::min(std::get<1>(z_vector[i+j]), (std::get<0>(z_vector[i]))[j]);
             if (cur_len > max_substr_len){
                 max_substr_len = cur_len;
                 max_index = i;
             }
        }
    }

    std::string max_substr = input_str.substr(max_index, max_substr_len);
    std::cout << max_substr;
    return 0;
}