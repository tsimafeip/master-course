#include <iostream>
#include <tuple>
#include <vector>
#include <complex>
#include <cmath>
#include <iterator>

#define PI 3.1415926535897931
typedef std::complex<double> complex_double;

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
    size_t max_a_b = std::max(fa.size(), fb.size());
    size_t padding_max_len = pow(2, std::ceil(log2(max_a_b)));
    padding_max_len *= 2;
    // align lens and fill missed coefs with zeros
    fa.resize(padding_max_len), fb.resize(padding_max_len), int_res.resize(padding_max_len);

    // recursively count values by coefs for a and b
    compute_values(fa, false);
    compute_values(fb, false);
    for (size_t i = 0; i < padding_max_len; i++)
        fa[i] *= fb[i];

    // recursively count coefs by values
    compute_values(fa, true);

    for (size_t i = 0; i < padding_max_len; ++i) {
        int_res[i] = std::round(fa[i].real()/padding_max_len);
    }
}

void read_vector(std::vector<complex_double> &cur_vector) {
    for (size_t i = 0; i < cur_vector.size(); i++) {
        std::cin >> cur_vector[i];
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, m;

    std::cin >> n;
    std::vector<complex_double> first_polynom(n);
    read_vector(first_polynom);

    std::cin >> m;
    std::vector<complex_double> second_polynom(m);
    read_vector(second_polynom);

    std::vector<int> res;
    multiply(first_polynom, second_polynom, res);

    int i = res.size() - 1;
    while (i>= 0 && res[i] == 0) {
        i--;
    }

    res.resize(i + 1);
    std::cout << res.size() << std::endl;
    copy(res.begin(), res.end(), std::ostream_iterator<int>(std::cout, " "));

    return 0;
}