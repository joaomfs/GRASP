    #include "Graph.h"

    Graph::Graph()
    {
    	n = 0;
    	m = 0;
    	adj = NULL;
    	deg = NULL;
    }

    Graph::Graph(int numb_vert)
    {
    	n = numb_vert;
    	m = 0;
    	adj = new list<int>[n];
    	deg = new int[n];
    	for(int i = 0; i < n; i++)
            deg[i] = 0;
    }

     Graph::Graph (const Graph &g)
    {
        this->n = g.n;
        this->m = g.m;
        adj = new list<int>[n];
        deg = new int[n];
            for(int i = 0; i < n; i++)
            {
                list<int>::iterator it;
                for(it = g.adj[i].begin(); it != g.adj[i].end(); ++it)
                    adj[i].push_back(*it);
                deg[i] = g.deg[i];
            }
    }

    Graph::~Graph()
    {
    	delete []adj;
    	delete []deg;
    }

    Graph& Graph::operator= (const Graph &g)
    {
    	if(this != &g)
        {

            delete [] adj;
            delete [] deg;

            this->n = g.n;
            this->m = g.m;

            adj = new list<int>[n];
            deg = new int[n];
            for(int i = 0; i < n; i++)
                {
                    list<int>::iterator it;
                    for(it = g.adj[i].begin(); it != g.adj[i].end(); ++it)
                        adj[i].push_back(*it);
                    deg[i] = g.deg[i];
                }
        }
        return *this;
    }



    void Graph::set_number_vert(int numb_vert)
    {
        n = numb_vert;

        delete [] adj;
        delete [] deg;

    	adj = new list<int>[n];
    	deg = new int[n];
    	for(int i = 0; i < n; i++)
            deg[i] = 0;
    }


    void Graph::addEdge(int src, int dest)
    {
        if(src == dest)
            return;
    	adj[src].push_back(dest);
    	adj[dest].push_back(src);

    	deg[src]++;
    	deg[dest]++;

    	m++;
    }


    void Graph::check_addEdge(int src, int dest)
    {


    	if(src == dest)
            return;

        if(src > dest)
        {
            int aux = src;
            src = dest;
            dest = aux;
        }

    	if(existsEdge(src, dest))
            return;

    	adj[src].push_back(dest);
    	adj[dest].push_back(src);

    	deg[src]++;
    	deg[dest]++;

    	m++;
    }

      void Graph::removeEdge(int src, int dest)
    {
    	adj[src].remove(dest);
    	adj[dest].remove(src);

    	deg[src]--;
    	deg[dest]--;

        m--;
    }


    bool Graph::existsEdge(int src, int dest)
    {
            if(deg[src] > deg[dest])
            {
                int aux = src;
                src = dest;
                dest = aux;
            }

            list<int>::iterator it;

    		for(it = adj[src].begin(); it != adj[src].end(); ++it)
    		{
    			if(*it == dest)
                   return true;
    		}
            return false;
    }


    void Graph::printGraph()
    {
    	for(int i = 0; i < n ; i++)
    	{
    		cout<<"Degree of vertex "<<i<<" is : ";
    		cout<<deg[i]<<endl;

    		cout<<"Adjecency List of vertex "<<i<<" is : ";
    		list<int>::iterator it;
    		for(it = adj[i].begin(); it != adj[i].end(); ++it)
    			cout<<*it<<" -> ";
    		cout<<"\n";
    	}
    	cout<<"Number of vertices: "<<n<<endl;
    	cout<<"Number of edges: "<<m<<endl;
    }


/********************* END OF GRAPH CLASS *********************************************/
