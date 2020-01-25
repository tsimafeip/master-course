#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <iterator>
#include <numeric>

#define ll unsigned long long

const ll MAX_N = 300001;
using namespace std;

struct Node {
    ll prior, index, data;
    Node *left, *right;

    Node() {
        prior = index = data = 0;
        left = right = nullptr;
    }

    Node(ll prior_, ll data_) : Node() {
        prior = prior_;
        data = data_;
    }
};

ll get_index(Node *cur_node) {
    return cur_node ? cur_node->index : 0;
}

void update(Node *cur_node) {
    if (cur_node) cur_node->index = 1 + get_index(cur_node->left) + get_index(cur_node->right);
}

void print(Node *node) {
    if (!node) return;
    print(node->left);
    cout << node->data + 1 << " ";
    print(node->right);
}

void debug_print(Node *node) {
    if (!node) return;
    if (node->left) cout << "Printing left(data:prior:index): ";
    debug_print(node->left);
    cout << node->data + 1 << ":" << node->prior << ":" << node->index << " ";
    if (node->right) cout << "Printing right: ";
    debug_print(node->right);
}

void merge(Node *&cur_node, Node *left, Node *right) {
    if (!left || !right) {
        cur_node = left ? left : right;
    } else if (left->prior > right->prior) {
        merge(left->right, left->right, right);
        cur_node = left;
    } else {
        merge(right->left, left, right->left);
        cur_node = right;
    }

    update(cur_node);
}

void split(Node *cur_node, Node *&left, Node *&right, ll const index,
        ll const parent_add = 0) {
    if (!cur_node) {
        // leaf reached, nothing to split
        return void(left = right = nullptr);
    }

    ll cur_index = parent_add + get_index(cur_node->left);
    if (index <= cur_index) {
        split(cur_node->left, left, cur_node->left, index, parent_add);
        right = cur_node;
    } else {
        split(cur_node->right, cur_node->right, right, index,cur_index + 1);
        left = cur_node;
    }

    update(cur_node);
}

void initial_insert(Node *&cur_node, Node *node_to_add, ll const index) {
    Node* t1;
    Node* t2;
    split(cur_node, t1, t2, index);
    merge(cur_node, t1, node_to_add);
    merge(cur_node, cur_node, t2);
}

void erase_and_insert_to_start(Node *&t, ll const a, ll const b){
    Node* t1;
    Node *t2;
    Node* t3;
    split(t, t1, t2, a);
    split(t2, t2, t3, b-a+1);
    merge(t, t1, t3);
    merge(t, t2, t);
}

int main() {
    freopen("archive.in", "r", stdin);
    freopen("archive.out", "w", stdout);

    ll n, q;
    cin >> n;

    std::vector<ll> initial_vector(n, 0);
    iota(initial_vector.begin(), initial_vector.end(), 0);
    shuffle(initial_vector.begin(), initial_vector.end(), std::mt19937(std::random_device()()));

    Node *root = new Node(initial_vector[0], 0);

    for (ll i = 1; i < n; i++) {
        Node *cur_node = new Node(initial_vector[i], i);
        initial_insert(root, cur_node, i);
    }

    cin >> q;
    ll a, b;
    for (ll i = 0; i < q; i++) {
        cin >> a;
        cin >> b;
        erase_and_insert_to_start(root, a-1, b-1);
    }
    print(root);

    return 0;
}