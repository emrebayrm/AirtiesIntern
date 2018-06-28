#include <bits/stdc++.h>
using namespace std;

const int MAX=5;
const int INF=899999;
int M[MAX][MAX];

void actualize(int x, int y){
    int minimum=INF;
    for(int k=1;k<x;k++){
        minimum=min(minimum,M[k][y]+M[x-k][y]+1);
    }
    for(int k=1;k<y;k++){
        minimum=min(minimum,M[x][k]+M[x][y-k]+1);
    }
    M[x][y]=minimum;
}

int main(){
    for(int i=1;i<MAX;i++){
        for(int j=1;j<MAX;j++){
            if(i==1 && j==1) continue;
            actualize(i,j);
        }
    }
    int m,n;
    cin >> m >> n;
    cout << M[m][n] << endl;
}