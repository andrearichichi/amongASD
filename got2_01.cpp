#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
// #include <climits>
#include <utility>
#include <iostream>
#include <map>
#include <chrono>
#include <set>
#include <unordered_set>
// #include "got2.h"

using namespace std;

auto startTime = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
struct nodo{
  unordered_set<nodo*> adj;
  int id_nodo;
  bool visitato=false;
  bool grafed=true;
};

vector<nodo> grafo;
vector<nodo*> grafo_p;
vector<nodo*> grafo2_p;
vector<nodo> grafo2;

struct CompareNodoPtr {
  bool operator()(const nodo* a, const nodo* b) const {
    if (a->adj.size() == b->adj.size()) {
      return a < b;  // Confronto i puntatori per mantenere tutti gli elementi
    } else {
      return a->adj.size() < b->adj.size();
    }
  }
};

struct CompareNodoPtrDesc {
  bool operator()(const nodo* a, const nodo* b) const {
     if (a->grafed && !b->grafed) {
            return true;  // Ordina a prima di b se a->grafed è true e b->grafed è false
        } else if (!a->grafed && b->grafed) {
            return false;  // Ordina b prima di a se a->grafed è false e b->grafed è true
        } else if (a->adj.size() == b->adj.size()) {
            return a > b;  // Confronto i puntatori per mantenere tutti gli elementi
        } else {
            return a->adj.size() > b->adj.size();
        }
  }
};
//globall
map<int, int> resu;
int grafo_count = 0;
unordered_set<string> archi_solidi;
vector<set<nodo*> > grafi;
vector<set<nodo*> > grafi2;
set<string> created;
set<string> created2;
set<string> destroyed;
set<string> destroyed2;
int mucca=0;
int mosse_effettuate=0;

void connetti_stacca_grafi2(vector<set<nodo*> > grafim, set<string>& created_m, set<string>& destroyed_m, int& muccam){
  int a = 0;
  for (set<nodo*>& g : grafim) {
    a++;
    vector<nodo*> vect(g.begin(), g.end());
    bool da_elliminare = true;
    // set<nodo*> mySet_ordered(vect.begin(), vect.end());
    while (da_elliminare) {
      da_elliminare=false;
      sort(vect.begin(), vect.end(), CompareNodoPtr());
      if (vect[0]->adj.size()<(g.size()-1)/2) {
        da_elliminare=true;
        unordered_set<nodo*> miao= vect[0]->adj;
        for (nodo* n:miao) {
          n->adj.erase(vect[0]);
          vect[0]->adj.erase(n);
          destroyed_m.insert("- "+to_string(vect[0]->id_nodo) + ' ' + to_string(n->id_nodo));
          muccam++;
        }
      }
      if (da_elliminare) {
        g.erase(vect[0]);
        vect.erase(vect.begin());
      }
    }
    for (nodo* to_add: g) {
      for (nodo* to_add_remain: g) {
        if(to_add != to_add_remain && to_add->adj.find(to_add_remain) == to_add->adj.end()) {
          to_add->adj.insert(to_add_remain);
          to_add_remain->adj.insert(to_add);
          size_t n_rimossi = destroyed_m.erase("- "+to_string(to_add->id_nodo)+" "+to_string(to_add_remain->id_nodo));
          size_t n_rimossi2 = destroyed_m.erase("- "+to_string(to_add_remain->id_nodo)+" "+to_string(to_add->id_nodo));
          if (n_rimossi == 0 && n_rimossi2 == 0) {
            created_m.insert("+ "+to_string(to_add->id_nodo)+" "+to_string(to_add_remain->id_nodo));
            muccam++;
          } else {
            muccam--;
          }
        }
      }
    }
  }
}

void calcola_grafi(vector<nodo*>& grafo_m, vector<set<nodo*> >& grafi_m){
  stack<int> coda;
  grafo_count = -1;
  for (nodo* x: grafo_m) {
    //se non visitato aggiungo gli adiacenti alla coda e aumento il grafo count di 1 che mi serve per inserire l'id
    if (!x->visitato){
      grafo_count++;
      set<nodo*> nuovoSet;
      grafi_m.push_back(nuovoSet);
      grafi_m[grafo_count].insert(x);
      x->visitato = true;
      for(nodo* v: x->adj){
        coda.push(v->id_nodo);
      }
    }
    //ciclo tutti gli adiacenti per salvarmi il grafo connesso! e settargli l'id
    while (!coda.empty()){
      int a = coda.top();
      coda.pop();
      nodo* n = grafo_m[a];
      if (n->visitato){
        continue;
      }
      n->visitato = true;
      grafi_m[grafo_count].insert(n);
      for(nodo* v: n->adj){
        coda.push(v->id_nodo);
      }
    }
  }
}

