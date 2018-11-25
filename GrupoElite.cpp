#include "GrupoElite.h"

GrupoElite::GrupoElite()
{
    n = 0;
    tamMax =10;
    time(&start);
}
GrupoElite::GrupoElite(int tam)
{
    n = 0;
    tamMax =tam;
    time(&start);
}

void GrupoElite::addElite(list<int> set, int cost)
{
    grupo[n].set = set;
    grupo[n].cost = cost;
    n++;
}


void GrupoElite::subsElite(list<int> set, int cost)
{
    int pos = getPosMaiorValor();
    grupo[pos].set = set;
    grupo[pos].cost = cost;
}

int GrupoElite::getMaiorValor()
{
    int maiorValor;
    if(n==0)
        return 0;
    maiorValor = grupo[0].cost; 
    for(int i=1;i<n;i++){
        if(grupo[i].cost>maiorValor)
            maiorValor=grupo[i].cost;
    }

    return maiorValor;
}

int GrupoElite::getPosMaiorValor()
{
    int maiorValor,m;
    if(n==0)
        return 0;
    maiorValor = grupo[0].cost;
    m=0;
    for(int i=1;i<n;i++){
        if(grupo[i].cost>maiorValor){
            maiorValor=grupo[i].cost;
            m=i;
        }
    }

    return m;
}

bool GrupoElite::checkIfExist(list<int> set, int cost)
{
    for(int i=0;i<n;i++){
        if(cost==grupo[i].cost && comparaLista(grupo[i].set, set))
            return true;
    }
    return false;
}
bool GrupoElite::comparaLista(list<int> &L1, list<int> &L2){
  list<int>::iterator it1, it2;
  for(it1 = L1.begin(), it2 = L2.begin(); it1 != L1.end() && it2!=L2.end(); ++it1, ++it2){
      int v1 = *it1;
      int v2 = *it2;
      if(v1!=v2)
        return false;
  }
  return true;
}
void GrupoElite::Execute(list<int> set, int cost){
    set.sort();
    if(!checkIfExist(set,cost)){
        if(n<tamMax)
            addElite(set,cost);
        else{
            if(cost<getMaiorValor())
                subsElite(set,cost);
        }
    }
}

void printalista(list<int> &L){
  list<int>::iterator it;
  for(it = L.begin(); it != L.end(); ++it){
    cout<<*it<<endl;
  }
}
void GrupoElite::printaGrupoElite(){
    ofstream out;
    out.open("conjunto elite.txt");
    for(int i=0; i<n;i++){
        out<<grupo[i].cost<<" "<<endl;
        printalista(grupo[i].set);
        cout<<"*****************************************"<<endl;
    }
    out.close();
}

int GrupoElite::saveGrupoElite(char path[100]){
    if(n<tamMax){
        cerr<<"conjunto elite menor que 10";
        return 0;
    }
    ofstream out;
    out.open(path);
    for(int i=0;i<n;i++){
        list<int>::iterator it;
        for(it = grupo[i].set.begin(); it!=grupo[i].set.end();it++){
            out<<*it<<" ";
        }
        out<<endl;
    }
    out.close();
    return 1;
}

void GrupoElite::saveAllInfo(char path[100], char p, char f){
    time_t end;
    time(&end);
    double time_taken= difftime(end, start);
    ofstream out;
    char pp[10], newpath[100];
    if(p=='a')
        strcpy(pp, "_pre_");
    else if(p=='d')
        strcpy(pp, "_pos_");
    else
        strcpy(pp, "deu ruim");
    if(f=='b')
        strcat(pp, "_busca_");
    else if(f=='c')
        strcat(pp, "_const_");
    else
        strcat(pp, "deu ruim");

    strcpy(newpath, path);
    strcat(newpath, pp);
    strcat(newpath, "all_info");
    out.open(newpath);
    out<<"Tamanho    Peso"<<endl;
    for(int i=0;i<n;i++){
        out<<grupo[i].set.size()<< "  ";
        out<<grupo[i].cost;
        out<<endl;
    }
    out<<endl<<"Time taken: "<< time_taken<<endl;
    out.close();
}

GrupoElite& GrupoElite::operator= (const GrupoElite &g)
{
    if(this != &g)
    {

        this->n = g.n;

        for(int i = 0; i < g.n; i++)
            {
                list<int>::iterator it;
                for(it = grupo[i].set.begin(); it != g.grupo[i].set.end(); ++it){
                    grupo[i].set.push_back(*it);
                }
            }
    }
    return *this;
}


/********************* END OF GRAPH CLASS *********************************************/