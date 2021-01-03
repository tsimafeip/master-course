#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>

std::vector<int> prefix_func(const std::string& s) {
    int n = s.length();
    std::vector<int> prefix_res(n, 0);
    for (int i = 1; i < n; i++){
        int j = prefix_res[i-1];
        while (j > 0 && s[i] != s[j])
            j = prefix_res[j-1];

        if (s[i] == s[j]) j++;
        prefix_res[i] = j;
    }

    return prefix_res;
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, k;
    std::string input_str;

    std::cin >> n >> input_str;

    std::vector<int> prefix_vector = prefix_func(input_str);
    for (int i = 1; i < n; i++){
        if (prefix_vector[i] == 0)
            continue;

        k = i + 1 - prefix_vector[i];

        if ((i + 1) % k == 0){
            std::cout << i + 1 << " " << (i + 1) / k << "\n";
        }
    }


    return 0;
}