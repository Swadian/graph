#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <set>
using namespace std;



class graph
{
    int n,m;
    vector<vector<int>> arcs;
    vector<vector<pair<int,int> > > cost_arcs;//first-destination, second-cost
    void sort_dfs(int start,vector<bool> &viz,queue<int> &coada);
    void tarjan(int n,vector< vector<int> > &C,vector<bool> &in_stack,stack <int> &S,vector<int> &idx,vector<int> &lowlink,int &index);
    void critical_arcs_dfs(int n,ostream &fout,vector<bool> &in_stack,stack <int> &S,vector<int> &idx,vector<int> &lowlink,int &index);
    int root(int x,vector<int> &tati,vector<int> &rang);
    void unite(int x,int y,vector<int> &tati,vector<int> &rang);
    bool negative_cycle=false;
    int apm_cost=0;
    int apm_size=0;
    int flow_BF(vector<int> &tati,vector<int> &viz,vector<int> &cd,vector< vector<int> > &capacities,vector< vector<int> > &flows);
public:
    int get_apm_cost(){return this->apm_cost;}
    int get_apm_size(){return this->apm_size;}
    void print();
    graph(){this->n=0;this->m=0;}
    graph(int n,int m,vector< vector<int>> arcs,vector<vector<pair<int,int> > > cost_arcs);
    graph(int n,int m,vector< vector<int>> arcs);
    void bfs(int start);
    void dfs(int start,vector<bool> &viz);//transforma vectorul dat intr-un vector de vizitari cu DFS din nodul start
    int componente_conexe();//returneaza numarul de componente conexe
    void sortare_top();
    graph Havel_Hakimi(vector<int> s);//returneaza un graf generat din vectorul de grade s
    void comp_tare_conexe();
    void critical_arcs();
    void disjoint_command(ifstream &fin,ofstream &fout,vector<int> &tati,vector<int> &rang);
    vector<int> bellman_ford();//actualizeaza verificarea daca are sau nu ciclu negativ si returneaza distantele de la nodul 0 la toate nodurile
    bool get_negative_cycle(){return this->negative_cycle;}
    vector<int> dijkstra();//returneaza distanta de la nodul 0 la celelalte noduri
    vector<vector<int> > apm();//returneaza APM-ul grafului
    void print_distance_matrix(string filename);//afiseaza matricea distantelor dintre toate nodurile in fisierul dat
    void roy_floyd();
    int d_arb();//returneaza diametrul arborelui (distanta cea mai mare intre doua frunze)
    int max_flow();//returneaza fluxul maxim din graf de la nodul 0 la n-1
    //doesn't work on IA
    bool has_euler_cycle();//returneaza daca graful are un ciclu eulerian
    vector<int> euler_cycle();//returneaza un ciclu eulerian din graf
    //doesn't work on IA
};
void graph::print()
{
    ofstream fout("graph.out");
    vector <int>::const_iterator it;
    for(int i=0; i<n; i++)
    {
        for (it = arcs[i].begin(); it != arcs[i].end(); ++ it)
            fout<<i<<' '<<*it<<'\n';
    }
}
graph::graph(int n,int m,vector< vector<int>> arcs)
{
    this->n=n;
    this->m=m;
    this->arcs=arcs;
}
graph::graph(int n,int m,vector< vector<int>> arcs,vector<vector<pair<int,int> > > cost_arcs)
{
    this->n=n;
    this->m=m;
    this->cost_arcs=cost_arcs;
    this->arcs=arcs;
}

