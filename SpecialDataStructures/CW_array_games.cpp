#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>
#include <bitset>
using namespace std;

class Node{
    bitset<1000> mask;
public:
    void init_node(int value){
        mask&=0;
        mask.set(value-1);
    }

    void update_parent_node(Node const &left_son, Node const &right_son){
        mask = left_son.mask^right_son.mask;
    }

    bitset<1000> get_mask() const {
        return mask;
    }

    void print_node() const{
        cout << "*******************************"<< "\n";
        cout << mask << ' ';
        cout << "*******************************"<< "\n";
    }
};

void build_tree(vector<Node> &tree, vector<int> const & initial_arr, long long index, long long left_border, long long right_border){
    if (right_border == left_border){
        int leaf_value = initial_arr[left_border];
        tree[index].init_node(leaf_value);
    }
    else{
        long long middle_of_interval = (left_border + right_border) / 2;
        build_tree(tree, initial_arr, index*2, left_border, middle_of_interval);
        build_tree(tree, initial_arr, index*2+1, middle_of_interval + 1, right_border);

        Node left_son = tree[index*2];
        Node right_son = tree[index*2+1];
        tree[index].update_parent_node(left_son, right_son);
    }
}

void set_element(vector <Node> & tree, long long cur_node_index, long long pos, int value,
        long long left_border, long long right_border){

    vector<long long> visited_nodes;

    while (left_border != right_border){
        visited_nodes.push_back(cur_node_index);
        long long mid = (left_border + right_border) / 2;
        if (pos <= mid) {
            right_border = mid;
            cur_node_index*=2;
        }
        else{
            left_border = mid + 1;
            cur_node_index = (cur_node_index*2 + 1);
        }
    }

    tree[cur_node_index].init_node(value);
    for (long long i = visited_nodes.size()-1; i >= 0; i--){
        tree[visited_nodes[i]].update_parent_node(tree[visited_nodes[i]*2], tree[visited_nodes[i]*2 + 1]);
    }
}

bitset<1000> get_bitset(vector <Node> const & tree, long long index, long long left_border,
        long long right_border, long long left_request, long long right_request){
    if (left_border > right_border or left_request > right_request)
        return bitset<1000>();

    if (left_border == left_request and right_border == right_request){
        return tree[index].get_mask();
    }

    long long mid = (left_border + right_border) / 2;
    bitset<1000> left_bitset = get_bitset(tree, 2*index, left_border, mid, left_request, min(mid, right_request));
    bitset<1000> right_bitset = get_bitset(tree, 2*index+1, mid+1, right_border,
                                    max(mid+1, left_request), right_request);

    return left_bitset^right_bitset;
}

void print_tree(vector <Node> const & tree){
    cout << "*******************************"<< "\n";
    for (long long i = 0; i < tree.size(); i++) {
        auto node = tree.at(i);

        cout << i << ";mask: "  << node.get_mask() << "\n";
    }
    cout << "*******************************"<< "\n";
}

int main() {
    freopen("array.in", "r", stdin);
    freopen("array.out", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    char operation;
    long long left_operand;
    long long right_operand;
    bitset<1000> odd_bitset;
    int odd_amount = 0;
    vector<int> out;
    long long array_len;
    cin >> array_len;
    long long operations_count;
    cin >> operations_count;

    vector <int> initial_arr;
    int current_element;

    for (long long i = 0; i < array_len; i++){
        cin >> current_element;
        initial_arr.push_back(current_element);
    }

    vector<Node> tree(array_len*4);
    build_tree(tree, initial_arr, 1, 0, array_len-1);

    for (long long i = 0; i < operations_count; i++){
        cin >> operation;
        cin >> left_operand;
        cin >> right_operand;
        if (operation == 'S'){
            set_element(tree,1, left_operand - 1, right_operand,
                    0, array_len-1);
        }
        else if (operation == 'Q'){
            odd_bitset = get_bitset(tree, 1, 0, array_len-1,
                    left_operand-1, right_operand-1);
            odd_amount = odd_bitset.count();
            out.push_back(odd_amount);
        }
    }

    copy(out.begin(), out.end(), ostream_iterator<int>(cout, "\n"));

    return 0;
}