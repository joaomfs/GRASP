#ifndef GRUPOELITE_H
#define GRUPOELITE_H

#include <iostream>
#include <list>
#include <fstream>

using namespace std;
typedef struct elite Elite;
  struct elite
  {
      list<int> set;
      int cost;
  };

class GrupoElite
{
    Elite grupo[10];
    int n;

public:
  GrupoElite();
	void addElite(list<int> set, int cost);
	void subsElite(list<int> set, int cost);
  int getMaiorValor();
  int getPosMaiorValor();
  GrupoElite & operator= (const GrupoElite &);
	bool checkIfExist(list<int> set, int cost);
  bool comparaLista(list<int> &L1, list<int> &L2);
  void Execute(list<int> set, int cost);
  void printaGrupoElite();
  int saveGrupoElite();
};

#endif // GRUPOELITE_H
