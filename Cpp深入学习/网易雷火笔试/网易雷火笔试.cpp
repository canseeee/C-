#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;



int main(int argc, char* argv[])
{
    int n,m,t,ans=0;
    cin>>n>>m>>t;
    // 这个是所有的名字
    vector<string> names(n);
    vector<int> vv(n*2,0);
    while(n--)
    {
        cin>>names[n];
    }

    // 这个是所有的队伍
    // 定义<string,int>，int表示队伍，可以让不同键的string对应到不同的队伍，相同键的string对应到同一个队伍。
    unordered_map<string,int> v1;
    // 队伍编号到队伍人数的映射

    //vector<pair<string,int>> v1;
    int mm=m;
    while(m--)
    {
        int a; // 这个是每个队伍的人数
        string name; // 记录输入名字
        cin>>a;
        vv[m]=a;
        while(a--)
        {
            cin>>name;
            // m是队伍的编号，把每个人的名字对应到队伍的编号
            v1[name]=m;
            //(pair<string,int>(name,m));
        }
    }

    for (int i=0;i<names.size();i++) //auto it=v1.begin();it!=v1.end();it++
    {
        if (v1.find(names[i])==v1.end())
        {
            v1[names[i]]=i+mm;
            vv[i+mm]=1;
        }
    } 
    // 记录被点到的名字的相应队伍编号
    //vector<int> v2(mm+1,0); //如果被点到了，判断该编号是否已存在，如果不存在就加入到数组中，如果存在就不加入到数组中。

    while(t--)
    {
        string name;
        cin>>name;
        // 找到name对应的int队伍编号，把int加入到数组中
        auto it=v1.find(name);
        //cout<<"it="<<it->second<<endl;
        //cout<<"v2[it->second]="<<v2[it->second]<<endl;
        ans+=vv[it->second];
        // if (v2[it->second]==0)
        // {

        //     ans+=vv[it->second];
            
        //     /*//找出v1所有值为it->second的元素的个数，并把所有个数加到ans中
        //     for (auto it1=v1.begin();it1!=v1.end();it1++)
        //     {
        //         if (it1->second==it->second)
        //         {
        //             ans++;
        //         }
        //     }*/
        // }
        
        
    }
    cout<<ans<<endl;
    system("pause");
    
    return 0;
}
/*4 3 2
a
b
c
d
2
a b
1
c
1
d
a
c*/
//3