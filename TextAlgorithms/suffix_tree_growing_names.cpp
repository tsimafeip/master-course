#include <iostream>
#include <cstring>
#include <tuple>
#include <queue>
#include <algorithm>
#include <map>

struct Node {
    std::map<char, int> next;
    std::map<char, int> go;
    bool is_terminal;
    int parent_index;
    char parent_char;
    int suffix_link;
    int local_max_len;
};

Node create_new_node(int p, char c) {
    Node v{};
    v.parent_index=p;
    v.parent_char=c;
    v.suffix_link = -1;
    v.is_terminal = false;
    return v;
}

std::vector<Node> trie;
int global_max = 1;

void add_pattern_to_trie(const std::string &pattern) {
    int v = 0;
    for (char c : pattern) {
        char char_index = (char)(c - 'a');
        if (trie[v].next.find(char_index) == trie[v].next.end()) {
            trie.push_back(create_new_node(v, char_index));
            trie[v].next[char_index] = int(trie.size())-1;
        }
        v = trie[v].next[char_index];
    }

    trie[v].is_terminal = true;
}

int go(int v, char c);

int get_link(int v) {
    if (trie[v].suffix_link == -1) {
        trie[v].suffix_link = (v == 0 || trie[v].parent_index == 0)
                              ? 0
                              : go(get_link(trie[v].parent_index), trie[v].parent_char);
    }
    return trie[v].suffix_link;
}

int go(int v, char c) {
    if (trie[v].go.find(c) == trie[v].go.end()) {// no known transition for the current state
        if (trie[v].next.find(c) != trie[v].next.end())// symbol link is available
            trie[v].go[c] = trie[v].next[c];
        else // symbol link is unavailable: (from root go to root) or (go to suffix link)
            trie[v].go[c] = v == 0 ? 0 : go(get_link(v), c);
    }
    return trie[v].go[c];
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    trie.push_back(create_new_node(0, 0)); //pushing root node

    int n;
    std::cin >> n;
    std::string cur_pattern;

    for (int i = 0; i < n; i++) {
        std::cin >> cur_pattern;
        add_pattern_to_trie(cur_pattern);
    }

    std::queue<int> nodes_to_process;
    nodes_to_process.push(0);
    int cur_node_index, parent_node_index, cur_suffix_link;

    // level-by-level processing of the tree
    while (!nodes_to_process.empty()) {
        cur_node_index = nodes_to_process.front();
        trie[cur_node_index].suffix_link = get_link(cur_node_index);
        nodes_to_process.pop();

        auto it = trie[cur_node_index].next.begin();
        while (it != trie[cur_node_index].next.end())
        {
            nodes_to_process.push(it->second);
            it++;
        }

        parent_node_index = trie[cur_node_index].parent_index;
        cur_suffix_link = trie[cur_node_index].suffix_link;

        trie[cur_node_index].local_max_len = trie[cur_node_index].is_terminal +
                                             std::max(trie[parent_node_index].local_max_len,
                                                      trie[cur_suffix_link].local_max_len);

        if (trie[cur_node_index].local_max_len > global_max)
            global_max = trie[cur_node_index].local_max_len;
    }

    std::cout << global_max;

    return 0;
}