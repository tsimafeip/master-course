#include <iostream>
#include <vector>
#include <bitset>
#include <list>
#include <queue>
#include <math.h>

#define ll unsigned long long
#define ushort ll
using namespace std;

const ushort MAX_N = 2000;
// 63 (0 bit reserved)*32 = 2016
const ushort lines_count = 32;
const ushort bytes_in_line = 63;

ushort n;
ll res;

class custom_bitset {
    static pair<ushort, ushort> get_line_pos(ushort num){
        ushort line_num = num / bytes_in_line;
        ushort pos_num = num % bytes_in_line;
        return make_pair(line_num, pos_num);
    }
public:
    vector<int64_t> bit_matrix;

    custom_bitset() {
        bit_matrix.resize(lines_count, 0);
    }

    custom_bitset(const custom_bitset &b1, const custom_bitset &b2) : custom_bitset(){
        for (ushort i = 0; i < lines_count; i++)
            bit_matrix[i] = b1.bit_matrix[i] & b2.bit_matrix[i];
    }

    void reset_bit(ushort num) {
        pair<ushort, ushort> line_pos = get_line_pos(num);
        bit_matrix[line_pos.first] &= (~(1LL << line_pos.second));
    }

    void set_bit(ushort num) {
        pair<ushort, ushort> line_pos = get_line_pos(num);
        bit_matrix[line_pos.first] |= (1LL << line_pos.second);
    }

    ushort next_set_bit(){
        for(ushort i = 0; i < lines_count; i++){
            if (bit_matrix[i] == 0) {
                //cout << "in" << endl;
                continue;
            }
            //cout << "out: "<< (bitset<64>) bit_matrix[i] << endl;

            ushort first_set_bit = log2(bit_matrix[i]&-bit_matrix[i]);

//            cout << "first set bit: " << first_set_bit <<endl;
//            cout << (bitset<64>)bit_matrix[i] << endl;
//            cout << (bitset<64>)(~(1LL << first_set_bit)) << endl;
//            cout << (bitset<64>)(bit_matrix[i] & (~(1LL << first_set_bit))) << endl;

            bit_matrix[i] &= (~(1LL << first_set_bit));

            return bytes_in_line*i + first_set_bit;
        }
        return 0;
    }
};

// s represents position in bitset. real index s-1.
void BFS(vector<custom_bitset> const &adj, ushort s) {
    custom_bitset not_used;
    for (ushort i = 0; i < n; i++)
        not_used.set_bit(i+1);

    vector<ushort> dist(n, n);
    dist[s-1] = 0;

    queue<ushort> q;
    ll local_res = (n * n) * (n - 1);
    not_used.reset_bit(s);
    q.push(s);
    // cout << endl;
    custom_bitset cur_line;
    while (!q.empty()) {
        ushort cur_v = q.front();
        q.pop();
        //cout << "current v: " << cur_v << endl;
        cur_line = adj[cur_v-1];

        custom_bitset possible_ways(cur_line, not_used);
//        cout << "possible ways: ";
//        copy(possible_ways.bit_matrix.begin(), possible_ways.bit_matrix.end(), ostream_iterator<bitset<64>>(cout, " "));
//        cout <<endl;
        ushort next_w = possible_ways.next_set_bit();

        while (next_w) {
            //cout << "next_w: " << next_w << endl;
            not_used.reset_bit(next_w);
            dist[next_w-1] = dist[cur_v-1] + 1;
            //cout << "dist " << dist[next_w-1] << endl;
            local_res += (dist[next_w-1] * dist[next_w-1] - n * n);
            q.push(next_w);
            next_w = possible_ways.next_set_bit();
//            cout << "next_new_w: " << next_w << endl;
//            cout <<endl;
        }
    }

//    copy(dist.begin(), dist.end(), ostream_iterator<ll>(cout, " "));
//    cout << endl;
//    cout << res << endl;
//    cout << "**********************" << endl;
    res += local_res;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    cin >> n;
    vector<custom_bitset> adj(n);

    string s;
    for (ushort i = 0; i < n; i++) {
        cin >> s;
        for (ushort j = 0; j < n; j++) {
            if (s[j] == '1') {
                adj[i].set_bit(j+1);
            }
        }
    }

    res = 0;

    for (ushort i = 0; i < n; i++) {
          BFS(adj, i+1);
//        copy(adj[i].bit_matrix.begin(), adj[i].bit_matrix.end(), ostream_iterator<bitset<64>>(cout, " "));
//        cout << endl;
//        cout << "********************" << endl;
    }
    //cout << "********************" << endl;
    cout << res;

    return 0;
}