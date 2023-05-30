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
#include <set>
#include <unordered_set>
#include "got2.h"
using namespace std;


struct nodo{
  unordered_set<nodo*> adj;
  int id_nodo;
  bool visitato=false;
};

vector<nodo> grafo;
vector<nodo*> grafo_p;


//globall
map<int, int> resu;
int grafo_count = 0;
unordered_set<string> archi_solidi;
vector<set<nodo*> > grafi;
map<int, set<nodo*> > grafi2;
set<string> created;
set<string> created2;
set<string> destroyed;
set<string> destroyed2;
int mucca=0;
int mucca2=0;

void connetti_stacca_grafi(vector<set<nodo*> > grafim, set<string>& created_m, set<string>& destroyed_m, int& muccam){
  for (set<nodo*>& g : grafim) {
      for (nodo* elemento : g) {
        for (nodo* elemento2 : g) {
          if (elemento != elemento2 && elemento->adj.find(elemento2) == elemento->adj.end()) {
            elemento->adj.insert(elemento2);
            elemento2->adj.insert(elemento);
            size_t n_rimossi = destroyed.erase("- "+to_string(elemento2->id_nodo)+" "+to_string(elemento->id_nodo));
            size_t n_rimossi2 = destroyed.erase("- "+to_string(elemento->id_nodo)+" "+to_string(elemento2->id_nodo));
            if (n_rimossi == 0 && n_rimossi2 == 0) {
              created.insert("+ "+to_string(elemento2->id_nodo)+" "+to_string(elemento->id_nodo));
              mucca++;
            } else {
              mucca--;
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
  archi_solidi.clear();
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
  mucca=0; 
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
      grafo_p.push_back(&n);
  }
  // //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  // //cout << "before cleanup: " << duration.count() <<  "\n";
  float rap = 0.5;
  // da migliorare con un ordered set di SOLIDI ARCHI
  // sort(grafo_p.begin(), grafo_p.end(), CompareNodoPtr());
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
  connetti_stacca_grafi(grafi, created, destroyed, mucca);

  out << created.size() << " " << destroyed.size() << "\n";
  for (string i: created){
    out << i << "\n";
  }
  for (string i: destroyed){
    out << i << "\n";
  }
  out << "***" << "\n";
}


int main() {
  // for (int i = 0; i <= 19; i++) {
  //   string inf = "input/input"+ to_string(i)+".txt";
  //   string outf = "output/output"+ to_string(i)+".txt";
  //   programmino(inf, outf, i);
  //   cout << i << endl;
  // }
  programmino("input.txt", "output.txt", 1);
}