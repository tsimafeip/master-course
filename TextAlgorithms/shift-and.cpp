#include <iostream>
#include <tuple>
#include <vector>

// n < 64
const int bits_in_line = 63;
const int alphabet_size = 26;
std::string pattern, text;
int n;

class custom_bitset {
    static int get_line_pos(int num){
        int pos_num = num % bits_in_line;
        return pos_num;
    }

    void bit_shift(){
        bit_vector = bit_vector<<1LL;
    }

public:
    int64_t bit_vector;

    custom_bitset() {
        bit_vector = 0;
    }

    custom_bitset(int64_t sample_num) {
        bit_vector = sample_num;
    }

    custom_bitset(const custom_bitset &b1, const custom_bitset &b2) : custom_bitset(){
        bit_vector = b1.bit_vector & b2.bit_vector;
    }

    bool is_empty(){
        return bit_vector == 0LL;
    }

    custom_bitset bit_shift_and_return_new_vector(){
        custom_bitset res_bit_set(bit_vector);
        res_bit_set.bit_shift();
        res_bit_set.reset_bit(n+1);
        res_bit_set.set_bit(0);
        return res_bit_set;
    }

    custom_bitset custom_and(custom_bitset another_bitset) {
        return custom_bitset(bit_vector & another_bitset.bit_vector);
    }

    void reset_bit(int num) {
        int pos_num = get_line_pos(num);
        bit_vector &= (~(1LL << pos_num));
    }

    void set_bit(int num) {
        int pos_num = get_line_pos(num);
        bit_vector |= (1LL << pos_num);
    }

    bool check_if_last_bit_is_set() {
        custom_bitset test_vector;
        test_vector.set_bit(n-1);
        return bit_vector & test_vector.bit_vector;
    }
};

custom_bitset get_u_value(char c){
    custom_bitset res_vector;
    for (size_t i = 0; i < n; i++){
        if (pattern[i] == c)
            res_vector.set_bit(i);
    }
    return res_vector;
}

int main()
{
     freopen("input.txt", "r", stdin);
     freopen("output.txt", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin >> pattern >> text;
    n = pattern.length();

    std::vector<custom_bitset> u_vectors(alphabet_size);
    for (char c : pattern) {
        char char_index = (char) (c - 'a');
        if (u_vectors[char_index].is_empty()) {
            for (size_t j = 0; j < pattern.size(); j++) {
                if (pattern[j] == c) {
                    u_vectors[char_index].set_bit(j);
                }
            }
        }
    }

    custom_bitset cur_m_col, prev_m_col;
    for (size_t j = 0; j < text.length(); j++){
        prev_m_col = cur_m_col;
        char char_index = (char) (text[j] - 'a');
        cur_m_col = custom_bitset(prev_m_col.bit_shift_and_return_new_vector(), u_vectors[char_index]);
        if (cur_m_col.check_if_last_bit_is_set())
            std::cout << j-n+1 << " ";
    }

    return 0;
}