void graph::bfs(int start)
{
    queue<int> que;
    que.push(start);
    int *dist=new int[this->n];
    for(int i=0; i<n; i++)dist[i]=-1; //initializez distantele cu -1
    dist[start]=0;//distanta startului e 0
    while(!que.empty())//cat timp mai am in coada
    {
        int current=que.front();//iau elementul curent
        que.pop();//il scot din coada
        for(unsigned int i=0; i<arcs[current].size(); i++) //ii parcurg lista de vecini
        {
            if(dist[arcs[current][i]]==-1)//daca vecinul e nevizitat
            {
                dist[arcs[current][i]]=dist[current]+1;//distanta lui devine cea a tatalui +1
                que.push(arcs[current][i]);//il pun in coada
            }
        }
    }
    ofstream fout("bfs.out");
    for(int i=0; i<this->n; i++)
        fout<<dist[i]<<' ';//afisez distantele
    delete[] dist;//sterg vectorul dinamic
}
void graph::dfs(int start,vector<bool> &viz)
{
    viz[start]=true;
    for(unsigned int i=0; i<arcs[start].size(); i++) //ii parcurg lista de vecini
    {
        if(!viz[arcs[start][i]])//daca e nevizitat
        {
            viz[arcs[start][i]]=true;//il vizitez
            this->dfs(arcs[start][i],viz);//apelez recursiv dfs pe el
        }
    }
}
int graph::componente_conexe()
{
    vector<bool> viz(n);
    for(int i=0; i<n; i++)viz[i]=false; //initializez cu fals vectorul
    int componente=0;//numarul de componente conexe
    for(int i=0; i<n; i++) //parcurg toate nodurile
        if(!viz[i])//daca nu e deja vizitat
        {
            dfs(i,viz);//incep un dfs din el
            componente++;//si am inca o componenta conexa
        }
    return componente;

}
void graph::sort_dfs(int start, vector<bool> &viz,queue<int> &coada)
{
    viz[start]=true;
    for(unsigned int i=0; i<arcs[start].size(); i++) //ii parcurg lista de vecini
    {
        if(!viz[arcs[start][i]])//daca e nevizitat
        {
            viz[arcs[start][i]]=true;//il vizitez
            this->sort_dfs(arcs[start][i],viz,coada);//apelez recursiv dfs pe el
        }
    }
    coada.push(start);
}
void graph::sortare_top()
{
    queue<int> coada;
    vector<bool> viz(n);
    for(int i=0; i<n; i++)viz[i]=false; //initializez cu fals vectorul
    sort_dfs(0,viz,coada);
    ofstream fout("sortaret.out");
    while(!coada.empty())
    {
        fout<<coada.back()+1<<' ';
        coada.pop();
    }

}
graph graph::Havel_Hakimi(vector<int> s)
{
    int n,m;
    m=0;
    n=s.size();
    vector<vector<int>> arcs;
    arcs.resize(n);
    vector<pair<int,int>> v;
    for(int i=0; i<n; i++)
        v.push_back(make_pair(i,s[i]));//first - indicele second - gradul
    sort(v.begin(),v.end(),[](pair<int,int> v1,pair<int,int> v2)->bool{return v1.second>v2.second;});//sortez dupa grade, in ordine inversa
    bool done=false;
    while(!done)
    {
        if(v[0].second>0)
        {
            for(int i=1; i<=v[0].second; i++) //de la urmatorul, pana epuizam gradul nodului de pe pozitia 0
            {
                m++;
                arcs[v[0].first].push_back(v[i].first);//pun arc intre noduri
                v[i].second--;//scad gradul si nodului pus
            }
        }
        else done=true;
        v[0].second=0;
        sort(v.begin(),v.end(),[](pair<int,int> v1,pair<int,int> v2)->bool{return v1.second>v2.second;});//sortez dupa grade, in ordine inversa
    }
    return graph(n,m,arcs);
}
void graph::comp_tare_conexe()
{
    vector<int> idx,lowlink;//indecsii nodurilor si indexul minim ce poate fi atins dintr-un dfs Tarjan
    vector<bool> in_stack;//daca nodul se afla in stiva pt Tarjan recursiv
    stack <int> S;//stiva pt Tarjan
    int index=0;
    vector< vector<int> > C;//ctc-urile
    lowlink.resize(n);//initializam marimea lui lowlink
    idx.assign(n, -1);
    in_stack.assign(n, 0);
    //initializam idx si in_stack cu valorile de inceput
    for (int i = 0; i < n; ++ i)
        if (idx[i] == -1)
            tarjan(i,C,in_stack,S,idx,lowlink,index);//apelam pentru fiecare nod nevizitat

    ofstream fout("ctc.out");
    fout<<(int)C.size()<<'\n';//numarul de linii din C este numarul de ctc
    for(int i=0; i<(int)C.size(); i++)
    {
        for(int j=0; j<(int)C[i].size(); j++)
            fout<<C[i][j]+1<<' ';
        fout<<'\n';
    }//afisez fiecare componenta in parte
    fout.close();
}
void graph::tarjan(int n,vector< vector<int> > &C,vector<bool> &in_stack,stack <int> &S,vector<int> &idx,vector<int> &lowlink,int &index)
{
    idx[n] = lowlink[n] = index;
    //setez indexul nodului, initializez lowlink-ul cu propria valoare
    index ++;

    S.push(n);
    in_stack[n] = true;
    //pun nodul curent in stiva si marchez asta
    vector <int>::const_iterator it;
    for (it = arcs[n].begin(); it != arcs[n].end(); ++ it)//parcurg toti vecinii nodului curent
    {
        if (idx[*it] == -1)//daca nu a mai fost vizitat
        {
            tarjan(*it,C,in_stack,S,idx,lowlink,index);//apelez recursiv
            lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca apelul recursiv a generat unul mai mic
        }
        else if (in_stack[*it])//daca e deja in stiva
            lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca este cazul
    }
    if (idx[n] == lowlink[n])//daca si-a pastrat lowlink-ul dupa parcurgere nu mai avem unde merge si afisam
    {
        vector<int> con;//ctc curenta
        int node;
        do
        {
            node = S.top();
            con.push_back(node);
            S.pop();
            in_stack[node] = false;
        }
        while (node != n);//dump la stiva in ctc curenta
        C.push_back(con);//pun ctc curenta in matricea de output
    }

}
void graph::critical_arcs_dfs(int n,ostream &fout,vector<bool> &in_stack,stack <int> &S,vector<int> &idx,vector<int> &lowlink,int &index)
{
    idx[n] = lowlink[n] = index;
    //setez indexul nodului, initializez lowlink-ul cu propria valoare
    index ++;

    S.push(n);
    in_stack[n] = true;
    //pun nodul curent in stiva si marchez asta
    vector <int>::const_iterator it;
    for (it = arcs[n].begin(); it != arcs[n].end(); ++ it)//parcurg toti vecinii nodului curent
    {
        if (idx[*it] == -1)//daca nu a mai fost vizitat
        {
            critical_arcs_dfs(*it,fout,in_stack,S,idx,lowlink,index);//apelez recursiv
            lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca apelul recursiv a generat unul mai mic
        }
        else if (in_stack[*it])//daca e deja in stiva
            lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca este cazul

        if(lowlink[*it]>idx[n]) fout<<n<<' '<<*it;
    }

}
void graph::critical_arcs()
{
    vector<int> idx,lowlink;//indecsii nodurilor si indexul minim ce poate fi atins dintr-un dfs Tarjan
    vector<bool> in_stack;//daca nodul se afla in stiva pt Tarjan recursiv
    stack <int> S;//stiva pt Tarjan
    int index=0;
    ofstream fout("crit_conn.out");
    lowlink.resize(n);//initializam marimea lui lowlink
    idx.assign(n, -1);
    in_stack.assign(n, 0);
    //initializam idx si in_stack cu valorile de inceput
    for (int i = 0; i < n; ++ i)
        if (idx[i] == -1)
            critical_arcs_dfs(i,fout,in_stack,S,idx,lowlink,index);//apelam pentru fiecare nod nevizitat
    fout.close();
}