void programmino(string inf, string outf, int id)
{
  grafo_p.clear();
  grafo2_p.clear();
  archi_solidi.clear();
  ifstream in(inf);
  ofstream out(outf);
  grafo_count = 0;
  grafi.clear();
  grafi2.clear();
  grafo.clear();
  destroyed2.clear();
  destroyed.clear();
  created2.clear();
  created.clear();
  int N,M;
  in >> N >> M;
  mucca=0; 
  mosse_effettuate=0; 
  grafo.resize(N);

  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "before read: " << duration.count() <<  "\n";
  //Lettura del grafo
  int f,t;
  for(int i=0;i<M;i++){
    in>>f>>t;
    grafo[f].id_nodo=f;
    grafo[t].id_nodo=t;
    grafo[f].adj.insert(&grafo[t]);
    grafo[t].adj.insert(&grafo[f]);
  }
  
  for (nodo& n : grafo) {
      grafo2_p.push_back(&n);
      grafo_p.push_back(&n);
  }
  
  for (nodo* x: grafo2_p) {
    unordered_set<nodo*> lista_adiacenti_copia= x->adj;
    for (nodo* n: lista_adiacenti_copia) {
      float half_len = static_cast<float>(x->adj.size() - 1) * rap;
      int non_trovati = 0;
      for (nodo* i: x->adj) {
        if (i!=n && n->adj.find(i) == n->adj.end()) {
          non_trovati++;
          if (non_trovati > half_len) {
            break;
          }
        }
      }
      if (non_trovati > half_len) {
        x->adj.erase(n);
        n->adj.erase(x);
        destroyed2.insert("- "+to_string(n->id_nodo) + ' ' + to_string(x->id_nodo));
        mosse_effettuate++;
      } 
    }
  }
  while (true) {
    sort(grafo2_p.begin(), grafo2_p.end(), CompareNodoPtrDesc());
    if (grafo2_p[0]->grafed == false) {
      break;
    }
    for (nodo* x: grafo2_p[0]->adj) {
      unordered_set<nodo*> lista_adiacenti_copia= x->adj;
      for (nodo* y: lista_adiacenti_copia) {
        if (y!=grafo2_p[0] && grafo2_p[0]->adj.find(y) == grafo2_p[0]->adj.end()) {
          x->adj.erase(y);
          y->adj.erase(x);
          destroyed2.insert("- "+to_string(x->id_nodo) + ' ' + to_string(y->id_nodo));
          mosse_effettuate++;
        }
      }
      x->grafed=false;
    }
    grafo2_p[0]->grafed=false;
  }
  calcola_grafi(grafo2_p, grafi2);
  connetti_stacca_grafi2(grafi2, created2, destroyed2, mosse_effettuate);
  out << created2.size() << " " << destroyed2.size() << "\n";
  for (string i: created2){
    out << i << "\n";
  }
  for (string i: destroyed2){
    out << i << "\n";
  }
  out << "***" << "\n";
  resu[id] = mosse_effettuate;

  // //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  // //cout << "before cleanup: " << duration.count() <<  "\n";
  float rap = 0.5;
  // da migliorare con un ordered set di SOLIDI ARCHI
  for (nodo* x: grafo_p) {
    unordered_set<nodo*> lista_adiacenti_copia= x->adj;
    for (nodo* n: lista_adiacenti_copia) {
      float half_len = static_cast<float>(x->adj.size() - 1) * rap;
      int non_trovati = 0;
      for (nodo* i: x->adj) {
        if (i!=n && n->adj.find(i) == n->adj.end()) {
          non_trovati++;
          if (non_trovati > half_len) {
            break;
          }
        }
      }
      if (non_trovati > half_len) {
        x->adj.erase(n);
        n->adj.erase(x);
        destroyed.insert("- "+to_string(n->id_nodo) + ' ' + to_string(x->id_nodo));
        mucca++;
      } 
    }
  }

  calcola_grafi(grafo_p, grafi);
  connetti_stacca_grafi2(grafi, created, destroyed, mucca);
  if (mucca < mosse_effettuate) {
    ofstream outonee(outf);
    outonee << created.size() << " " << destroyed.size() << "\n";
    for (string i: created){
      outonee << i << "\n";
    }
    for (string i: destroyed){
      outonee << i << "\n";
    }
    outonee << "***" << "\n";
    resu[id] = mucca;
  }
}


int main() {
  for (int i = 14; i <= 19; i++) {
    string inf = "input/input"+ to_string(i)+".txt";
    string outf = "output/output"+ to_string(i)+".txt";
    programmino(inf, outf, i);
    duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
    cout << inf << "   " << duration.count() <<  "\n";
  }
  string outf2 = "final_new.txt";
  ofstream out2(outf2);
  for (const auto& pair : resu) {
      out2 << pair.first << ": " << pair.second << "\n";
  }
  // programmino("input.txt", "output.txt", 1);
}