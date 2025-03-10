#include<iostream>
using namespace std;

int main(){
	freopen("Machine_language.txt","rb",stdin);
	freopen("Binary.txt","wb",stdout);
	while(true){
		string line;
		getline(cin,line);
		int n=line.size();
		if(!n) break;
		else if(line=="Binary:-") continue;
		
		if(line[4]=='E') continue; // Skip if the line results was an error
		for(int i=4;i<n;i++) if(line[i]!=' ') cout<<line[i];
		cout<<"\n";
	}
}