int graph::root(int x,vector<int> &tati, vector<int> &rang)
{
    int c=x,y;
    while(tati[c]!=c)c=tati[c];
    while(tati[x]!=x)
    {
        y=tati[x];
        tati[x]=c;
        x=y;
    }
    return c;
}
void graph::unite(int x,int y,vector<int> &tati,vector<int> &rang)
{
    if(rang[x]>rang[y])
        tati[y]=x;
    else tati[x]=y;//punem in arborele mai mare arborele mai mic
    if(rang[x]==rang[y])rang[y]++;//daca au aceiasi marime, o incrementez
}
void graph::disjoint_command(ifstream &fin,ofstream &fout,vector<int> &tati,vector<int> &rang)
{
    int c,x,y;
    fin>>c>>x>>y;
    x--;
    y--;
    if(c==2)//daca se cere daca sunt in aceiasi multime
        if(root(x,tati,rang)==root(y,tati,rang))//daca au aceiasi radacina
            fout<<"DA\n";
        else fout<<"NU\n";
    else unite(root(x,tati,rang),root(y,tati,rang),tati,rang);//reunim multimile
}
void graph::print_distance_matrix(string filename)
{
    ofstream fout(filename);
    vector < pair<int,int> >::const_iterator it;
    for(int i=0; i<n; i++)
    {
        for (it = cost_arcs[i].begin(); it != cost_arcs[i].end(); ++ it)
            fout<<(*it).second<<' ';
        fout<<'\n';
    }
    fout.close();
}
void graph::roy_floyd()
{
    for(int k=0; k<n; k++)
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                if (cost_arcs[i][k].second && cost_arcs[k][j].second
                        && (cost_arcs[i][j].second > cost_arcs[i][k].second + cost_arcs[k][j].second
                            || !(cost_arcs[i][j].second)) && i != j)
                    cost_arcs[i][j] = make_pair(j,cost_arcs[i][k].second + cost_arcs[k][j].second);

}
vector<int> graph::bellman_ford()
{
    const int INF = 0x3f3f3f3f;
    vector<int> dist(n);
    dist.assign(n,INF);
    queue<int> q;
    vector<bool> in_queue;
    vector<int> cnt_in_queue;
    cnt_in_queue.assign(n,0);
    in_queue.assign(n,false);
    int x;
    vector < pair <int, int> >::iterator it;
    dist[0]=0;
    q.push(0);
    in_queue[0]=true;
    //initializari
    while(!q.empty() && !this->negative_cycle)
    {
        x=q.front();
        q.pop();
        in_queue[x]=false;
        for (it = cost_arcs[x].begin(); it != cost_arcs[x].end(); ++ it)
            if (dist[x] < INF)
                if(dist[it->first]>dist[x]+it->second)//daca pot optimiza cu arcul curent
                {
                    dist[it->first]=dist[x]+it->second;//optimizez
                    if(!in_queue[it->first]) //daca nodul respectiv nu a fost pus in coada deja
                    {
                        if(cnt_in_queue[it->first]>n)//daca a fost pus in coada de mai mult de n ori
                            negative_cycle=true;//exista un ciclu negativ
                        else
                        {
                            q.push(it->first);
                            in_queue[it->first]=true;//il pun in coada
                            cnt_in_queue[it->first]++;//marchez ca a mai fost pus in coada odata
                        }
                    }
                }
    }
    return dist;
}
vector<int> graph::dijkstra()
{
    const int INF = 0x3f3f3f3f;
    vector<int> dist;
    dist.assign(n,INF);
    dist[0]=0;
    set<pair<int,int> > h;
    h.insert(make_pair(0,0));
    vector<pair<int, int>>::iterator it;
    while(!h.empty())
    {
        int node=h.begin()->second;
        h.erase(h.begin());

        for(it=cost_arcs[node].begin(); it!=cost_arcs[node].end(); it++)
        {
            int to=it->first;
            int cost=it->second;
            if(dist[to]>dist[node]+cost)
            {
                if(dist[to]!=INF)
                {
                    h.erase(h.find(make_pair(dist[to],to)));
                }
                dist[to]=dist[node]+cost;
                h.insert(make_pair(dist[to],to));
            }
        }
    }
    for(vector<int>::iterator i=dist.begin()+1; i!=dist.end(); i++)
        if(*i==INF)
            *i=0;

    return dist;
}
vector< vector<int> > graph::apm()
{
    this->apm_cost=0;
    this->apm_size=0;
    vector<int> tati(n),rang(n);
    rang.assign(n,1);
    for(int i=0; i<n; i++)tati[i]=i;
    vector<vector<int> > apm(n);

    struct triple
    {
        int x,y,c;
    };
    vector<triple> muchii(n);
    triple aux;

    for(int i=0; i<(int)cost_arcs.size(); i++)
        for(int j=0; j<(int)cost_arcs[i].size(); j++)
        {
            aux.x=i;
            aux.y=(cost_arcs[i])[j].first;
            aux.c=(cost_arcs[i])[j].second;
            muchii.push_back(aux);
        }

    //convertesc vectorul de vectori la un format usor sortabil
    sort(muchii.begin(),muchii.end(),[](triple v1,triple v2)->bool{return v1.c<v2.c;});
    for(int i=0; i<(int)muchii.size(); i++)
    {
        if(root(muchii[i].x,tati,rang)!=root(muchii[i].y,tati,rang))
        {
            apm[muchii[i].x].push_back(muchii[i].y);
            this->apm_cost+=muchii[i].c;
            unite(root(muchii[i].x,tati,rang),root(muchii[i].y,tati,rang),tati,rang);
            this->apm_size++;
        }
    }
    return apm;
}
int graph::d_arb()
{
    queue<int> que;
    que.push(0);
    vector<int> dist;
    dist.assign(n,-1);
    int end_of_chain=0;
    int max_chain_length=0;
    dist[0]=0;
    while(!que.empty())
    {
        int current=que.front();//iau elementul curent
        que.pop();//il scot din coada
        for(unsigned int i=0; i<arcs[current].size(); i++) //ii parcurg lista de vecini
        {
            if(dist[arcs[current][i]]==-1)//daca vecinul e nevizitat
            {
                dist[arcs[current][i]]=dist[current]+1;//distanta lui devine cea a tatalui +1
                que.push(arcs[current][i]);//il pun in coada
            }
        }
        end_of_chain=current;
    }
    dist.assign(n,-1);
    dist[end_of_chain]=0;
    que.push(end_of_chain);
    while(!que.empty())
    {
        int current=que.front();//iau elementul curent
        que.pop();//il scot din coada
        for(unsigned int i=0; i<arcs[current].size(); i++) //ii parcurg lista de vecini
        {
            if(dist[arcs[current][i]]==-1)//daca vecinul e nevizitat
            {
                dist[arcs[current][i]]=dist[current]+1;//distanta lui devine cea a tatalui +1
                que.push(arcs[current][i]);//il pun in coada
                max_chain_length=max(max_chain_length,dist[current]+1);
            }
        }
    }
    return max_chain_length+1;

}

