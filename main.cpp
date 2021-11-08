#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;



class graph{
int n,m;
vector<vector<int>> arcs;
void sort_dfs(int start,vector<bool> &viz,queue<int> &coada);
vector< vector<int> > C;//ctc-urile
vector<int> idx,lowlink;//indecsii nodurilor si indexul minim ce poate fi atins dintr-un dfs Tarjan
vector<bool> in_stack;//daca nodul se afla in stiva pt Tarjan recursiv
stack <int> S;//stiva pt Tarjan
int index;
void tarjan(int n);
void critical_arcs_dfs(int n,ostream &fout);
public:
    void print();
    graph(){this->n=0;this->m=0;}
    graph(int n,int m,vector< vector<int>> arcs);
    void bfs(int start);
    void dfs(int start,vector<bool> &viz);
    void componente_conexe();
    void sortare_top();
    graph Havel_Hakimi(vector<int> s);
    void comp_tare_conexe();
    void critical_arcs();
};
void graph::print()
{
    ofstream fout("graph.out");
    vector <int>::const_iterator it;
    for(int i=0;i<n;i++)
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
void graph::bfs(int start)
{
    queue<int> que;
    que.push(start);
    int *dist=new int[this->n];
    for(int i=0;i<n;i++)dist[i]=-1;//initializez distantele cu -1
    dist[start]=0;//distanta startului e 0
    while(!que.empty())//cat timp mai am in coada
    {
        int current=que.front();//iau elementul curent
        que.pop();//il scot din coada
        for(unsigned int i=0;i<arcs[current].size();i++)//ii parcurg lista de vecini
        {
            if(dist[arcs[current][i]]==-1)//daca vecinul e nevizitat
            {
                dist[arcs[current][i]]=dist[current]+1;//distanta lui devine cea a tatalui +1
                que.push(arcs[current][i]);//il pun in coada
            }
        }
    }
    ofstream fout("bfs.out");
    for(int i=0;i<this->n;i++)
        fout<<dist[i]<<' ';//afisez distantele
    delete[] dist;//sterg vectorul dinamic
}
void graph::dfs(int start,vector<bool> &viz)
{
    viz[start]=true;
        for(unsigned int i=0;i<arcs[start].size();i++)//ii parcurg lista de vecini
        {
            if(!viz[arcs[start][i]])//daca e nevizitat
            {
                viz[arcs[start][i]]=true;//il vizitez
                this->dfs(arcs[start][i],viz);//apelez recursiv dfs pe el
            }
        }
}
void graph::componente_conexe()
{
    vector<bool> viz(n);
    for(int i=0;i<n;i++)viz[i]=false;//initializez cu fals vectorul
    int componente=0;//numarul de componente conexe
    for(int i=0;i<n;i++)//parcurg toate nodurile
        if(!viz[i])//daca nu e deja vizitat
        {
            dfs(i,viz);//incep un dfs din el
            componente++;//si am inca o componenta conexa
        }
    ofstream fout("dfs.out");
    fout<<componente;

}
void graph::sort_dfs(int start, vector<bool> &viz,queue<int> &coada)
{
    viz[start]=true;
        for(unsigned int i=0;i<arcs[start].size();i++)//ii parcurg lista de vecini
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
    for(int i=0;i<n;i++)viz[i]=false;//initializez cu fals vectorul
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
    for(int i=0;i<n;i++)
        v.push_back(make_pair(i,s[i]));//first - indicele second - gradul
    sort(v.begin(),v.end(),[](pair<int,int> v1,pair<int,int> v2)->bool{return v1.second>v2.second;});//sortez dupa grade, in ordine inversa
    bool done=false;
    while(!done)
    {
        if(v[0].second>0)
        {
        for(int i=1;i<=v[0].second;i++)//de la urmatorul, pana epuizam gradul nodului de pe pozitia 0
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
    lowlink.resize(n);//initializam marimea lui lowlink
    idx.assign(n, -1);
    in_stack.assign(n, 0);
    //initializam idx si in_stack cu valorile de inceput
    for (int i = 0; i < n; ++ i)
        if (idx[i] == -1)
            tarjan(i);//apelam pentru fiecare nod nevizitat

    ofstream fout("ctc.out");
    fout<<(int)C.size()<<'\n';//numarul de linii din C este numarul de ctc
    for(int i=0;i<(int)C.size();i++)
    {
        for(int j=0;j<(int)C[i].size();j++)
            fout<<C[i][j]+1<<' ';
        fout<<'\n';
    }//afisez fiecare componenta in parte
    fout.close();
}
void graph::tarjan(int n)
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
                tarjan(*it);//apelez recursiv
                lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca apelul recursiv a generat unul mai mic
            }
        else if (in_stack[*it])//daca e deja in stiva
            lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca este cazul
    }
    if (idx[n] == lowlink[n])//daca si-a pastrat lowlink-ul dupa parcurgere nu mai avem unde merge si afisam
        {
        vector<int> con;//ctc curenta
        int node;
        do {
            node = S.top();
            con.push_back(node);
            S.pop();
            in_stack[node] = false;
        }
        while (node != n);//dump la stiva in ctc curenta
        C.push_back(con);//pun ctc curenta in matricea de output
    }

}
void graph::critical_arcs_dfs(int n,ostream &fout)
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
                critical_arcs_dfs(*it,fout);//apelez recursiv
                lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca apelul recursiv a generat unul mai mic
            }
        else if (in_stack[*it])//daca e deja in stiva
            lowlink[n] = min(lowlink[n], lowlink[*it]);//updatez minimul daca este cazul

        if(lowlink[*it]>idx[n]) fout<<n<<' '<<*it;
    }

}
void graph::critical_arcs()
{
    ofstream fout("crit_conn.out");
    lowlink.resize(n);//initializam marimea lui lowlink
    idx.assign(n, -1);
    in_stack.assign(n, 0);
    //initializam idx si in_stack cu valorile de inceput
    for (int i = 0; i < n; ++ i)
        if (idx[i] == -1)
            critical_arcs_dfs(i,fout);//apelam pentru fiecare nod nevizitat
    fout.close();
}
void bfs_main()
{
    int n,m,s;
    ifstream fin("bfs.in");
    fin>>n>>m>>s;
    s--;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0;i<m;i++)
    {
        fin>>a>>b;
        a--;b--;
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
    for(int i=0;i<m;i++)
    {
        fin>>a>>b;
        a--;b--;
        arce[a].push_back(b);
        arce[b].push_back(a);
    }
    graph g(n,m,arce);
    g.componente_conexe();

}
void sort_top_main()
{
        int m,n;
        ifstream fin("sortaret.in");
    fin>>n>>m;
    vector<vector<int>> arce(n);
    int a,b;
    for(int i=0;i<m;i++)
    {
        fin>>a>>b;
        a--;b--;
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
    for(int i=0;i<m;i++)
    {
        fin>>a>>b;
        a--;b--;
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
    for(int i=0;i<m;i++)
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
    for(int i=0;i<n;i++)
        {fin>>l;s.push_back(l);}
    graph g;
    g=g.Havel_Hakimi(s);
    g.print();

}
int main()
{
    return 0;
}
