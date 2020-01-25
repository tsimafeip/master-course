#include <iostream>
#include <set>
#include <string>
#include <stack>
#define ll unsigned long long
using namespace std;

int main() {
    freopen("array.in", "r", stdin);
    freopen("array.out", "w", stdout);

    string result;
    bool correct_end = true;
    ll candidate;
    ll first;
    cin >> first;

    stack<ll> st;
    st.push(first);

    set<ll> used_leaves;
    set<ll> current_used_set;
    current_used_set.insert(first);

    while (cin >> candidate){//1
        ll cur_top = st.top();//1
        if (candidate == cur_top)
        {
            result = "NO";
            correct_end = false;
            break;
        }

        st.pop();//1
        if(!st.empty()) {
            if (used_leaves.find(candidate) != used_leaves.end()) {
                result = "NO";
                correct_end = false;
                break;
            }

            ll prev_value = st.top();//1

            if (candidate == prev_value) {
                used_leaves.insert(cur_top);
                current_used_set.erase(cur_top);
                continue;
            }

            if (current_used_set.find(candidate) == current_used_set.end()){
                st.push(cur_top);
                st.push(candidate);
                current_used_set.insert(candidate);
            }
            else {
                correct_end = false;
                result = "NO";
            }
        }
        else{
            if (used_leaves.find(candidate) != used_leaves.end()) {
                result = "NO";
                correct_end = false;
                break;
            }
            st.push(cur_top);
            st.push(candidate);
            current_used_set.insert(candidate);
        }
    }

    if (correct_end && st.size() == 1 && st.top() == first)
        result = "YES";

    cout << result;
    return 0;
}