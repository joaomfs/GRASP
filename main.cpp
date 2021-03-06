    #include <iostream>
    #include <map>
    #include <list>
    #include <queue>
    #include <vector>
    #include <stdlib.h>
    #include <fstream>
    #include <time.h>
    #include <cstdlib>
    #include <dirent.h>
    #include <string.h>
    #include <algorithm>
    #include <iomanip>
    #include <cmath>
    #include "Graph.h"
    #include "ReadFile.h"
    #include "GrupoElite.h"

    #define NUM_REP 1
    #define NUM_TEST 10
    #define NUM_INST 6

    #define MAX_ITERATION 100

    #define TIME_LIMIT  300 // 5 min
    #define MAXCOST 100
    #define BETA 50    //percent used in local search
    #define MAX_NEIGHBOUR 2
    #define ALPHA_G  0.90    //alpha to the Restricted Candidate List

    using namespace std;
    int seed;
    int iteracoes;
    typedef struct myPair PAIR;
    struct myPair
    {
        int v;
        double value;
        myPair(int v1 = -1, double value1 = -1){ v = v1; value = value1;}
        bool operator<(const myPair &p){return value < p.value;}
        bool operator==(const myPair &p){return v == p.v && value == p.value;}
        myPair(const myPair &p){v = p.v; value = p.value;}
    };


    /* the modified version of the WTSS heuristic*/
    int WTSS2(const Graph &G, int *C, int *T, list<int> &S)
    {
        int cost = 0, n = G.get_num_vert(), u, v, elemR, vmax, caso;
        int *d = new int[n], *k = new int[n], * process = new int [n];
        double fvalue, fmax;
        list<int> *N = new list<int>[n];

        for(v = 0; v < n; v++)
        {
            d[v] = G.degree(v);
            k[v] = T[v];
            N[v] = G.getList_Adj(v);
            process[v] = 0;
        }

        list<int>::iterator itU, itN;
        int numProcess = 0, inicio = 0;

        while(numProcess < n)
        {
            caso = 3;

            vmax = 0;
            fmax = -1;

            for(v = inicio; v < n && caso==3; v++)
            {
                if(process[v] == 0)
                {
                    if(k[v] == 0 )
                    {
                        caso = 1;
                        elemR = v;
                        inicio = -1;
                        for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                        {
                            u = *itN;
                            if(k[u] > 0 && process[u] == 0)
                            {
                                k[u] = k[u]-1;
                                if(k[u] == 0)
                                {
                                    if(inicio == -1)
                                        inicio = u;
                                    else
                                    {
                                        if(u < inicio)
                                            inicio = u;
                                    }

                                }

                            }
                        }
                        if(inicio == -1)
                            inicio = 0;
                    }
                    else
                    {
                        if(d[v] < k[v])
                        {
                            caso = 2;
                            elemR = v;
                            S.push_back(v);
                            cost += C[v];
                            inicio = -1;

                            for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                            {
                                u = *itN;
                                if(k[u] > 0 && process[u] == 0)
                                {
                                    k[u] = k[u]-1;
                                    if(k[u] == 0)
                                    {
                                        if(inicio == -1)
                                            inicio = u;
                                        else
                                        {
                                            if(u < inicio)
                                                inicio = u;
                                        }

                                    }
                                }
                            }

                            if(inicio == -1)
                                inicio = 0;
                        }

                        else
                        {
                            fvalue = (C[v]*n*1.) / (d[v]*(d[v]+1-k[v])) ;
                            if(fmax < fvalue)
                            {
                                fmax = fvalue;
                                vmax = v;
                            }
                        }

                    }
                }

            }

            if(caso == 3)
            {
                elemR = vmax;
                inicio = 0;
            }

            for(itN = N[elemR].begin(); itN != N[elemR].end(); ++itN)
            {
                u = *itN;
                if(process[u] == 0)
                    d[u] = d[u] - 1;
            }

            numProcess++;
            process[elemR] = 1;
        }

        delete []d;
        delete []k;

        for(int i = 0; i < n; i++)
            N[i].clear();
        delete []N;

        return cost;
    }


    /* A greedy function based in WTSS2, but start from a partial solution*/
    /* This function is used in the local search*/
    void WTSS2_aux(const Graph &G, int *C, int *T, list<int> &S, int &cost, int*d, int*k, list<int> *N, int *process, int &numProcess)
    {
        int n = G.get_num_vert(), u, v, elemR, vmax, caso;
        double fvalue, fmax;


        list<int>::iterator itN;
        int inicio = 0;

        while(numProcess < n)
        {
            caso = 3;

            vmax = 0;
            fmax = -1;

            for(v = inicio; v < n && caso==3; v++)
            {
                if(process[v] == 0)
                {
                    if(k[v] == 0 )
                    {
                        caso = 1;
                        elemR = v;
                        inicio = -1;
                        for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                        {
                            u = *itN;
                            if(k[u] > 0 && process[u] == 0)
                            {
                                k[u] = k[u]-1;
                                if(k[u] == 0)
                                {
                                    if(inicio == -1)
                                        inicio = u;
                                    else
                                    {
                                        if(u < inicio)
                                            inicio = u;
                                    }

                                }

                            }
                        }
                        if(inicio == -1)
                            inicio = 0;
                    }
                    else
                    {
                        if(d[v] < k[v])
                        {
                            caso = 2;
                            elemR = v;
                            S.push_back(v);
                            cost += C[v];
                            inicio = -1;

                            for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                            {
                                u = *itN;
                                if(k[u] > 0 && process[u] == 0)
                                {
                                    k[u] = k[u]-1;
                                    if(k[u] == 0)
                                    {
                                        if(inicio == -1)
                                            inicio = u;
                                        else
                                        {
                                            if(u < inicio)
                                                inicio = u;
                                        }

                                    }
                                }
                            }

                            if(inicio == -1)
                                inicio = 0;
                        }

                        else
                        {
                            fvalue = (C[v]*k[v]*1.) / (d[v]*(d[v]+1));
                            if(fmax < fvalue)
                            {
                                fmax = fvalue;
                                vmax = v;
                            }
                        }

                    }
                }


            }

            if(caso == 3)
            {
                elemR = vmax;
                inicio = 0;
            }

            for(itN = N[elemR].begin(); itN != N[elemR].end(); ++itN)
            {
                u = *itN;
                if(process[u] == 0)
                    d[u] = d[u] - 1;
            }

            numProcess++;
            process[elemR] = 1;
        }

    }


    /** a greedy randomized function for generate initials solutions in the graph*/
    int InitialSolution(const Graph &G, int *C, int *T, list<int> &S, int*d, int*k, list<int> *N, int *process)
    {
        int cost = 0, n = G.get_num_vert(), u, v, elemR, caso, pos, random_pos;
        double fvalue, fmax, fmin;
        list<int> LC;

        for(v = 0; v < n; v++)         //U = V(G)
        {
            d[v] = G.degree(v);
            k[v] = T[v];
            process[v] = 0;
        }

        list<int>::iterator itN;
        int numProcess = 0, inicio = 0;

        while(numProcess < n)
        {
            caso = 3;
            LC.clear();
            fmax = -1;
            fmin = (MAXCOST+2)*n;

            if(inicio == 0)
            {

                for(v = 0; v < n; v++)
                {
                    if(k[v] > 0 && d[v] >= k[v] && process[v] == 0)
                    {
                        fvalue = (C[v]*n*1.) / (d[v]*(d[v]+1-k[v]));
                        if(fmax < fvalue)
                            fmax = fvalue;
                        if(fmin > fvalue)
                            fmin = fvalue;
                    }
                }
            }

            for(v = inicio; v < n && caso==3; v++)
            {
                if(process[v] == 0)
                {
                    if(k[v] == 0 )
                    {
                        caso = 1;
                        elemR = v;
                        inicio = -1;
                        for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                        {
                            u = *itN;
                            if(k[u] > 0 && process[u] == 0)
                            {
                                k[u] = k[u]-1;
                                if(k[u] == 0)
                                {
                                    if(inicio == -1)
                                        inicio = u;
                                    else
                                    {
                                        if(u < inicio)
                                            inicio = u;
                                    }

                                }

                            }
                        }
                        if(inicio == -1)
                            inicio = 0;
                    }
                    else
                    {
                        if(d[v] < k[v])
                        {
                            caso = 2;
                            elemR = v;
                            S.push_back(v);
                            cost += C[v];
                            inicio = -1;

                            for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                            {
                                u = *itN;
                                if(k[u] > 0 && process[u] == 0)
                                {
                                    k[u] = k[u]-1;
                                    if(k[u] == 0)
                                    {
                                        if(inicio == -1)
                                            inicio = u;
                                        else
                                        {
                                            if(u < inicio)
                                                inicio = u;
                                        }

                                    }
                                }
                            }

                            if(inicio == -1)
                                inicio = 0;
                        }

                        else
                        {
                            fvalue = (C[v]*n*1.) / (d[v]*(d[v]+1-k[v])) ;
                            if( fvalue >= fmin + (fmax-fmin)*ALPHA_G )
                                LC.push_back(v);
                        }
                    }
                }

            }


            if(caso == 3)
            {
                pos = 0;
                random_pos = rand()%LC.size();

                while(pos < random_pos)
                {
                    LC.pop_front();
                    pos++;
                }

                elemR = LC.front();
                inicio = 0;
            }

            for(itN = N[elemR].begin(); itN != N[elemR].end(); ++itN)
            {
                u = *itN;
                if(process[u] == 0)
                    d[u] = d[u] - 1;
            }

            numProcess++;
            process[elemR] = 1;
        }

        return cost;
    }
        /** a greedy randomized function for generate initials solutions in the graph that receives a partial solucion*/
    int InitialSolution_aux(const Graph &G, int *C, int *T, list<int> &S, int*d, int*k, list<int> *N, int *process, int &numProcess, int &cost)
    {
        int n = G.get_num_vert(), u, v, elemR, caso, pos, random_pos;
        double fvalue, fmax, fmin;
        list<int> LC;

        list<int>::iterator itN;
        int inicio = 0;

        while(numProcess < n)
        {
            caso = 3;
            LC.clear();
            fmax = -1;
            fmin = (MAXCOST+2)*n;

            if(inicio == 0)
            {

                for(v = 0; v < n; v++)
                {
                    if(k[v] > 0 && d[v] >= k[v] && process[v] == 0)
                    {
                        fvalue = (C[v]*k[v]*1.) / (d[v]*(d[v]+1));
                        if(fmax < fvalue)
                            fmax = fvalue;
                        if(fmin > fvalue)
                            fmin = fvalue;
                    }
                }
            }

            for(v = inicio; v < n && caso==3; v++)
            {
                if(process[v] == 0)
                {
                    if(k[v] == 0 )
                    {
                        caso = 1;
                        elemR = v;
                        inicio = -1;
                        for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                        {
                            u = *itN;
                            if(k[u] > 0 && process[u] == 0)
                            {
                                k[u] = k[u]-1;
                                if(k[u] == 0)
                                {
                                    if(inicio == -1)
                                        inicio = u;
                                    else
                                    {
                                        if(u < inicio)
                                            inicio = u;
                                    }

                                }

                            }
                        }
                        if(inicio == -1)
                            inicio = 0;
                    }
                    else
                    {
                        if(d[v] < k[v])
                        {
                            caso = 2;
                            elemR = v;
                            S.push_back(v);
                            cost += C[v];
                            inicio = -1;

                            for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                            {
                                u = *itN;
                                if(k[u] > 0 && process[u] == 0)
                                {
                                    k[u] = k[u]-1;
                                    if(k[u] == 0)
                                    {
                                        if(inicio == -1)
                                            inicio = u;
                                        else
                                        {
                                            if(u < inicio)
                                                inicio = u;
                                        }

                                    }
                                }
                            }

                            if(inicio == -1)
                                inicio = 0;
                        }

                        else
                        {
                            fvalue = (C[v]*k[v]*1.) / (d[v]*(d[v]+1)) ;
                            if( fvalue >= fmin + (fmax-fmin)*ALPHA_G )
                                LC.push_back(v);
                        }
                    }
                }

            }


            if(caso == 3)
            {
                pos = 0;
                random_pos = rand()%LC.size();

                while(pos < random_pos)
                {
                    LC.pop_front();
                    pos++;
                }

                elemR = LC.front();
                inicio = 0;
            }

            for(itN = N[elemR].begin(); itN != N[elemR].end(); ++itN)
            {
                u = *itN;
                if(process[u] == 0)
                    d[u] = d[u] - 1;
            }

            numProcess++;
            process[elemR] = 1;
        }

        return cost;
    }

    /*A local search using the first improvement strategy*/
    int firstBestN(const Graph &G, int *C, int *T, list<int> &S, int cost, int*d, int*k, list<int> *N, int *process)
    {
        int cost2 = cost, bestCost = cost, n = G.get_num_vert(), u, v, numProcess;
        list<int> S2, Sbest = S;
        list<int>::iterator itS, itN;

        int neighbourg = 0;

        do{

            S2 = Sbest;

            neighbourg++;

            for(v = 0; v < n; v++)         //U = V(G)
            {
                d[v] = G.degree(v);
                k[v] = T[v];
                process[v] = 0;
            }

            vector<int> Sk(S2.size());
            vector<int>::iterator itSk;

            v = 0;

            for(itS = S2.begin(); itS != S2.end(); ++itS)
            {
                Sk[v] = *itS;
                v++;
            }

            random_shuffle(Sk.begin(), Sk.end());

            int cantRemov = (Sk.size()* BETA)/100;

            for(int i = 0; i < cantRemov; i++)
            {
                Sk.pop_back();
            }

            S2.clear();
            cost2 = 0;

            for(itSk = Sk.begin(); itSk != Sk.end(); ++itSk)
            {
                v = *itSk;
                process[v] = 1;
            }

            numProcess = 0;

            for(itSk = Sk.begin(); itSk != Sk.end(); ++itSk)
            {
                v = *itSk;
                numProcess++;
                S2.push_back(v);
                cost2 += C[v];

                for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                {
                    u = *itN;
                    if(process[u] == 0)
                    {
                        if(k[u] > 0)
                            k[u] = k[u]-1;
                        d[u] = d[u] - 1;
                    }

                }
            }

            WTSS2_aux(G,C,T,S2,cost2,d,k,N,process,numProcess);

            if(cost2 < bestCost)
            {
                Sbest = S2;
                bestCost = cost2;
                neighbourg = 0;
            }

        }while(neighbourg < MAX_NEIGHBOUR);

        S = Sbest;
        return bestCost;
    }


    int LocalSearch(const Graph &G, int *C, int *T, list<int> &S, int cost, int*d, int*k, list<int> *N, int * process)
    {
        int bestCost = firstBestN(G,C,T,S,cost,d,k,N,process);
        return bestCost;
    }

    /*The GRASP procedure*/
    int GRASP(const Graph &G, int *C, int *T, list<int> &S, int maxIter, char name[100])
    {
        int cost, numIter = 0, bestCost, n = G.get_num_vert();       //Initial Solution
        list<int> Sbest, S2;
        int *d = new int[n], *k = new int[n], *process = new int [n];
        list<int> *N = new list<int>[n];
        double *time_constr= new double[maxIter];
        double *time_localSearch= new double[maxIter];
        clock_t start_const, start_localSearch, end_const, end_localSearch;
        for(int i = 0; i < n; i++)
            N[i] = G.getList_Adj(i);

        cost = WTSS2(G,C,T,S2);
        cost = LocalSearch(G,C,T,S2,cost,d,k,N,process);

        Sbest = S2;
        bestCost = cost;
                /*MAGIA DOS CAMINHOS*/
        /*constroi caminho conjunto elite*/
        char path_ce[100];  //CAMINHO CONJUNTO ELITE
        strcpy(path_ce, name);
        strcat(path_ce, "_conjunto_elite");
        //fim constroi caminho conjunto elite
        /*constroi caminho padroes*/
        char path[100];
        strcpy(path, "../GRASP/");
        strcat(path, name);
        char path_info[100];
        strcpy(path_info, path);
        GrupoElite ge, pre_construcao(50), pre_local_search(50);
        while(numIter < maxIter/2)
        {
            numIter++;
            S2.clear();

            start_const = clock();
            
            cost = InitialSolution(G,C,T,S2,d,k,N,process);
            pre_construcao.addElite(S2, cost);

            end_const = clock();
            start_localSearch = clock();

            cost = LocalSearch(G,C,T,S2,cost,d,k,N,process);
            end_localSearch = clock();

            time_constr[numIter-1]=((double)end_const- (double)start_const)/CLOCKS_PER_SEC;
            time_localSearch[numIter-1]= ((double)end_localSearch- (double)start_localSearch)/CLOCKS_PER_SEC;

            if(cost < bestCost)
            {
                Sbest = S2;
                bestCost = cost;
            }

            ge.Execute(S2, cost);
            pre_local_search.addElite(S2,cost);

        }

        pre_local_search.saveAllInfo(path_info, 'a', 'b');
        pre_construcao.saveAllInfo(path_info, 'a', 'c');

        if(!ge.saveGrupoElite(path_ce)) exit(1);

        strcat(path, "_patterns");
        /*fim constroi caminho padroes e começa constroi cmd*/
        char cmd[200];
        strcpy(cmd, "cd ../FPmax; ./fpmax_hnmp 1 1 ../GRASP/");
        strcat(cmd, path_ce);
        strcat(cmd, " 10 7 10 ");
        strcat(cmd, path);
        //fim constroi cmd
        system(cmd);
        list<int> patterns[10];
        int tam = readSaida(path, patterns);
        list<int> Sk;
        list<int>::iterator itSk,itN;
        int u, v,numProcess;
        GrupoElite pos_construcao(50), pos_local_search(50);
        while(numIter < maxIter)
        {
            numProcess=0, cost=0;
            Sk = patterns[numIter%tam];
            numIter++;
            S2.clear();
            start_const=clock();
            for(v = 0; v < n; v++)         //U = V(G)
            {
                d[v] = G.degree(v);
                k[v] = T[v];
                process[v] = 0;
            }

            for(itSk = Sk.begin(); itSk != Sk.end(); ++itSk)
            {
                v = *itSk;
                process[v] = 1;
                numProcess++;
                S2.push_back(v);
                cost += C[v];

                for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                {
                    u = *itN;
                    if(process[u] == 0)
                    {
                        if(k[u] > 0)
                            k[u] = k[u]-1;
                        d[u] = d[u] - 1;
                    }

                }
            }

            cost = InitialSolution_aux(G,C,T,S2,d,k,N,process,numProcess, cost);
            pos_construcao.addElite(S2,cost);
            end_const=clock();
            start_localSearch = clock();
            cost = LocalSearch(G,C,T,S2,cost,d,k,N,process);
            end_localSearch = clock();
            time_constr[numIter-1]=((double)end_const- (double)start_const)/CLOCKS_PER_SEC;
            time_localSearch[numIter-1]= ((double)end_localSearch- (double)start_localSearch)/CLOCKS_PER_SEC;

            if(cost < bestCost)
            {
                Sbest = S2;
                bestCost = cost;
            }
            pos_local_search.addElite(S2, cost);
        }
        pos_local_search.saveAllInfo(path_info, 'd', 'b');
        pos_construcao.saveAllInfo(path_info, 'd', 'c');

        saveTimes(path_info, time_constr, time_localSearch, maxIter);

        delete []d;
        delete []k;
        delete []process;
        delete []time_constr;
        delete []time_localSearch;


        for(int i = 0; i < n; i++)  
            N[i].clear();
        delete []N;

        for(int i = 0; i < 10; i++)  
            patterns[i].clear();

        S = Sbest;
        return bestCost;
    }

    /* function used to check a solution in the develop stage*/
    bool check2(const Graph &G, int *C, int *T, list<int> &S, int cost, char name[20])
    {
        int c = 0, n = G.get_num_vert(), u, v, elemR;
        int *d = new int[n], *k = new int[n];
        list<int> U, *N = new list<int>[n];
        list<int>::iterator itS,itN,itU;

        ofstream out;
        out.open("Result.txt", ios::out | ios::app);

        for(itS = S.begin(); itS != S.end(); ++itS)
            c += C[*itS];
        if(c != cost)
        {out<<"Problemas con el costo costS: "<<c<<" , cost: "<<cost<<"\n"; out.close();return false;}



        for(v = 0; v < n; v++)         //U = V(G)
        {
            U.push_back(v);
            d[v] = G.degree(v);
            k[v] = T[v];
            N[v] = G.getList_Adj(v);
        }

        for(itS = S.begin(); itS != S.end(); ++itS)
        {
            v = *itS;

            for(itN = N[v].begin(); itN != N[v].end(); ++itN)
            {
                u = *itN;
                if(k[u] > 0)
                    k[u] = k[u]-1;
                d[u] = d[u] - 1;
                N[u].remove(v);
            }
            U.remove(v);
        }


        bool change = true;
        while(!U.empty() && change)
        {
            change = false;
            for(itU = U.begin(); itU != U.end(); ++itU)
            {
                v = *itU;

                if(k[v] == 0)
                {
                    change = true;
                    elemR = v;
                    for(itN = N[v].begin(); itN != N[v].end(); ++itN)
                    {
                        u = *itN;
                        if(k[u]>0)
                            k[u] = k[u]-1;
                        d[u] = d[u] - 1;
                        N[u].remove(elemR);
                    }
                    break;

                }
            }
            U.remove(elemR);

        }
        bool result = U.empty()?true:false;

            while(!U.empty())
            {
                v = U.front();
                U.pop_front();
                out<<v<<"\t";
            }
            out<<"\n";
            out.close();
            return result;
    }

    void read_instances(int num_instance, Graph& G)
    {
        char path[100];
        switch(num_instance)
        {

            case 1: strcpy(path,"../DBWTSS/power.txt");
                    readPower(path,G);
                    break;

            case 2: strcpy(path,"../DBWTSS/CA-GrQc.txt");
                    readStanfordCA(path,G);
                    break;

            case 3: strcpy(path,"../DBWTSS/facebook.txt");
                    readFacebook(path,G);
                    break;

            case 4: strcpy(path,"../DBWTSS/CA-HepPh.txt");
                    readStanfordCA(path,G);
                    break;

            case 5: strcpy(path,"");
                    readBlog(path,G);
                    break;

            case 6: strcpy(path,"../DBWTSS/CA-HepTh.txt");
                    readStanfordCA(path,G);
                    break;

            default: break;

        }

    }

    int main(int argc, char *argv[])
    {
        int n, n2, *T, *C;
        list<int> S;
        seed = atoi(argv[1]);
        int test = atoi(argv[2]);
        int num_inst = atoi(argv[3]);

        //int num_inst = 0;
        float meanG, varG;
        int resultG[NUM_REP], minCost;
        char instances_name[][20] = {"CA-GrQc", "Power","Facebook","CA-HepPh.txt", "BlogCatalog","CA-HepTh"};
        float meanTimeG;

        srand (seed);

        //ERA O WHILE
            ofstream out;
            char dir_result[100];
            sprintf (dir_result, "%d", seed);
	    strcat(dir_result, "/");
	    char instancia[2];
	    sprintf (instancia, "%d", test);
            char dir_prev_result[100] = {"/GRASP_FPMax7_"}, num[10];
            sprintf (num, "%d", num_inst);
            strcat(dir_result, dir_prev_result);
            strcat(dir_result,instancia);
            strcat(dir_result,instances_name[num_inst-1]);

            out.open(dir_result);
            out<<endl;
            out<<"Number of iterations: "<<MAX_ITERATION<<endl;
            out<<"Limit time: "<<TIME_LIMIT<<endl;
            out<<"ALPHA_G: "<<ALPHA_G<<endl;
            out<<"BETA: "<<BETA<<endl<<endl<<endl;
            out.precision(3);

            Graph G;
            read_instances(num_inst, G);
            out<<instances_name[num_inst-1];
            cout<<"Processing "<<instances_name[num_inst-1]<<" ..."<<endl;
            out<<"G: n = "<< G.get_num_vert() <<" , m = "<<G.get_numb_edges()<<endl<<endl;

            n = G.get_num_vert();

            T = new int[n];
            C = new int[n];


            //int test = 0;
                
                minCost = G.get_num_vert()*MAXCOST;
                char path_testes[100];
                sprintf(path_testes, "%d/%d/", seed,test);
                strcat(path_testes, instances_name[num_inst-1]);
                ifstream inData;
                char path_Data[10] = "Data_", num_path[10], path_Ins[100];
                sprintf (num_path, "%d", test);
                strcat(path_Data,num_path);

                switch(num_inst)
                {
                    case 1: strcpy(path_Ins,"INSTANCES/Power/");
                            break;
                    case 2: strcpy(path_Ins,"INSTANCES/QrQc/");
                            break;
                    case 3: strcpy(path_Ins,"INSTANCES/Facebook/");
                            break;
                    case 4: strcpy(path_Ins,"INSTANCES/Hep/");
                            break;
                    case 5: strcpy(path_Ins,"INSTANCES/Blog3/");
                            break;
                    case 6: strcpy(path_Ins,"INSTANCES/HepTh/");
                            break;
                    default: break;
                }
                strcat(path_Ins, path_Data);
                inData.open(path_Ins);
                if(inData.fail())
                {
                    cerr << "problema ao abrir o arquivo de dados";
                    exit(1);
                }
                inData>>n2;

                for(int i = 0; i < n; i++)
                {
                    inData>>C[i]>>T[i];
                }

                inData.close();

                double Time;

                meanG = 0;
                meanTimeG = 0;
                int cost;
                time_t start, end;
                time(&start);
                for(int num_rep = 0; num_rep < NUM_REP; num_rep++)
                {
                S.clear();
                
                cost = GRASP(G,C,T,S,MAX_ITERATION, path_testes);
                //start = clock();
                time(&end);
                Time = difftime(end, start);
                out<<S.size()<<"\t";
                out<<cost<<"\t";
                out<<Time<<"\n\n";

                meanTimeG += Time;
                meanG += cost;

                resultG[num_rep] = cost;

                if(cost < minCost)
                    minCost = cost;
                }


                meanG = (meanG*1.0)/NUM_REP;
                meanTimeG = (meanTimeG*1.0)/NUM_REP;

                varG = 0;

                for(int num_rep = 0; num_rep < NUM_REP; num_rep++)
                {
                    varG += (resultG[num_rep] - meanG)*(resultG[num_rep] - meanG);
                }

                //out<<minCost<<"\t"<<meanG<<"\t"<<sqrt(varG)<<"\t"<<meanTimeG<<endl;   //for several repetitions

                

                bool check = check2(G,C,T,S,cost,instances_name[num_inst-1]);  //just in the develop stage
                if(check == false){
                    cout<<"An error ocurred!!!"<<endl;
                    out<<"An error ocurred!!!" <<endl;
                    out.close();
                    exit(1);
                }
                else
                    cout<<"OK"<<endl;
                

            out.close();

            delete []T;
            delete []C;
        

        return 0;
    }

