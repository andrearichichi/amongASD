#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
// #include <climits>
using namespace std;


struct arco{
  int id;
  int nodo_partenza;
  int nodo;
  int peso;
  int peso_max;
  arco(){
    id=-1;
    nodo_partenza=-1;
    peso_max=-1;
    peso=0;
  }
};

struct informazione{
  int costo_totale;
  vector<int> stanze_passate;
  vector<int> archi_speciali_passati;
  vector<int> archi_speciali_passati_non_maxati;
};

struct nodo{
  vector<arco> adj;
  int arco_di_passaggio;
  int nodo_entrante;
  int costo;
  nodo(){
    costo=999999;
    nodo_entrante=-1;
  }
};

int fablab;
vector<arco> archi_speciali;
arco impostore, studente;
void calcola_grafo(vector<nodo> grafo_da_elaborare, arco partenza, informazione &info){
  //st lista di archi con peso e ct=peso totale raggiunto fin la
  stack<arco> coda;
  grafo_da_elaborare[impostore.nodo].costo=0;
  grafo_da_elaborare[studente.nodo].costo=0;
  for(arco v:grafo_da_elaborare[partenza.nodo].adj){
    if (v.peso_max != -1) {v.peso = archi_speciali[v.id].peso;}

    if (grafo_da_elaborare[v.nodo].costo > v.peso+partenza.peso)
    {
      grafo_da_elaborare[v.nodo].costo = v.peso+partenza.peso;
      coda.push(v);       
    }
    }
  while (!coda.empty()){
    arco a = coda.top();
    coda.pop();
    int nodo = a.nodo;
    int costo = grafo_da_elaborare[nodo].costo;
    if (costo < grafo_da_elaborare[a.nodo_partenza].costo+a.peso){
      continue;
    }
    grafo_da_elaborare[nodo].nodo_entrante = a.nodo_partenza;
    grafo_da_elaborare[nodo].arco_di_passaggio = a.id;
    for(arco v:grafo_da_elaborare[nodo].adj){
      if (v.peso_max != -1) {v.peso = archi_speciali[v.id].peso;}
      if (grafo_da_elaborare[v.nodo].costo > v.peso+costo)
      {
        grafo_da_elaborare[v.nodo].costo = v.peso+costo;
        if (v.nodo_partenza != fablab && v.nodo_partenza != partenza.nodo)
        coda.push(v);       
      }
    }
  }

  info.stanze_passate.push_back(fablab);
  nodo n=grafo_da_elaborare[fablab];
  while(n.nodo_entrante!=-1){
    if (n.arco_di_passaggio != -1){
      arco a = archi_speciali[n.arco_di_passaggio];
      info.archi_speciali_passati.push_back(n.arco_di_passaggio);
      if (a.peso != a.peso_max){
        info.archi_speciali_passati_non_maxati.push_back(n.arco_di_passaggio);
      }
    }
    info.stanze_passate.push_back(n.nodo_entrante);
    if (n.nodo_entrante == fablab || n.nodo_entrante == partenza.nodo){
      break;
    }
    n=grafo_da_elaborare[n.nodo_entrante];
  }
  info.costo_totale = grafo_da_elaborare[fablab].costo;
}
int main(void)
{
  
  ifstream in("input.txt");
  ofstream out("output.txt");
  int N,M,S;
  in >> N >> M >> S;
  int vittoria;

  in >> impostore.nodo >> studente.nodo >> fablab;
  informazione info_stud;
  informazione info_impostore;
  vector<nodo> grafo;
  vector<arco> archi;
  
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

  int count = 0;
  for(int i=0;i<S;i++){
    arco s1;
    in>>f>>t>>j>>k;
    s1.id=count;
    s1.nodo_partenza = f;
    s1.nodo = t;
    s1.peso = j;
    s1.peso_max=k;
    archi_speciali.push_back(s1);
    grafo[f].adj.push_back(s1);
    count++;
  }
  
  int miglior_caso;
  calcola_grafo(grafo, studente, info_stud);
  calcola_grafo(grafo, impostore, info_impostore);
  vittoria = info_impostore.costo_totale < info_stud.costo_totale ? 1 : info_impostore.costo_totale > info_stud.costo_totale ? 2 : 0;
  miglior_caso = vittoria;
  while(vittoria!=1 && !info_stud.archi_speciali_passati_non_maxati.empty()){
    int id_arco = info_stud.archi_speciali_passati_non_maxati.back();
    info_stud.archi_speciali_passati_non_maxati.pop_back();
    archi_speciali[id_arco].peso = archi_speciali[id_arco].peso_max;
    bool b = std::count(info_impostore.archi_speciali_passati_non_maxati.begin(), info_impostore.archi_speciali_passati_non_maxati.end(),id_arco);
    if (!b)
    {
      info_stud={};
      calcola_grafo(grafo, studente, info_stud);
      vittoria = info_impostore.costo_totale < info_stud.costo_totale ? 1 : info_impostore.costo_totale > info_stud.costo_totale ? 2 : 0;
      if (vittoria!=2) miglior_caso = vittoria;
      continue;
    }
    info_stud={};
    calcola_grafo(grafo, studente, info_stud);
    info_impostore={};
    calcola_grafo(grafo, impostore, info_impostore);
    vittoria = info_impostore.costo_totale < info_stud.costo_totale ? 1 : info_impostore.costo_totale > info_stud.costo_totale ? 2 : 0;
    if (vittoria!=2) miglior_caso = vittoria;
  }

  out<<miglior_caso<<endl;
  out<<info_impostore.costo_totale << " " << info_stud.costo_totale<<endl;

  for (arco x: archi_speciali){
    out<< x.peso << " ";
  }
  out << endl;
  out << info_impostore.stanze_passate.size()<<endl;
  while (info_impostore.stanze_passate.size()>0){
    out<< info_impostore.stanze_passate.back()<< " ";
    info_impostore.stanze_passate.pop_back();
  }

  return 0;
}

