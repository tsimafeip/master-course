#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>

std::vector<int> prefix_func(const std::string& s, int pattern_len) {
    int n = s.length();
    std::vector<int> prefix_res(n, 0);
    std::vector<int> positions_found;
    for (int i = 1; i < n; i++){
        int j = prefix_res[i-1];
        while (j > 0 && s[i] != s[j])
            j = prefix_res[j-1];

        if (s[i] == s[j]) j++;
        prefix_res[i] = j;
        if (j == pattern_len) {
            positions_found.push_back(i);
        }
    }

    return positions_found;
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    std::string text, pattern;
    int n;
    std::cin >> n >> pattern >> text;

    std::string combined_text = pattern + "$" + text + text;
    std::vector<int> pattern_places = prefix_func(combined_text, n);

    std::cout << (pattern_places.empty() ? -1 : 3*n - pattern_places.back());

    return 0;
}