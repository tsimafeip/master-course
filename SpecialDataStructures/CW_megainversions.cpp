#include <iostream>
#include <algorithm>
#include <vector>

#define ll unsigned long long

using namespace std;

void activate(vector<ll> &tree, ll cur_index, ll left_border, ll right_border, ll num_to_activate,
        ll value_to_activate = 1) {
    // cout << "*********************************" << endl;
    vector<ll> visited_nodes;

    while (left_border != right_border) {
        visited_nodes.push_back(cur_index);
        ll mid = (left_border + right_border) / 2;
        if (num_to_activate <= mid) {
            right_border = mid;
            cur_index *= 2;
        } else {
            left_border = mid + 1;
            cur_index = (cur_index * 2 + 1);
        }
    }

    tree[cur_index] = value_to_activate;

    ll counter = visited_nodes.size();
    while (counter != 0) {
        ll i = counter - 1;
        tree[visited_nodes[i]] = tree[visited_nodes[i] * 2] + tree[visited_nodes[i] * 2 + 1];
        counter--;
    }
}

void count_activated(vector<ll> &tree, ll index, ll left_border, ll right_border,
                     ll left_request, ll right_request, ll &activated_count) {
//    cout << "**********************" << endl;
//    cout << "Left border: " << left_border << endl;
//    cout << "Right border: " << right_border << endl;
//    cout << "Left request: " << left_request << endl;
//    cout << "Right request: " << right_request << endl;
    if (left_border > right_border or left_request > right_request
        or left_border > left_request or right_border < right_request)
        return;

    if (left_border == left_request and right_request == right_border) {
        activated_count += tree[index];
        return;
    } else {
        ll mid = (left_border + right_border) / 2;
        count_activated(tree, index * 2, left_border, mid,
                        left_request, min(mid, right_request), activated_count);
        count_activated(tree, index * 2 + 1, mid + 1,
                        right_border, max(mid + 1, left_request), right_request, activated_count);

        tree[index] = tree[index * 2] + tree[index * 2 + 1];

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

    vector<ll> activation_tree((n+1) * 4, 0);
    vector<ll> inv_tree((n+1) * 4, 0);

    ll num, megainv_count = 0, inv_count = 0, activated_count = 0;
    for (ll i = 0; i < n; i++) {
        cin >> num;
        activate(activation_tree, 1, 0, n - 1, num - 1);
        count_activated(activation_tree, 1, 0, n - 1, num, n - 1, activated_count);

        activate(inv_tree, 1, 0, n-1, num-1, activated_count);
        count_activated(inv_tree, 1, 0, n - 1, num, n - 1, inv_count);
//        cout << "activated: " << activated_count << endl;
//        cout << "inv count: " << megainv_count << endl;
        megainv_count+=inv_count;
        activated_count = 0;
        inv_count = 0;
    }

    cout << megainv_count;

    return 0;
}