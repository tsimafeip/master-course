#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#define ll unsigned long long
using namespace std;

class HelpingNode{
public:
    ll value;
    ll prev_index;
    ll cur_index;

    bool operator < (const HelpingNode& str) const
    {
        return (value < str.value);
    }
};

bool operator < (const long long unsigned int left, const HelpingNode & node) {
    return left < node.value;
}

bool operator < (const HelpingNode & node, const long long unsigned int right) {
    return node.value < right;
}

class Node{
public :
    ll value;
    Node* left_son;
    Node* right_son;

    Node(){
        value = 0;
        left_son = nullptr;
        right_son = nullptr;
    }

    Node(Node* l, Node* r, ll value_){
        value = value_;
        left_son = l;
        right_son = r;
    }
};

Node* root_pointers[200001];

void build_tree(Node* node, ll left_border, ll right_border){
    if (right_border == left_border){
        return;
    }
    else{
        ll middle_of_interval = (left_border + right_border) / 2;
        node->left_son = new Node();
        node->right_son = new Node();
        build_tree(node->left_son, left_border, middle_of_interval);
        build_tree(node->right_son, middle_of_interval + 1, right_border);
    }
}

void activate_new_node(Node* prev_node, Node* cur_node, ll left_border, ll right_border, ll const index){
    if (index < left_border or index > right_border or right_border < left_border)
        return;

    if (left_border == right_border){
        cur_node->value = 1;
        return;
    }

    ll mid = (left_border + right_border) / 2;

    if (index <= mid){
        cur_node->right_son = prev_node->right_son;
        cur_node->left_son = new Node();
        activate_new_node(prev_node->left_son, cur_node->left_son, left_border, mid, index);
    }
    else{
        cur_node->left_son = prev_node->left_son;
        cur_node->right_son = new Node();

        activate_new_node(prev_node->right_son, cur_node->right_son, mid+1, right_border, index);
    }
    cur_node->value = 0;
    if (cur_node->left_son != nullptr){
        cur_node->value+=cur_node->left_son->value;
    }
    if (cur_node->right_son != nullptr){
        cur_node->value+=cur_node->right_son->value;
    }
}

ll request_result(Node* node, ll left_border, ll right_border, ll left_request, ll right_request){
    // request to the appropriate version of the segment tree
    if (right_border < left_border or left_request > right_request)
        return 0;

    if (left_border == left_request && right_border == right_request){
//        cout << left_border << ": left border; right border: " << right_border << '\n';
//        cout << node->value << ": result\n";
        return node->value;
    }

    ll mid = (left_border + right_border) / 2;
    ll left_value = request_result(node->left_son, left_border, mid, left_request, min(mid, right_request));
    ll right_value = request_result(node->right_son, mid+1, right_border,
            max(mid+1, left_request), right_request);

    return left_value + right_value;
}

int main() {
//    freopen("array.in", "r", stdin);
//    freopen("array.out", "w", stdout);
    ll n;
    ll q;
    cin >> n;
    cin >> q;
    vector<HelpingNode> sorted_nums(n);
    Node* root = new Node();
    build_tree(root, 0, n-1);
    root_pointers[0] = root;

//    cout << n << ": n \n";
//    cout << q << ": q \n";
    ll value = 0;
    for (ll i = 0; i < n; i++){
        cin >> value;
//        cout << i << ": " << value << "\n";

        sorted_nums[i].prev_index = i;
        sorted_nums[i].value = value;
    }
//    cout << "****************************\n";
    sort(sorted_nums.begin(), sorted_nums.end());

    for (ll i = 0; i < n; i++){
        sorted_nums[i].cur_index = i;
//        cout << sorted_nums[i].prev_index << ": " << sorted_nums[i].value << "\n";

        Node* new_root = new Node();
        activate_new_node(root_pointers[i], new_root, 0, n-1, sorted_nums[i].prev_index);
        root_pointers[i+1] = new_root;
    }

//    cout << root_pointers[n]->value << "****************************\n";

    ll left_request;
    ll right_request;
    ll lower_value;
    ll upper_value;

    vector<ll> out(q, 0);
    for (ll i = 0; i < q; i++){
        cin >> left_request;
        cin >> right_request;
        cin >> lower_value;
        cin >> upper_value;

//        cout << "****************************\n";
//        cout << "lower value: " << lower_value << "\n";
        ll lower_index = 0;
        ll lower_result = 0;
        auto it = lower_bound(sorted_nums.begin(), sorted_nums.end(), lower_value);

        if (it == sorted_nums.end())
            continue;

        if (it != sorted_nums.begin()) {
            it--;
            lower_index = it->cur_index + 1;

//            cout << "lower index: " << lower_index << "\n";
            Node *lower_version_root = root_pointers[lower_index];
            lower_result = request_result(lower_version_root, 0, n - 1,
                                          left_request - 1, right_request - 1);
        }
//        cout << "lower result: " << lower_result << "\n";

        ll upper_index = 0;
        auto up_it = upper_bound(sorted_nums.begin(), sorted_nums.end(), upper_value);

        if (up_it == sorted_nums.begin())
            continue;

        up_it--;
        upper_index = up_it->cur_index+1;

//        cout << "upper index: " << upper_index << "\n";
        Node* upper_version_root = root_pointers[upper_index];
        ll upper_result = request_result(upper_version_root, 0, n-1,
                left_request-1, right_request-1);

//        cout << "upper result: " << upper_result << "\n";

        out[i] = upper_result-lower_result;
    }

    copy(out.begin(), out.end(), ostream_iterator<ll>(cout, "\n"));

    return 0;
}