int graph::flow_BF(vector<int> &tati,vector<int> &cd,vector<int> &viz,vector< vector<int> > &capacities,vector< vector<int> > &flows)
{
    int nod,V;
    cd[0]=cd[1]=0;
    viz.assign(n,0);
    viz[0]=1;
    for (int i = 0; i <= cd[0]; i++)
    {
        nod = cd[i];
        if (nod == n-1)continue;//daca s-a ajuns la destinatie
        for (int j = 0; j < (int)arcs[nod].size(); j++)
        {
            V = arcs[nod][j];
            if (capacities[nod][V] == flows[nod][V] || viz[V])continue;//daca a mai fost vizitat sau nu mai are capacitate
            viz[V] = 1;
            cd[ ++cd[0] ] = V;
            tati[V] = nod;
        }
    }

    return viz[n-1];//daca s-a ajuns la destinatie
}
int graph::max_flow()
{
    const int INF = 0x3f3f3f3f;
    vector<int> cd,viz,tati;
    cd.assign(n,0);
    viz.assign(n,0);
    tati.assign(n,0);
    vector< vector<int> > capacities(n),flows(n);
    for(auto it=capacities.begin(); it!=capacities.end(); it++)
        (*it).assign(n,0);
    for(auto it=flows.begin(); it!=flows.end(); it++)
        (*it).assign(n,0);
    for(int i=0; i<n; i++)
        for(auto it=cost_arcs[i].begin(); it!=cost_arcs[i].end(); it++)
            capacities[i][(*it).first]=(*it).second;
    //initializez matricea de capacitati din lista de muchii
    int flow=0,fmin,nod;
    //initializari
    while (flow_BF(tati,viz,cd,capacities,flows))//cat timp exista un drum catre destinatie
        for (auto it = arcs[n-1].begin(); it!=arcs[n-1].end(); it++) //verific nodurile ce intra in destinatie
        {
            nod = (*it);
            if (flows[nod][n] == capacities[nod][n] || !viz[nod]) continue;//daca nodul e saturat
            tati[n-1] = nod;//consider nodul curent pe post de tata al destinatiei

            fmin = INF;
            for (nod = n-1; nod != 0; nod = tati[nod])
                fmin = min(fmin, capacities[ tati[nod] ][nod] - flows[ tati[nod] ][nod]);//gasesc bottleneck-ul
            if (fmin == 0) continue;

            for (nod = n-1; nod != 0; nod = tati[nod])
            {
                flows[ tati[nod] ][nod] += fmin;
                flows[nod][ tati[nod] ] -= fmin;
            }//updatez cantitatile trimise pe drumul gasit

            flow += fmin;
        }

    return flow;
}

