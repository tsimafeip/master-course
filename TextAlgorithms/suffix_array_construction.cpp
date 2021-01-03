#include <iostream>
#include <cstring>
#include <vector>

const int maxlen = 100000;
const int alphabet = 26 + 10 + 1;

std::string input_str;
int n;
int p[maxlen];

int get_char_index(char c) {
    if (c == '$')
        return 0;

    if (isdigit(c))
        return c - '0' + 1; // sentinel

    return (c - 'a') + 11; // sentinel + 10 digits
}

void calc_bwt(std::vector<char>& bwt){

    for (int i = 0; i < n; i++){
        int prev_index = p[i] - 1;
        if (prev_index < 0)
            prev_index+=n;
        bwt[i] = input_str[prev_index];
    }
}

void kasai(std::vector<int>& lcp)
{
    std::vector<int> rank(n);
    for (int i = 0; i < n; i++)
        rank[p[i]] = i;

    for (int i = 0, k = 0; i < n; i++)
    {
        if (k > 0)
            k--;

        if (rank[i] == n - 1)
        {
            lcp[n - 1] = -1;
            k = 0;
            continue;
        }

        int j = p[rank[i] + 1];
        while (std::max(i + k, j + k) < n && input_str[i + k] == input_str[j + k])
            k++;

        lcp[rank[i]] = k;
    }
    // we are calculating lcp[i] = lcp between i and i+1 suffixes.
}


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    std::cin >> input_str;
    input_str += '$';
    n = input_str.length();

    int counter_arr[maxlen], c[maxlen];

    std::memset(counter_arr, 0, alphabet * sizeof(int));
    // count occurrences of each char in input str
    for (int i = 0; i < n; ++i) {
        int char_index = get_char_index(input_str[i]);
        ++counter_arr[char_index];
    }

    // cumulative sum
    for (int i = 1; i < alphabet; ++i)
        counter_arr[i] += counter_arr[i - 1];

    // get permutation for the class in reverse order
    for (int i = 0; i < n; ++i) {
        int char_index = get_char_index(input_str[i]);
        p[--counter_arr[char_index]] = i;
    }

    // the lowest element in permutation definitely has the first class
    c[p[0]] = 0;
    int classes_count = 1;
    for (int i = 1; i < n; ++i) {
        if (input_str[p[i]] != input_str[p[i - 1]]) {
            // classes increase
            classes_count += 1;
        }

        c[p[i]] = classes_count - 1;
    }

    int p_new[maxlen], c_new[maxlen];
    int d = 1;
    while (d < n){
        for (int i = 0; i < n; ++i) {
            p_new[i] = p[i] - d;
            if (p_new[i] < 0)
                p_new[i] += n;
        }

        std::memset(counter_arr, 0, classes_count * sizeof(int));
        for (int i = 0; i < n; ++i)
            ++counter_arr[c[p_new[i]]];

        for (int i = 1; i < classes_count; ++i)
            counter_arr[i] += counter_arr[i - 1];

        for (int i = n - 1; i >= 0; --i)
            p[--counter_arr[c[p_new[i]]]] = p_new[i];

        p_new[p[0]] = 0;
        classes_count = 1;
        for (int i = 1; i < n; ++i) {
            int mid1 = (p[i] + d) % n, mid2 = (p[i - 1] + d) % n;
            if (c[p[i]] != c[p[i - 1]] || c[mid1] != c[mid2])
                ++classes_count;
            c_new[p[i]] = classes_count - 1;
        }
        memcpy(c, c_new, n * sizeof(int));

        d <<= 1;
    }

    std::cout << n - 1 << std::endl;
    for (int i = 0; i < n; i++){
        std::cout << p[i];
    }

//    std::vector<char> bwt(n);
//    calc_bwt(bwt);
//    for (int i = 0; i < n; i++){
//        std::cout << bwt[i];
//    }

    std::cout << std::endl;

    std::vector<int> lcp(n);
    kasai(lcp);
    for (int i = 1; i < n-1; i++){
        std::cout << lcp[i]<< " ";
    }

    return 0;
}