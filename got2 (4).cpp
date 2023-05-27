#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
// #include <climits>
#include <iostream>
#include <map>
#include <chrono>
#include <set>
#include <unordered_set>


using namespace std;

auto startTime = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
struct nodo{
  unordered_set<int> adj;
  int id_nodo;
  bool visitato;
  nodo(){
    visitato=false;
    id_nodo=-1;
  }
};

//globall
int grafo_count = 0;
map<int, unordered_set<int>> grafi;
vector<nodo> grafo;
vector<string> delete_base;
set<string> created;
set<string> destroyed;
int best_move=0;
int mucca=0;

void connetti_stacca_grafi(){
  for (const auto& coppia : grafi) {
    for (const int& elemento : coppia.second) {
      for (const int& elemento2 : coppia.second) {
        if (elemento != elemento2 && grafo[elemento].adj.find(elemento2) == grafo[elemento].adj.end()) {
          grafo[elemento].adj.insert(elemento2);
          grafo[elemento2].adj.insert(elemento);
          size_t n_rimossi = destroyed.erase("- "+to_string(elemento2)+" "+to_string(elemento));
          size_t n_rimossi2 = destroyed.erase("- "+to_string(elemento)+" "+to_string(elemento2));
          if (n_rimossi == 0 && n_rimossi2 == 0) {
            created.insert("+ "+to_string(elemento2)+" "+to_string(elemento));
            mucca++;
          }
        }
      }
    }

  }
}

void calcola_grafi(){
  stack<int> coda;
  grafo_count = 0;
  for (nodo& x: grafo) {
    //se non visitato aggiungo gli adiacenti alla coda e aumento il grafo count di 1 che mi serve per inserire l'id
    if (!x.visitato){
      grafo_count++;
      grafi[grafo_count].insert(x.id_nodo);
      x.visitato = true;
      for(int v: x.adj){
        coda.push(v);
      }
    }
    //ciclo tutti gli adiacenti per salvarmi il grafo connesso! e settargli l'id
    while (!coda.empty()){
      int a = coda.top();
      coda.pop();
      nodo& n = grafo[a];
      if (n.visitato){
        continue;
      }
      n.visitato = true;
      grafi[grafo_count].insert(n.id_nodo);
      for(int v: n.adj){
        coda.push(v);
      }
    }
  }
}

void programmino(string inf, string outf)
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
  //cout << "before read: " << duration.count() <<  endl;
  //Lettura del grafo
  int f,t;
  for(int i=0;i<M;i++){
      in>>f>>t;
      delete_base.push_back("- " + to_string(f) + " " + to_string(t));
      best_move++;
      grafo[f].adj.insert(t);
      grafo[f].id_nodo=f;
      grafo[t].adj.insert(f);
      grafo[t].id_nodo=t;
  }
  if (best_move < 200000) {
    out << "0 " << best_move << endl;
    for (string i: delete_base) {
      out << i << endl;
    }
    out << "***" << endl;
  }
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "before cleanup: " << duration.count() <<  endl;
  for (nodo& x: grafo) {
    unordered_set<int> lista_adiacenti_copia= x.adj;
    for (int n_id: lista_adiacenti_copia) {
      int half_len = (x.adj.size()-1)/2;
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
  //cout << "after cleanup: " << duration.count() <<  endl;
  calcola_grafi();
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "after grafo id: " << duration.count() <<  endl;
  connetti_stacca_grafi();
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "finale: " << duration.count() <<  endl;
  if (best_move > mucca) {
    out << created.size() << " " << destroyed.size() << endl;
    for (string i: created){
      out << i << endl;
    }
    for (string i: destroyed){
      out << i << endl;
    }
    out << "***" << endl;
  }
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "fine stampa: " << duration.count() <<  endl;
}


int main() {
  // for (int i = 0; i <= 19; i++) {
  //   string inf = "input/input"+ to_string(i)+".txt";
  //   string outf = "output/output"+ to_string(i)+".txt";
  //   duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //   cout << inf << "   " << duration.count() <<  endl;
  //   programmino(inf, outf);
  // }
  programmino("input.txt", "output.txt");
}