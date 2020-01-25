#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

#define ll unsigned long long

const ll MAX_COORD = 500020;
using namespace std;

struct Node{
    ll real_update;
    ll parent_update;

    ll overall(){
        return real_update + parent_update;
    }

    Node(){
        real_update = 0;
        parent_update = 0;
    }
};

void get_node_value(vector<Node> &tree, ll index, ll left_border, ll right_border,
        ll left_request, ll right_request, ll x_to_increment, ll &requested_overall) {
    if (left_border > right_border or left_request > right_request
    or left_border < left_request or right_border < right_request)
        return;

    if (left_border == left_request and right_request == right_border) {
//        cout << "Left request: " << left_request << endl;
//        cout << "Right request: " << right_request << endl;
        requested_overall+=tree[index].overall();
//        cout << "requested overall: " << requested_overall << endl;
        if (right_request == x_to_increment)
            tree[index].real_update+=1;
//        cout << "Level: " << tree[index].overall() << endl;
    }
    else {
        ll mid = (left_border + right_border) / 2;
        get_node_value(tree, index * 2, left_border, mid,
                left_request, min(mid, right_request), x_to_increment, requested_overall);
        get_node_value(tree, index * 2 + 1, mid+ 1,
                right_border, max(mid+1, left_request), right_request, x_to_increment, requested_overall);

//        cout << "**********************"<< endl;
//        cout << "Left border: " << left_border << endl;
//        cout << "Right border: " << right_border << endl;
//        cout << "Left: " << tree[index*2].overall() << endl;
//        cout << "Right: " << tree[index*2 + 1].overall() << endl;
//        cout << "Real value before: " << tree[index].real_update << endl;
//        cout << "Parent value before: " << tree[index].parent_update << endl;
        tree[index].parent_update = tree[index*2].overall() + tree[index*2 + 1].overall();
//        cout << "Real value after: " << tree[index].real_update << endl;
//        cout << "Parent value after: " << tree[index].parent_update << endl;
//        cout << "**********************"<< endl;

        return;
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    ll n;
    cin >> n;
    vector<ll> levels_array(n, 0);
    vector<Node> tree(MAX_COORD * 4, Node());

    ll x;
    ll y;
    for (ll i = 0; i < n; i++) {
        cin >> x;
        cin >> y;
        ll requested_overall = 0;
        get_node_value(tree, 1, 0, MAX_COORD-1, 0, x, x, requested_overall);
        // cout << "!!!!!!!!!!!requested overall FINAL: " << requested_overall << endl;
        levels_array[requested_overall]++;
        // cout << "*****************************************************************************"<< endl;
    }
    copy(levels_array.begin(), levels_array.end(), ostream_iterator<ll>(cout, "\n"));

    return 0;
}