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


using namespace std;



auto startTime = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
struct nodo{
  unordered_set<nodo*> adj;
  int id_nodo;
  bool visitato=false;
};

vector<nodo> grafo;
vector<nodo*> grafo_p;
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
    if (a->adj.size() == b->adj.size()) {
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
map<int, set<nodo*> > grafi2;
set<string> created;
set<string> created2;
set<string> destroyed;
set<string> destroyed2;
int mucca=0;
int mucca2=0;

void connetti_stacca_grafi(vector<set<nodo*> > grafim, set<string>& created_m, set<string>& destroyed_m, int& muccam){
  int a = 0;
  for (set<nodo*>& g : grafim) {
    cout << a << endl;
    a++;
    vector<nodo*> vect(g.begin(), g.end());
    sort(vect.begin(), vect.end(), CompareNodoPtr());
    set<nodo*> mySet_ordered(vect.begin(), vect.end());

    bool da_elliminare = true;
    nodo* nodo_da_elliminarea = nullptr;
    while (da_elliminare) {
      int n_n = mySet_ordered.size()-1;
      if (nodo_da_elliminarea != nullptr) {
        unordered_set<nodo*> adjSet = nodo_da_elliminarea->adj;
        cout  << nodo_da_elliminarea->id_nodo <<" "<< g.size()<< " " <<  mySet_ordered.size()<< endl;
        g.erase(nodo_da_elliminarea);
        mySet_ordered.erase(nodo_da_elliminarea);
        for (nodo* n_to_del: adjSet) {
          nodo_da_elliminarea->adj.erase(n_to_del);
          n_to_del->adj.erase(nodo_da_elliminarea);
          destroyed_m.insert("- "+to_string(nodo_da_elliminarea->id_nodo) + ' ' + to_string(n_to_del->id_nodo));
          muccam++;
        }
        nodo_da_elliminarea = nullptr;
      }
      da_elliminare = false;
      for (nodo* to_del : mySet_ordered) {
        if ( to_del->adj.size() < n_n/2) {
          nodo_da_elliminarea = to_del;
          da_elliminare = true;
          break;
        }
      }
    }
    //coolllega il resto
    for (nodo* to_add: mySet_ordered) {
      for (nodo* to_add_remain: mySet_ordered) {
        if(to_add != to_add_remain && to_add->adj.find(to_add_remain) == to_add->adj.end()) {
          to_add->adj.insert(to_add_remain);
          to_add_remain->adj.insert(to_add);
          size_t n_rimossi = destroyed_m.erase("- "+to_string(to_add->id_nodo)+" "+to_string(to_add_remain->id_nodo));
          size_t n_rimossi2 = destroyed_m.erase("- "+to_string(to_add_remain->id_nodo)+" "+to_string(to_add->id_nodo));
          if (n_rimossi == 0 && n_rimossi2 == 0) {
            created_m.insert("+ "+to_string(to_add->id_nodo)+" "+to_string(to_add_remain->id_nodo));
            muccam++;
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
  sort(grafo_p.begin(), grafo_p.end(), CompareNodoPtr());
  for (nodo* x: grafo_p) {
    // vector<nodo*> vec(x->adj.begin(), x->adj.end());
    // sort(vec.begin(), vec.end(), CompareNodoPtr());
    // x->adj.clear();
    // for (nodo* p : vec) {
    //   x->adj.insert(p);
    // }
    bool da_elliminare = true;
    nodo* nodo_da_elliminare = nullptr;
    while (da_elliminare) {
      if (nodo_da_elliminare != nullptr) {
        x->adj.erase(nodo_da_elliminare);
        nodo_da_elliminare->adj.erase(x);
        destroyed.insert("- "+to_string(nodo_da_elliminare->id_nodo) + ' ' + to_string(x->id_nodo));
        mucca++;
      }
      da_elliminare = false;
      bool esci = false;
      for (nodo* n: x->adj) {
        if (archi_solidi.find(to_string(x->id_nodo)+to_string(n->id_nodo)) != archi_solidi.end()) {
          continue;
        }
        if (esci) {
          break;
        }
        float rapporto = 0;
        if (x->adj.size()>1) {
          rapporto = (n->adj.size()-1)/(x->adj.size() -1);
        } 
        if (rapporto > 0 && (rapporto < rap || rapporto > (1/rap))) {
          nodo_da_elliminare = n;
          da_elliminare = true;
          break;
        }
        int half_len = (max(x->adj.size(), n->adj.size()) - 1) * rap;
        int non_trovati = 0;
        int trovati = 0;
        for (nodo* i: n->adj) {
          if (i!=n && x->adj.find(i) == x->adj.end()) {
            non_trovati++;
            if (n->adj.size() - non_trovati < half_len) {
              nodo_da_elliminare = n;
              da_elliminare = true;
              esci=true;
              break;
            }
          } 
        }
        if (da_elliminare == false) {
          archi_solidi.insert(to_string(n->id_nodo)+to_string(x->id_nodo));
          archi_solidi.insert(to_string(x->id_nodo)+to_string(n->id_nodo));
        }
      }
    }
  }
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "after cleanup: " << duration.count() <<  "\n";
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
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "after grafo id: " << duration.count() <<  "\n";
  // calcola_grafi(grafo, grafi);
  // connetti_stacca_grafi(grafo, grafi, created, destroyed, mucca);
  //duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  //cout << "finale: " << duration.count() <<  "\n";
  // if (mucca2 > mucca) {
  //   out << created.size() << " " << destroyed.size() << "\n";
  //   for (string i: created){
  //     out << i << "\n";
  //   }
  //   for (string i: destroyed){
  //     out << i << "\n";
  //   }
  //   out << "***" << "\n";
  // } 
  
  // duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  // cout << "fine stampa: " << duration.count() <<  "\n";
  resu[id] = mucca;
}


int main() {
  for (int i = 0; i <= 19; i++) {
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