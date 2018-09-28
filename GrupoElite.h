#ifndef GRUPOELITE_H
#define GRUPOELITE_H

#include <iostream>
#include <list>
#include <fstream>
#include <string.h>
#include <time.h>

using namespace std;
typedef struct elite Elite;
  struct elite
  {
      list<int> set;
      int cost;
  };

class GrupoElite
{
    Elite grupo[50];
    int n;
    int tamMax;
    time_t start;

public:
  GrupoElite();
  GrupoElite(int tam);
  void addElite(list<int> set, int cost);
  void subsElite(list<int> set, int cost);
  int getMaiorValor();
  int getPosMaiorValor();
  GrupoElite & operator= (const GrupoElite &);
  bool checkIfExist(list<int> set, int cost);
  bool comparaLista(list<int> &L1, list<int> &L2);
  void Execute(list<int> set, int cost);
  void printaGrupoElite();
  int saveGrupoElite(char path[100]);
  void saveAllInfo(char path[100], char p);
};

#endif // GRUPOELITE_H