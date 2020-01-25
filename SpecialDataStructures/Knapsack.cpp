#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <bitset>
#include <math.h>
using namespace std;

int things_amount;
long long max_weight;

struct Node{
    long long cost;
    long long weight;
    bitset<20> mask;
    long long best;

    bool operator < (const Node & another_node)
    {
        return weight < another_node.weight;
    }
};

bool operator < (const long long int left, const Node & node) {
    return left < node.weight;
}

string get_correct_substr(Node const & node, int const & size){
    string string_mask = node.mask.to_string();
    reverse(string_mask.begin(), string_mask.end());
    return string_mask.substr(0, size);
}

string get_all_subsets(vector<Node> const &things){
    const uint64_t fn = things_amount / 2;
    long long first_part_subsets_size = pow(2, fn);
    vector<Node> first_part_subsets(first_part_subsets_size);

    Node first_part_best_candidate;
    long long first_part_best_cost = 0;
    for (long long i = 0; i < first_part_subsets_size; i++){
        auto mask = bitset<20>(i);
        for (int j = 0; j < fn; j++){
            if (mask.test(j)){
                first_part_subsets[i].weight+=things[j].weight;
                first_part_subsets[i].cost+=things[j].cost;
            }
        }

        first_part_subsets[i].mask = mask;

        if (first_part_subsets[i].weight <= max_weight and first_part_subsets[i].cost > first_part_best_cost){
            first_part_best_candidate = first_part_subsets[i];
            first_part_best_cost = first_part_best_candidate.cost;
        }
    }

    sort(first_part_subsets.begin(), first_part_subsets.end());

    long long current_first_part_best_index = 0;
    for (int i = 0; i < first_part_subsets_size; i++){
        if (first_part_subsets[i].weight > max_weight)
            break;

        if (first_part_subsets[i].cost > first_part_subsets[current_first_part_best_index].cost){
            current_first_part_best_index = i;
        }

        first_part_subsets[i].best = current_first_part_best_index;
    }

    const int sn = things_amount - fn;
    long long second_part_subsets_size = pow(2, sn);
    vector<Node> second_part_subsets(second_part_subsets_size);
    Node second_part_best_candidate;
    pair<Node, Node> best_combined_candidates;
    long long combined_best_cost = 0;
    long long second_part_best_cost = 0;

    for (long long i = 0; i < second_part_subsets_size; i++){
        auto mask = bitset<20>(i);
        for (int j = 0; j < sn; j++){
            if (mask.test(j)){
                second_part_subsets[i].weight+=things[fn+j].weight;
                second_part_subsets[i].cost+=things[fn+j].cost;
            }
        }

        second_part_subsets[i].mask = mask;

        long long available_weight = max_weight - second_part_subsets[i].weight;

        if (available_weight > 0){
            auto first_part_iterator = upper_bound(first_part_subsets.begin(), first_part_subsets.end(), available_weight);

            if (first_part_iterator != first_part_subsets.begin()) {
                first_part_iterator--;

                Node first_part_node = first_part_subsets[first_part_iterator->best];
                if ((first_part_node.cost + second_part_subsets[i].cost) > combined_best_cost) {
                    best_combined_candidates = pair(first_part_node, second_part_subsets[i]);
                    combined_best_cost = first_part_node.cost + second_part_subsets[i].cost;
                }
            }
        }

        if (available_weight >= 0 and second_part_subsets[i].cost > second_part_best_cost){
            second_part_best_candidate = second_part_subsets[i];
            second_part_best_cost = second_part_best_candidate.cost;
        }
    }

    if (first_part_best_cost > second_part_best_cost) {
        if (combined_best_cost > first_part_best_cost){
            string first_sub_string = get_correct_substr(best_combined_candidates.first, fn);
            string second_sub_string = get_correct_substr(best_combined_candidates.second, sn);

            reverse(second_sub_string.begin(), second_sub_string.end());
            reverse(first_sub_string.begin(), first_sub_string.end());
            return second_sub_string + first_sub_string;
        }
        else {
            string only_first_best = get_correct_substr(first_part_best_candidate, fn);

            reverse(only_first_best.begin(), only_first_best.end());
            return string(sn, '0') + only_first_best;
        }
    }
    else {
        if (combined_best_cost > second_part_best_cost){
            string first_sub_string = get_correct_substr(best_combined_candidates.first, fn);

            string second_sub_string = get_correct_substr(best_combined_candidates.second, sn);

            reverse(second_sub_string.begin(), second_sub_string.end());
            reverse(first_sub_string.begin(), first_sub_string.end());
            return second_sub_string + first_sub_string;
        }
        else {
            string only_second_best = get_correct_substr(second_part_best_candidate, sn);

            reverse(only_second_best.begin(), only_second_best.end());
            return only_second_best + string(fn, '0');
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> things_amount;
    cin >> max_weight;

    vector<Node> things(things_amount);
    for(int i = 0; i < things_amount; i++){
        cin >> things[i].weight;
        cin >> things[i].cost;
    }

    string result = get_all_subsets(things);
    bitset<40> answer(result);
    cout << answer.count() << '\n';
    for (int i = 0; i < answer.size(); i++){
        if (answer.test(i)){
            cout << i + 1 << ' ';
        }
    }

    return 0;
}