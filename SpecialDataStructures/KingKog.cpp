#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;

class Node{
public :
    long long node_max, durations_sum;

    void update_parent_node(Node const& left_son, Node const& right_son){
        node_max = max(left_son.node_max + right_son.durations_sum, right_son.node_max);
        durations_sum = left_son.durations_sum + right_son.durations_sum;
    }
};

void build_tree(vector<Node> &tree, long long index, long long left_border, long long right_border){
    if (right_border == left_border){
        tree[index].node_max = left_border;
    }
    else{
        long long middle_of_interval = (left_border + right_border) / 2;
        build_tree(tree, index*2, left_border, middle_of_interval);
        build_tree(tree, index*2+1, middle_of_interval + 1, right_border);

        Node left_son = tree[index*2];
        Node right_son = tree[index*2+1];
        tree[index].update_parent_node(left_son, right_son);
    }
}

void query_request(vector<Node> const &tree, long long index, long long left_border, long long right_border,
        long long time_of_princess_join, long long& current_result){
    if (right_border <= time_of_princess_join){
        Node current_node = tree[index];
        current_result = max(current_result+current_node.durations_sum, current_node.node_max);
    }
    else{
        long long middle_of_interval = (left_border + right_border) / 2;

        query_request(tree, index*2, left_border,
                      middle_of_interval, time_of_princess_join, current_result);

        if (middle_of_interval < time_of_princess_join) {
            query_request(tree, index * 2 + 1, middle_of_interval + 1,
                          right_border, time_of_princess_join, current_result);
        }
    }
}

void update(vector<Node> &tree, long long index, long long left_border,
        long long right_border, long long time_of_knight_join, long long duration){
    if (right_border == left_border){
        tree[index].durations_sum+=duration;
        tree[index].node_max+=duration;
    }
    else{
        long long middle_of_interval = (left_border + right_border) / 2;

        time_of_knight_join <= middle_of_interval
        ? update(tree, index*2, left_border, middle_of_interval, time_of_knight_join, duration)
        : update(tree, index*2+1, middle_of_interval + 1, right_border, time_of_knight_join, duration);

        Node left_son = tree[index*2];
        Node right_son = tree[index*2+1];
        tree[index].update_parent_node(left_son, right_son);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    long long max_tree_count = 1000000+10;

    char operation;

    long long query_return;
    vector<long long> out;
    long long operations_count;
    cin >> operations_count;

    vector<Node> tree(max_tree_count*4);
    build_tree(tree, 1, 1, max_tree_count);

    vector<long long> left_operands(max_tree_count);
    vector<long long> values(max_tree_count);
    for (long long i = 1; i <= operations_count; i++){
        cin >> operation;
        cin >> left_operands[i];
        if (operation == '?'){
            query_return = 0;
            query_request(tree, 1, 1, max_tree_count, left_operands[i], query_return);
            out.push_back(max(query_return-left_operands[i], (long long) 0));
        }
        else if (operation == '+'){
            cin >> values[i];
            update(tree, 1, 1, max_tree_count, left_operands[i], values[i]);
        }
        else if (operation == '-'){
            update(tree, 1, 1, max_tree_count,
                   left_operands[left_operands[i]], -values[left_operands[i]]);
        }
    }

    copy(out.begin(), out.end(), ostream_iterator<long long>(cout, "\n"));

    return 0;
}