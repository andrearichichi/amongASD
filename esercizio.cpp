#include <fstream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;


struct arco{
  int nodo_partenza;
  int nodo;
  int peso;
  int peso_max;
  arco(){
    nodo_partenza=-1;
    peso_max=-1;
    peso=0;
  }
};

struct informazione{
  int costo_totale;
  vector<int> stanze_passate;
  vector<arco> archi_speciali_passati;
};
struct nodo{
  vector<arco> adj;
  int nodo_entrante;
  int costo;
  nodo(){
    costo=INT_MAX;
    nodo_entrante=-1;
  }
};

int fablab;

void calcola_grafo(vector<nodo> grafo_da_elaborare, arco partenza, informazione &info){
  //st lista di archi con peso e ct=peso totale raggiunto fin la
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> coda;
  coda.emplace(0, partenza.nodo);

  while (!coda.empty()) {
    int nodo = coda.top().second;
    int costo = coda.top().first;
    coda.pop();

    for(arco v:grafo_da_elaborare[nodo].adj){
      if (grafo_da_elaborare[v.nodo].costo > v.peso+costo)
      {
        grafo_da_elaborare[v.nodo].costo = v.peso+costo;
        grafo_da_elaborare[v.nodo].nodo_entrante = nodo;
        coda.emplace(grafo_da_elaborare[v.nodo].costo, v.nodo);       
      }
    }
  }

  info.stanze_passate.push_back(fablab);
  nodo n=grafo_da_elaborare[fablab];
  while(n.nodo_entrante!=-1 && n.nodo_entrante!=0){
    info.stanze_passate.push_back(n.nodo_entrante);
    n=grafo_da_elaborare[n.nodo_entrante];
  }
}
int main(void)
{
  
  ifstream in("input.txt");
  ofstream out("output.txt");
  int N,M,S;
  in >> N >> M >> S;
  int vittoria;
  vector<int> stanze_imp;

  arco impostore, studente;
  in >> impostore.nodo >> studente.nodo >> fablab;
  informazione info_stud;
  informazione info_impostore;
  vector<nodo> grafo;
  vector<arco> archi;
  vector<arco> archi_speciali;
  grafo.resize(N);
  //Lettura del grafo
  int f,t,j,k;
  for(int i=0;i<M;i++){
    arco s1;
    in>>f>>t>>j;
    s1.nodo_partenza = f;
    s1.nodo = t;
    s1.peso = j;
    archi.push_back(s1);
    grafo[f].adj.push_back(s1);
  }

  for(int i=0;i<S;i++){
    arco s1;
    in>>f>>t>>j>>k;
    s1.nodo_partenza = f;
    s1.nodo = t;
    s1.peso = j;
    s1.peso_max=k;
    archi_speciali.push_back(s1);
    grafo[f].adj.push_back(s1);
  }

  calcola_grafo(grafo, studente, info_stud);
  calcola_grafo(grafo, impostore, info_impostore);

  vittoria = info_impostore.costo_totale < info_stud.costo_totale ? 1 : info_impostore.costo_totale > info_stud.costo_totale ? 2 : 0;
  // nodo n;
  // stanze_imp.push_back(fablab);
  // n=grafo_impostore[fablab];
  // while(n.nodo_entrante!=-1 && n.nodo_entrante!=0){
  //   stanze_imp.push_back(n.nodo_entrante);
  //   n=grafo_impostore[n.nodo_entrante];
  // }

  out<<vittoria<<endl;
  out<<info_impostore.costo_totale << " " << info_stud.costo_totale<<endl;
  // //numeri ventole
  out << info_impostore.stanze_passate.size()<<endl;
  while (info_impostore.stanze_passate.size()>0){
    out<< info_impostore.stanze_passate.back()<< " ";
    info_impostore.stanze_passate.pop_back();
  }

  return 0;
}

