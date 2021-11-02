#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

    bool compare(pair<int,int> v1,pair<int,int> v2)
    {
        return v1.second>v2.second;
    }

class graph{
int n,m;
vector<vector<int>> arcs;
void sort_dfs(int start,vector<bool> &viz,queue<int> &coada);

public:
    graph(int n,int m,vector< vector<int>> arcs);
    void bfs(int start);
    void dfs(int start,vector<bool> &viz);
    void componente_conexe();
    void sortare_top();
    graph Havel_Hakimi(vector<int> s);
    void comp_tare_conexe();
};

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
    vector<pair<int,int>> v;
    for(int i=0;i<n;i++)
        v.push_back(make_pair(i,s[i]));//first - indicele second - gradul
    sort(v.begin(),v.end(),[](pair<int,int> v1,pair<int,int> v2)->bool{return v1.second<v2.second;});//sortez dupa grade, in ordine inversa
    bool done=false;
    while(!done)
    {
        if(v[0].second>0)
        for(int i=1;i<v[0].second;i++)//de la urmatorul, pana epuizam gradul nodului de pe pozitia 0
            {
                m++;
                arcs[v[0].first].push_back(v[i].first);//pun arc intre noduri
                v[i].second--;//scad gradul si nodului pus
            }
        else done=true;
        v[0].second=0;
        sort(v.begin(),v.end(),[](pair<int,int> v1,pair<int,int> v2)->bool{return v1.second<v2.second;});//sortez dupa grade, in ordine inversa
    }
    return graph(n,m,arcs);
    //NOT TESTED
}
void graph::comp_tare_conexe()
{
    int s=0;

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
int main()
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
    return 0;
}
