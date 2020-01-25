#include <iostream>
#include <vector>
#define ll unsigned long long
using namespace std;

const ll max_n = 100002;
vector<ll> g[max_n];
bool used[max_n];
ll path[2*max_n];

void dfs(ll v, ll &path_index){
    used[v]=true;
    path[path_index++]=v;
    for (auto i : g[v]){
        if (!used[i]) {
            dfs(i, path_index);
            path[path_index++]=v;
        }
    }
}

int main() {
    ll n;
    cin >> n;

    ll edge_l = 0;
    ll edge_r = 0;
    while(cin >> edge_l && cin >> edge_r){
        g[edge_l].push_back(edge_r);
        g[edge_r].push_back(edge_l);
    }

    ll start = 0;
    dfs(1, start);
    for (ll i = 0; i < 2*n-1; i++){
        cout << path[i] << " ";
    }

    return 0;
}