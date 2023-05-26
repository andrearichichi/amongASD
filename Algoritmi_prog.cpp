#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

struct Nodo {
    unordered_set<int> adj;
    int id_grafo = -1;
    int id_nodo = -1;
    bool visitato = false;
};

//global
int grafo_count = 0;
unordered_map<int, unordered_set<int>> grafi;
vector<Nodo> grafo;
vector<pair<int,int>> delete_base;
struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ h2;
    }
};

struct PairEqual {
    bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

// ...

unordered_map<pair<int,int>, bool, PairHash, PairEqual> created;
unordered_map<pair<int,int>, bool, PairHash, PairEqual> destroyed;
int best_move=0;
int mucca=0;

void connetti_stacca_grafi() {
  for (const auto& coppia : grafi) {
    for (const int& elemento : coppia.second) {
      unordered_set<int> differenceSet;
      for (const auto& adj: coppia.second) {
          if (!grafo[elemento].adj.count(adj)) {
              differenceSet.insert(adj);
          }
      }
      differenceSet.erase(elemento);
      for (int n: differenceSet) {
        grafo[elemento].adj.insert(n);
        grafo[n].adj.insert(elemento);
        size_t n_rimossi = destroyed.erase({n, elemento});
        size_t n_rimossi2 = destroyed.erase({elemento, n});
        if (n_rimossi == 0 && n_rimossi2 == 0) {
          created[{n, elemento}] = true;
          mucca++;
        }
      }
    }
  }
}

void calcola_grafi(){
  stack<int> coda;
  grafo_count = 0;
  for (Nodo& x: grafo) {
    if (!x.visitato){
      grafo_count++;
      grafi[grafo_count].insert(x.id_nodo);
      x.visitato = true;
      x.id_grafo = grafo_count;
      for(int v: x.adj){
        coda.push(v);
      }
    }
    while (!coda.empty()){
      int a = coda.top();
      coda.pop();
      Nodo& n = grafo[a];
      if (n.visitato){
        continue;
      }
      n.visitato = true;
      grafi[grafo_count].insert(n.id_nodo);
      n.id_grafo = grafo_count;
      for(int v: n.adj){
        coda.push(v);
      }
    }
  }
}

int main()
{
  string inf = "input.txt";
  string outf = "output.txt";
  ifstream in(inf);
  ofstream out(outf);
  grafo_count = 0;
  grafi.clear();
  grafo.clear();
  destroyed.clear();
  created.clear();
  int N,M;
  in >> N >> M;

  best_move=0;
  mucca=0;
  
  delete_base.clear();
  
  grafo.resize(N);

  int f,t;
  for(int i=0;i<M;i++){
      in>>f>>t;
      delete_base.push_back({f,t});
      best_move++;
      grafo[f].adj.insert(t);
      grafo[f].id_nodo=f;
      grafo[t].adj.insert(f);
      grafo[t].id_nodo=t;
  }
  if (best_move < 200000) {
    out << "0 " << best_move << endl;
    for (auto i: delete_base) {
      out << "- " << i.first << " " << i.second << endl;
    }
    out << "***" << endl;
  }

  for (Nodo& x: grafo) {
    for (auto it = x.adj.begin(); it != x.adj.end(); ) {
      int n_id = *it;
      int mancanti = 0;
      for (const auto& adj: x.adj) {
          if (!grafo[n_id].adj.count(adj)) {
              mancanti++;
          }
      }
      int n_adj = x.adj.size() - 1;
      if (mancanti > 0 && n_adj/mancanti < 2) {
        grafo[n_id].adj.erase(x.id_nodo);
        destroyed[{n_id, x.id_nodo}] = true;
        mucca++;
        it = x.adj.erase(it);
      } else {
        ++it;
      }
    }
  }
  calcola_grafi();
  connetti_stacca_grafi();
  if (best_move > mucca) {
    out << created.size() << " " << destroyed.size() << endl;
    for (auto i: created){
      out << "+ " << i.first.first << " " << i.first.second << endl;
    }
    for (auto i: destroyed){
      out << "- " << i.first.first << " " << i.first.second << endl;
    }
    out << "***" << endl;
  }
}
