#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
// #include <climits>
#include <iostream>
#include <map>
//#include <chrono>
#include <set>
#include <unordered_set>


using namespace std;
struct CompareSecond {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
        return a.second < b.second;
    }
};
//auto startTime = chrono::high_resolution_clock::now();
//auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
struct nodo{
  unordered_set<int> adj;
  int id_nodo;
  bool visitato=false;
};
vector<nodo> grafo;
vector<nodo> grafo2;

//globall
map<int, int> resu;
int grafo_count = 0;
map<int, map<int, int>> grafi;
map<int, map<int, int>> grafi2;
set<string> delete_base;
set<string> created;
set<string> created2;
set<string> destroyed;
set<string> destroyed2;
int best_move=0;
int mucca=0;
int mucca2=0;

void connetti_stacca_grafi(vector<nodo>& grafom, map<int, map<int, int>>& grafim, set<string>& created_m, set<string>& destroyed_m, int& muccam){
  int a = grafim.size();
  for (const auto& coppia : grafim) {
    
    unordered_set <int> nodi_restanti;
    int n_n = grafim[coppia.first].size()-1;
    vector<pair<int, int>> pairs(coppia.second.begin(), coppia.second.end());

    sort(pairs.begin(), pairs.end(), CompareSecond());
    while (!pairs.empty() && pairs[0].second < n_n/2) {
      unordered_set<int> lista_adiacenti_copia= grafom[pairs[0].first].adj;
      for (const auto& to_del : lista_adiacenti_copia) {
        grafom[pairs[0].first].adj.erase(to_del);
        grafom[to_del].adj.erase(pairs[0].first);
        destroyed_m.insert("- "+to_string(pairs[0].first) + ' ' + to_string(to_del));
        muccam++;
      }
      pairs.erase(pairs.begin());
      sort(pairs.begin(), pairs.end(), CompareSecond());
      n_n--;
    }
    for (const auto& pair : pairs) {
        nodi_restanti.insert(pair.first);
    }
    for (const auto& e1 : nodi_restanti) {
      for (const auto& e2 : nodi_restanti) {
        if (e1 != e2 && grafom[e1].adj.find(e2) == grafom[e1].adj.end()) {
          grafom[e1].adj.insert(e2);
          grafom[e2].adj.insert(e1);
          size_t n_rimossi = destroyed_m.erase("- "+to_string(e2)+" "+to_string(e1));
          size_t n_rimossi2 = destroyed_m.erase("- "+to_string(e1)+" "+to_string(e2));
          if (n_rimossi == 0 && n_rimossi2 == 0) {
            created_m.insert("+ "+to_string(e2)+" "+to_string(e1));
            muccam++;
          }
        }
      }
    }
  }
}

void calcola_grafi(vector<nodo>& grafo_m, map<int, map<int, int>>& grafi_m){
  stack<int> coda;
  grafo_count = 0;
  for (nodo& x: grafo_m) {
    //se non visitato aggiungo gli adiacenti alla coda e aumento il grafo count di 1 che mi serve per inserire l'id
    if (!x.visitato){
      grafo_count++;
      grafi_m[grafo_count][x.id_nodo] = x.adj.size();
      x.visitato = true;
      for(int v: x.adj){
        coda.push(v);
      }
    }
    //ciclo tutti gli adiacenti per salvarmi il grafo connesso! e settargli l'id
    while (!coda.empty()){
      int a = coda.top();
      coda.pop();
      nodo& n = grafo_m[a];
      if (n.visitato){
        continue;
      }
      n.visitato = true;
      grafi_m[grafo_count][n.id_nodo] = n.adj.size();
      for(int v: n.adj){
        coda.push(v);
      }
    }
  }
}

