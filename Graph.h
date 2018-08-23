#ifndef GRAPH_H
#define GRAPH_H

    #include <iostream>
    #include <list>

    using namespace std;

    class Graph
    {
            int n, m;
            list<int> *adj;
            int *deg;

    	public:
    	    Graph();
    		Graph(int);
    		Graph(const Graph&);
    		~Graph();

    		Graph & operator= (const Graph &);
    		void set_number_vert(int);
    		void addEdge(int, int);
    		void check_addEdge(int, int);
    		void removeEdge(int src, int dest);


            bool existsEdge(int , int );
            int get_num_vert()const {return n;}
            int get_numb_edges()const {return m;}
            int degree(int i) const {return deg[i];}
            list<int> getList_Adj(int i)const {return adj[i];}
            void printGraph();
    };

#endif // GRAPH_H
