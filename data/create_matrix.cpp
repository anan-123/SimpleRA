#include<bits/stdc++.h>
#define ll long long int
using namespace std;


int main(){
    int n = 17;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
	    cout<<n*i+j+1;
	    if(j < n-1){
		    cout<<", ";
	    }
	}
	cout<<endl;
    }
}