bool graph::has_euler_cycle()
{
    for(int i=0; i<n; i++)
        if((int)arcs[i].size()%2==1)
            return false;
    return true;
}
vector<int> graph::euler_cycle()
{
    vector<int> cycle;
    stack<int> st;
    st.push(0);
    vector< vector<int> > node_edges(n);
    vector<pair<int,int> > edges;
    int k=0;
    for(int i=0; i<n; i++)
        for(int j=0; j<(int)arcs[i].size(); j++)
        {
            edges.push_back(make_pair(i,arcs[i][j]));
            node_edges[i].push_back(k);
            node_edges[arcs[i][j]].push_back(k++);
        }
    vector<bool> used_edge;
    used_edge.assign(k,false);
    int nod;
    while(!st.empty())
    {
        nod=st.top();
        if(!node_edges[nod].empty())
        {
            int e=node_edges[nod].back();
            node_edges[nod].pop_back();
            if(!used_edge[e])
            {
                used_edge[e]=true;
                st.push(edges[e].first ^ edges[e].second ^ nod);
            }
        }
        else
        {
            st.pop();
            cycle.push_back(nod);
        }
    }

    return cycle;
}

void bfs_main()
{
    int n,m,s;
    ifstream fin("bfs.in");
    fin>>n>>m>>s;
    s--;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0; i<m; i++)
    {
        fin>>a>>b;
        a--;
        b--;
        arce[a].push_back(b);
    }
    graph g(n,m,arce);
    g.bfs(s);

}
void dfs_main()
{
    int n,m;
    ifstream fin("dfs.in");
    fin>>n>>m;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0; i<m; i++)
    {
        fin>>a>>b;
        a--;
        b--;
        arce[a].push_back(b);
        arce[b].push_back(a);
    }
    graph g(n,m,arce);
    ofstream fout("dfs.out");
    fout<<g.componente_conexe();

}
void sort_top_main()
{
    int m,n;
    ifstream fin("sortaret.in");
    fin>>n>>m;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0; i<m; i++)
    {
        fin>>a>>b;
        a--;
        b--;
        arce[a].push_back(b);
        arce[b].push_back(a);
    }
    graph g(n,m,arce);
    g.sortare_top();
}
void ctc_main()
{
    int n,m;
    ifstream fin("ctc.in");
    fin>>n>>m;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0; i<m; i++)
    {
        fin>>a>>b;
        a--;
        b--;
        arce[a].push_back(b);
    }
    graph g(n,m,arce);
    g.comp_tare_conexe();

}
void arce_critice_main()
{
    int n,m;
    ifstream fin("crit_conn.in");
    fin>>n>>m;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0; i<m; i++)
    {
        fin>>a>>b;
        //a--;b--;
        arce[a].push_back(b);
    }
    graph g(n,m,arce);
    g.critical_arcs();

}
void havel_hakimi_main()
{
    int n;
    vector<int> s;
    ifstream fin("HavHak.in");
    fin>>n;
    int l;
    for(int i=0; i<n; i++)
    {
        fin>>l;
        s.push_back(l);
    }
    graph g;
    g=g.Havel_Hakimi(s);
    g.print();

}
void disj_main()
{
    ifstream fin("disjoint.in");
    ofstream fout("disjoint.out");
    int n,m;
    graph g=graph();
    fin>>n>>m;
    vector<int> tati(n),rang(n);
    rang.assign(n,1);
    for(int i=0; i<n; i++)tati[i]=i;
    for(int i=0; i<m; i++)g.disjoint_command(fin,fout,tati,rang);
}
void bellmanford_main()
{
    int n,m;
    ifstream fin("bellmanford.in");
    ofstream fout("bellmanford.out");
    fin>>n>>m;
    vector<vector<pair<int,int> > > cost_arcs(n);
    vector< vector<int>> arcs(n);
    int x,y,c;
    for(int i=0; i<m; i++)
    {
        fin>>x>>y>>c;
        x--;
        y--;
        cost_arcs[x].push_back(make_pair(y,c));
        arcs[x].push_back(y);
    }
    graph g(n,m,arcs,cost_arcs);
    vector<int> out=g.bellman_ford();
    if(g.get_negative_cycle())fout<<"Ciclu negativ!\n";
    else for(int i=1; i<n; i++)
            fout<<out[i]<<' ';
    fout.close();
}
void dijkstra_main()
{
    int n,m;
    ifstream fin("dijkstra.in");
    ofstream fout("dijkstra.out");
    fin>>n>>m;
    vector<vector<pair<int,int> > > cost_arcs(n);
    vector< vector<int>> arcs(n);
    int x,y,c;
    for(int i=0; i<m; i++)
    {
        fin>>x>>y>>c;
        x--;
        y--;
        cost_arcs[x].push_back(make_pair(y,c));
        arcs[x].push_back(y);
    }
    graph g(n,m,arcs,cost_arcs);
    vector<int> out=g.dijkstra();
    for(int i=1; i<n; i++)
        fout<<out[i]<<' ';
    fout.close();

}
void apm_main()
{
    int n,m;
    ifstream fin("apm.in");
    ofstream fout("apm.out");
    fin>>n>>m;
    vector<vector<pair<int,int> > > cost_arcs(n);
    vector< vector<int>> arcs(n);
    int x,y,c;
    for(int i=0; i<m; i++)
    {
        fin>>x>>y>>c;
        x--;
        y--;
        cost_arcs[x].push_back(make_pair(y,c));
        arcs[x].push_back(y);
    }
    graph g(n,m,arcs,cost_arcs);
    vector<vector<int>> out =g.apm();
    fout<<g.get_apm_cost()<<'\n'<<g.get_apm_size()<<'\n';
    for(int i=0; i<(int)out.size(); i++)
        for(auto it=out[i].begin(); it!=out[i].end(); it++)
            fout<<i+1<<' '<<(*it)+1<< '\n';
    fout.close();

}
void roy_floyd_main()
{
    int n;
    ifstream fin("royfloyd.in");
    fin>>n;
    vector<vector<pair<int,int> > > cost_arcs(n);
    vector<vector<int> > arcs(n);

    int c;
    for(int x=0; x<n; x++)
    {
        for(int y=0; y<n; y++)
        {
            fin>>c;
            arcs[x].push_back(y);
            cost_arcs[x].push_back(make_pair(y,c));

        }
    }
    graph g(n,n,arcs,cost_arcs);
    g.roy_floyd();
    g.print_distance_matrix("royfloyd.out");
    fin.close();
}
void darb_main()
{
    int n;
    ifstream fin("darb.in");
    ofstream fout("darb.out");
    fin>>n;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0; i<n-1; i++)
    {
        fin>>a>>b;
        a--;
        b--;
        arce[a].push_back(b);
        arce[b].push_back(a);
    }
    graph g(n,n-1,arce);
    fout<<g.d_arb();
}
void maxflow_main()
{
    int n,m;
    ifstream fin("maxflow.in");
    ofstream fout("maxflow.out");
    fin>>n>>m;
    vector<vector<pair<int,int> > > cost_arcs(n);
    vector< vector<int>> arcs(n);
    int x,y,c;
    for(int i=0; i<m; i++)
    {
        fin>>x>>y>>c;
        x--;
        y--;
        cost_arcs[x].push_back(make_pair(y,c));
        arcs[x].push_back(y);
        arcs[y].push_back(x);
    }
    graph g(n,m,arcs,cost_arcs);
    fout<<g.max_flow();
    fin.close();
    fout.close();
}
void euler_main()
{
    ifstream fin("ciclueuler.in");
    ofstream fout("ciclueuler.out");
    int n, m, x, y;
    fin >> n >> m;
    vector<vector<int>> arcs(n);
    for(int i=0; i<m; i++)
    {
        fin>>x>>y;
        x--;
        y--;
        arcs[x].push_back(y);
    }
    graph g(n,m,arcs);
    if(!g.has_euler_cycle())
        fout<<"-1\n";
    else
    {
        vector<int> out = g.euler_cycle();
        for(auto it=out.begin(); it!=out.end()-1; it++)
            fout<<*it+1<<' ';
    }
    fin.close();
    fout.close();
}
int main()
{
    euler_main();
    return 0;
}
