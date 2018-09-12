    #ifndef READFILE_H
    #define READFILE_H

    #include <fstream>
    #include <list>
    #include <set>
    #include <map>
    #include <stdlib.h>
    #include <string.h>



//******READING INSTANCES**************

    void readPower(char *ruta, Graph& G)
    {
        int n = 0, src, dest;
        char trash[128];
        bool node = true, edge = true;

        ifstream f;
        f.open(ruta);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }


        f.getline(trash,128);f.getline(trash,128);
        f.getline(trash,128);f.getline(trash,128);

        do{
            f>>trash;
            if(strcmp(trash,"node")==0)
                {
                    n++;
                    f.getline(trash,128);f.getline(trash,128);
                    f.getline(trash,128);f.getline(trash,128);
                }
            else
                node = false;
        }while(node);

        G.set_number_vert(n);

        while(edge)
        {
            f.getline(trash,128);f.getline(trash,128);

            f>>trash; f>>src;
            f>>trash; f>>dest;
            G.addEdge(src,dest);

            f.getline(trash,128);f.getline(trash,128);

            f>>trash;

            if(strcmp(trash,"edge")!=0)
                edge = false;
        }

        f.close();
    }


    void readAmazon(char *ruta, Graph& G)
    {
        int n = 0, m = 0, src, dest;
        char trash[128];

        ifstream f;
        f.open(ruta);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }
        f.getline(trash,128);f.getline(trash,128);
        f>>trash;f>>trash;
        f>>n;
        f>>trash;
        f>>m;
        f.getline(trash,128);f.getline(trash,128);

        G.set_number_vert(n);

        for(int i = 0; i < m; i++)
        {
            f >> src;
            f >> dest;
            if(src < dest)
                G.addEdge(src,dest);
            //G.check_addEdge(src,dest);
            f.getline(trash,128);
        }
    f.close();
    }


    void readStanfordCA(char *ruta, Graph& G)
    {
        int n = 0, m = 0, src, dest;
        char trash[200];

        ifstream f,f2;
        f.open(ruta);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }
        f.getline(trash,200);f.getline(trash,200);
        f>>trash;f>>trash;
        f>>n;
        f>>trash;
        f>>m;
        f.getline(trash,200);f.getline(trash,200);

        G.set_number_vert(n);
        set<int>Id;

        for(int i = 0; i < m; i++)
        {
            f >> src;
            f >> dest;
            Id.insert(src);
            Id.insert(dest);
            f.getline(trash,200);
        }
        f.close();

        map<int,int> mymap;
        set<int>::iterator itS;

        int v = 0;
        for(itS = Id.begin(); itS != Id.end(); ++itS)
        {
            mymap.insert(pair<int,int>(*itS, v));
            v++;
        }

        f2.open(ruta);
        f2.getline(trash,200);f2.getline(trash,200);
        f2>>trash;f2>>trash;
        f2>>n;
        f2>>trash;
        f2>>m;
        f2.getline(trash,200);f2.getline(trash,200);

        for(int i = 0; i < m; i++)
        {
            f2 >> src;
            f2 >> dest;
            G.check_addEdge(mymap[src],mymap[dest]);//cout<<"src: "<<mymap[src]<<" "<<"dest"<<mymap[dest]<<endl;
            f2.getline(trash,200);
        }
        f2.close();
    }


    void readDouban(char *ruta, Graph& G)
    {
        int n = 0, src, dest;
        char c, trash[128], rutan[100] = "../DBWTSS/Douban-dataset/data/nodes.txt", rutae[100] = "../DBWTSS/Douban-dataset/data/edges.txt";

        ifstream f;
        f.open(rutan);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }
        f>>n;
        f.close();
        G.set_number_vert(n);

        f.open(rutae);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }


        while(!f.eof())
        {
            f >> src;
            f >> c;
            f >> dest;
            if(src <= n && dest <= n)
                G.check_addEdge(src-1,dest-1);
            f.getline(trash,128);
        }
        f.close();
    }


    void readBlog(char *ruta, Graph& G)
    {
        int n = 0, src, dest;
        char c, trash[128], rutan[100] = "../DBWTSS/BlogCatalog-dataset/data/nodes.csv", rutae[100] = "../DBWTSS/BlogCatalog-dataset/data/edges.csv";

        ifstream f;
        f.open(rutan);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo rutan "<< rutan;
            exit(1);
        }
        while(!f.eof())
        {
            f>>c; f.getline(trash,128);
            n++;
        }
        f.close();

        G.set_number_vert(n);

        f.open(rutae);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo rutae";
            exit(1);
        }


        while(!f.eof())
        {
            f >> src;
            f >> c;
            f >> dest;
            //if(src <= n && dest <= n)
            //    G.check_addEdge(src-1,dest-1);
            G.addEdge(src-1,dest-1);
            f.getline(trash,128);
        }
        f.close();
    }



    void readBlog3(char *ruta, Graph& G)
    {
        int n = 0, src, dest;
        char c, trash[128], rutan[100] = "../DBWTSS/BlogCatalog-dataset3/data/nodes.txt", rutae[100] = "../DBWTSS/BlogCatalog-dataset3/data/edges.txt";

        ifstream f;
        f.open(rutan);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }
        while(!f.eof())
        {
            f>>c; f.getline(trash,128);
            n++;
        }
        f.close();

        G.set_number_vert(n);

        f.open(rutae);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }


        while(!f.eof())
        {
            f >> src;
            f >> c;
            f >> dest;
            //if(src <= n && dest <= n)
            //    G.check_addEdge(src-1,dest-1);
            G.addEdge(src-1,dest-1);
            f.getline(trash,128);
        }
        f.close();
    }


void readFacebook(char *ruta, Graph& G)
    {
        int n = 0, m = 0, src, dest;
        char trash[200];

        ifstream f,f2;
        f.open(ruta);
        if(f.fail())
        {
            cerr << "problema ao abrir o arquivo";
            exit(1);
        }

        set<int>Id;

        while(!f.eof())
        {
            f >> src;
            f >> dest;
            Id.insert(src);
            Id.insert(dest);
            f.getline(trash,200);
            m++;
        }
        f.close();

        n = Id.size();

        G.set_number_vert(n);

        f2.open(ruta);
        for(int i = 0; i < m; i++)
        {
            f2 >> src;
            f2 >> dest;
            G.check_addEdge(src,dest);
            f2.getline(trash,128);
        }
        f2.close();
    }

int readSaida(char* ruta, list<int> set[10]){
    int n,j,cs;
    ifstream f;
    f.open(ruta);
    if(f.fail())
    {
        cerr << "problema ao abrir o arquivo saida";
        exit(1);
    }
    int tam=0;
    while(!f.eof())
    {
        f>> n;
        f.ignore(1,';');
        f>>cs;
        f.ignore(1,';');
        for(int i=0;i<n;i++){
            f>>j;
            set[tam].push_back(j);
        }
        tam++;
    }
    f.close();
    return tam-1;
}


    #endif // READFILE_H

