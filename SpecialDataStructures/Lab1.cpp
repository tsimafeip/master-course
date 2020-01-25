#include <iostream>
#include <stack>
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

void push_to_stack(stack<pair<int, int> > &st, int const &element_to_push) {
    if (st.empty()) {
        st.push(make_pair(element_to_push, element_to_push));
    } else {
        int minimum = min(element_to_push, st.top().second);
        st.push(make_pair(element_to_push, minimum));
    }
}

int get_min(stack<pair<int, int> > const &st_1, stack<pair<int, int> > const &st_2) {
    if (st_1.empty() && st_2.empty())
        return -1;

    if (st_1.empty())
        return st_2.top().second;

    if (st_2.empty())
        return st_1.top().second;

    return min(st_1.top().second, st_2.top().second);
}

void pop_from_stack(stack<pair<int, int> > &st_to_pop, stack<pair<int, int> > &another_stack) {
    if (st_to_pop.empty()) {
        stack<int> temp;
        auto mid = another_stack.size() / 2 + 1;
        while (another_stack.size() > mid) {
            temp.push(another_stack.top().first);
            another_stack.pop();
        }

        while (another_stack.size() != 1) {
            push_to_stack(st_to_pop, another_stack.top().first);
            another_stack.pop();
        }
        another_stack.pop();

        while (!temp.empty()) {
            push_to_stack(another_stack, temp.top());
            temp.pop();
        }
    } else {
        st_to_pop.pop();
    }
}

int main() {
    freopen("archive.in", "r", stdin);
    freopen("archive.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int operations_count;
    cin >> operations_count;

    int element_to_add;
    string operation;
    stack<pair<int, int> > s_left, s_right;
    vector<int> out;

    for (int i = 0; i < operations_count; i++) {
        cin >> operation;
        if (operation == "+L") {
            cin >> element_to_add;
            push_to_stack(s_left, element_to_add);
        } else if (operation == "+R") {
            cin >> element_to_add;
            push_to_stack(s_right, element_to_add);
        } else if (operation == "-L") {
            pop_from_stack(s_left, s_right);
        } else if (operation == "-R") {
            pop_from_stack(s_right, s_left);
        }

        out.push_back(get_min(s_left, s_right));
    }

    copy(out.begin(), out.end(), ostream_iterator<int>(cout, "\n"));

    return 0;
}