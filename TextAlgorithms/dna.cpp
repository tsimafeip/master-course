#include <iostream>
#include <tuple>
#include <vector>
#include <complex>
#include <cmath>
#include <iterator>

#define PI 3.1415926535897931
typedef std::complex<double> complex_double;
const std::string alphabet = "ABCD";

void compute_values(std::vector<complex_double> &a, bool return_coefs_by_values) {
    int n = a.size();
    if (n == 1) return;

    std::vector<complex_double> b(n / 2), c(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        b[j] = a[i];
        c[j] = a[i + 1];
    }
    compute_values(b, return_coefs_by_values);
    compute_values(c, return_coefs_by_values);

    double angle = (2 * PI / n) * (return_coefs_by_values ? -1 : 1);
    complex_double w = 1, wn(cos(angle), sin(angle));
    for (int i = 0; i < n / 2; i++) {
        a[i] = b[i] + w * c[i];
        a[i + n / 2] = b[i] - w * c[i];
        w *= wn;
    }
}

void multiply(std::vector<complex_double> &fa, std::vector<complex_double> &fb, std::vector<int> &int_res) {
    // recursively count values by coefs for a and b
    compute_values(fa, false);
    compute_values(fb, false);
    int n = fa.size();
    for (size_t i = 0; i < n; i++)
        fa[i] *= fb[i];

    // recursively count coefs by values
    compute_values(fa, true);

    int_res.resize(n);
    for (size_t i = 0; i < n; ++i) {
        int_res[i] = std::round(fa[i].real() / n);
    }
}

int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int m;
    std::string first_string, second_string;

    std::cin >> m >> first_string >> second_string;

    std::vector<int> final_vector(m * 2);
    for (size_t j = 0; j < alphabet.length(); j++) {
        std::vector<complex_double> first_vector(2 * m, 0), second_vector(2 * m, 0);
        char cur_symbol = alphabet[j];

        for (size_t i = 0; i < m; i++) {
            if (cur_symbol == first_string[i]) {
                first_vector[i] = 1;
                first_vector[i + m] = 1;
            }

            if (cur_symbol == second_string[m - i - 1]) {
                second_vector[i] = 1;
            }
        }

        std::vector<int> fft_result;
        multiply(first_vector, second_vector, fft_result);

        for (size_t k = 0; k < fft_result.size(); k++) {
            final_vector[k] += fft_result[k];
        }
    }

    long long max_element = -1, max_index = -1;
    for (size_t k = m - 1; k < final_vector.size(); k++) {
        if (final_vector[k] > max_element) {
            max_element = final_vector[k];
            max_index = k;
        }
    }

    if (max_element == -1) {
        std::cout << 0 << " " << 0;
    } else {
        std::cout << max_element << " " << max_index - m + 1;
    }


    return 0;
}