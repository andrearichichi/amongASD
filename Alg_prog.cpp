#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <utility>

using namespace std;

struct Node {
    set<int> adj;
    int graph_id = -1;
    int node_id = -1;
    bool visited = false;
};

int graph_count = 0;
map<int, set<int>> graphs;
vector<Node> graph;
vector<pair<int,int>> to_delete;
set<pair<int,int>> created;
set<pair<int,int>> destroyed;
int best_move=0;

void connect_disconnect_graphs() {
  for (const auto& pair : graphs) {
    for (const int& element : pair.second) {
      set<int> difference;
      for (const auto& neighbor: pair.second) {
          if (!graph[element].adj.count(neighbor)) {
              difference.insert(neighbor);
          }
      }
      difference.erase(element);
      for (int n: difference) {
        graph[element].adj.insert(n);
        graph[n].adj.insert(element);
        size_t n_removed = destroyed.erase({n, element});
        size_t n_removed2 = destroyed.erase({element, n});
        if (n_removed == 0 && n_removed2 == 0) {
          created.insert({n, element});
          best_move++;
        }
      }
    }
  }
}

void calculate_graphs() {
  stack<int> stack;
  graph_count = 0;
  for (Node& x: graph) {
    if (!x.visited){
      graph_count++;
      graphs[graph_count].insert(x.node_id);
      x.visited = true;
      x.graph_id = graph_count;
      for(int v: x.adj){
        stack.push(v);
      }
    }
    while (!stack.empty()){
      int a = stack.top();
      stack.pop();
      Node& n = graph[a];
      if (n.visited){
        continue;
      }
      n.visited = true;
      graphs[graph_count].insert(n.node_id);
      n.graph_id = graph_count;
      for(int v: n.adj){
        stack.push(v);
      }
    }
  }
}


void programmino(string inf, string outf){
  ifstream in(inf);
  ofstream out(outf);
  int N,M;
  in >> N >> M;
  graph.resize(N);
  int f,t;
  for(int i=0;i<M;i++){
      in>>f>>t;
      to_delete.push_back({f,t});
      graph[f].adj.insert(t);
      graph[f].node_id=f;
      graph[t].adj.insert(f);
      graph[t].node_id=t;
  }
  
  for (Node& x: graph) {
    for (auto it = x.adj.begin(); it != x.adj.end(); ) {
      int n_id = *it;
      int missing = count_if(x.adj.begin(), x.adj.end(), [&](int adj) {
        return graph[n_id].adj.find(adj) == graph[n_id].adj.end();
      });
      int n_adj = x.adj.size() - 1;
      if (missing > 0 && n_adj/missing < 2) {
        graph[n_id].adj.erase(x.node_id);
        destroyed.insert({n_id, x.node_id});
        best_move++;
        it = x.adj.erase(it);
      } else {
        ++it;
      }
    }
  }
  calculate_graphs();
  connect_disconnect_graphs();
  
  out << created.size() << " " << destroyed.size() << "\n";
  for (const auto& i: created){
    out << "+ " << i.first << " " << i.second << "\n";
  }
  for (const auto& i: destroyed){
    out << "- " << i.first << " " << i.second << "\n";
  }
  out << "***" << "\n";
}



int main() {
  for (int i = 0; i <= 19; i++) {
    string inf = "input/input"+ to_string(i)+".txt";
    string outf = "output/output"+ to_string(i)+".txt";
    
    programmino(inf, outf);
  }
}