void programmino(string inf, string outf, int id)
{
  ifstream in(inf);
  ofstream out(outf);
  grafo_count = 0;
  grafi.clear();
  grafo.clear();
  destroyed.clear();
  created.clear();
  int N,M;
  in >> N >> M;
  int vittoria;

  best_move=0;
  mucca=0;
  
  delete_base.clear();
  
  grafo.resize(N);

  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "before read: " << duration.count() <<  "\n";
  //Lettura del grafo
  int f,t;
  for(int i=0;i<M;i++){
      in>>f>>t;
      delete_base.insert("- " + to_string(f) + " " + to_string(t));
      best_move++;
      grafo[f].adj.insert(t);
      grafo[f].id_nodo=f;
      grafo[t].adj.insert(f);
      grafo[t].id_nodo=t;
  }
  out << "0 " << best_move << "\n";
  for (string i: delete_base) {
    out << i << "\n";
  }
  out << "***"<< "\n";
  
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "before cleanup: " << duration.count() <<  "\n";
  for (nodo& x: grafo2) {
    unordered_set<int> lista_adiacenti_copia= x.adj;
    for (int n_id: lista_adiacenti_copia) {
      float half_len = static_cast<float>(x.adj.size() - 1) * 0.9;
      int non_trovati = 0;
      for (int i: x.adj) {
        if (i!=n_id && grafo2[n_id].adj.find(i) == grafo2[n_id].adj.end()) {
          non_trovati++;
          if (non_trovati > half_len) {
            break;
          }
        }
      }
      if (non_trovati > half_len) {
        x.adj.erase(n_id);
        grafo2[n_id].adj.erase(x.id_nodo);
        destroyed2.insert("- "+to_string(n_id) + ' ' + to_string(x.id_nodo));
        mucca2++;
      } 
    }
  }
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "after cleanup: " << duration.count() <<  "\n";
  calcola_grafi(grafo2, grafi2);
  connetti_stacca_grafi(grafo2, grafi2, created2, destroyed2, mucca2);
  if (best_move > mucca2){
    out << created2.size() << " " << destroyed2.size() << "\n";
    for (string i: created2){
      out << i << "\n";
    }
    for (string i: destroyed2){
      out << i << "\n";
    }
    out << "***" << "\n";
  }
  for (nodo& x: grafo) {
    unordered_set<int> lista_adiacenti_copia= x.adj;
    for (int n_id: lista_adiacenti_copia) {
      float half_len = static_cast<float>(x.adj.size() - 1) * 0.5;
      int non_trovati = 0;
      for (int i: x.adj) {
        if (i!=n_id && grafo[n_id].adj.find(i) == grafo[n_id].adj.end()) {
          non_trovati++;
          if (non_trovati > half_len) {
            break;
          }
        }
      }
      if (non_trovati > half_len) {
        x.adj.erase(n_id);
        grafo[n_id].adj.erase(x.id_nodo);
        destroyed.insert("- "+to_string(n_id) + ' ' + to_string(x.id_nodo));
        mucca++;
      } 
    }
  }
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "after grafo id: " << duration.count() <<  "\n";
  calcola_grafi(grafo, grafi);
  connetti_stacca_grafi(grafo, grafi, created, destroyed, mucca);
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "finale: " << duration.count() <<  "\n";
  if (mucca2 > mucca) {
    out << created.size() << " " << destroyed.size() << "\n";
    for (string i: created){
      out << i << "\n";
    }
    for (string i: destroyed){
      out << i << "\n";
    }
    out << "***" << "\n";
  } 
  
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "fine stampa: " << duration.count() <<  "\n";
}


int main() {
  // for (int i = 0; i <= 15; i++) {
  //   string inf = "input/input"+ to_string(i)+".txt";
  //   string outf = "output/output"+ to_string(i)+".txt";
  //   programmino(inf, outf, i);
  //   duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //   cout << inf << "   " << duration.count() <<  "\n";
  // }
  // string outf2 = "final_new.txt";
  // ofstream out2(outf2);
  // for (const auto& pair : resu) {
  //     out2 << pair.first << ": " << pair.second << "\n";
  // }
  programmino("input.txt", "output.txt", 1);
}