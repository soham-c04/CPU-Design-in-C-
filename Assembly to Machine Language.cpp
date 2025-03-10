#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
#define f(n) for(int i=0;i<(n);i++)
#define pb push_back

/*
README:
	To compare hexadecimal versions with my code use website: 
	https://riscvasm.lucasteske.dev/#  (Whole code conversion. But there are discrepancies in the branch and jump of this website)
	https://luplab.gitlab.io/rvcodecjs/  (Single instruction conversion)
	
	Check the Assembly.txt file first to understand possible input methods
	
	Press ENTER 3 times to exit processing the input lines.
*/

string error(int x){
	string s="Error: ";
	if(x==1) s+="Invalid immediate value";
	else if(x==5) s+="Invalid offset";
	else if(x==2) s+="Register not found";
	else if(x==3) s+="Excess information at the end of instruction";
	else if(x==6) s+="Undefined reference to the label";
	else s+="Instruction not found";
	return s;
}
string binary(string s,int bit){
	if(s.size()==0 || s.size()>9) return "";
	int i=0;
	if(s[i]=='-') i++;
	for(;i<s.size();i++) if(s[i]>'9' || s[i]<'0') return "";
	int num=stoi(s);
	if(bit>5 && (num>=(1<<(bit-1)) || num<(-(1<<(bit-1))))) return "";
	if(num<0) num+=1<<bit;
	string bin="";
	for(int i=bit-1;i>=0;i--) bin.pb('0'+((num>>i)&1));
	return bin;
}
// Contains binary of registers
class reg{
	map<string,string> Reg;
	public:
		void initialize(){
		    f(32){
		        string bin="",s="x";
		        s+=to_string(i);
		        for(int j=4;j>=0;j--) bin.pb('0'+((i>>j)&1));
		        Reg[s]=bin;
		    }
		    int cnt=0;
		    Reg["zero"]=binary(to_string(cnt++),5);
		    Reg["ra"] = binary(to_string(cnt++),5);
		    Reg["sp"] = binary(to_string(cnt++),5);
		    Reg["gp"] = binary(to_string(cnt++),5);
		    Reg["tp"] = binary(to_string(cnt++),5);
		    Reg["t0"] = binary(to_string(cnt++),5);
		    Reg["t1"] = binary(to_string(cnt++),5);
		    Reg["t2"] = binary(to_string(cnt++),5);
		    Reg["s0"] = binary(to_string(cnt++),5);
		    Reg["s1"] = binary(to_string(cnt++),5);
		    Reg["a0"] = binary(to_string(cnt++),5);
		    Reg["a1"] = binary(to_string(cnt++),5);
		    Reg["a2"] = binary(to_string(cnt++),5);
		    Reg["a3"] = binary(to_string(cnt++),5);
		    Reg["a4"] = binary(to_string(cnt++),5);
		    Reg["a5"] = binary(to_string(cnt++),5);
		    Reg["a6"] = binary(to_string(cnt++),5);
		    Reg["a7"] = binary(to_string(cnt++),5);
		    Reg["s2"] = binary(to_string(cnt++),5);
		    Reg["s3"] = binary(to_string(cnt++),5);
		    Reg["s4"] = binary(to_string(cnt++),5);
		    Reg["s5"] = binary(to_string(cnt++),5);
		    Reg["s6"] = binary(to_string(cnt++),5);
		    Reg["s7"] = binary(to_string(cnt++),5);
		    Reg["s8"] = binary(to_string(cnt++),5);
		    Reg["s9"] = binary(to_string(cnt++),5);
		    Reg["s10"]= binary(to_string(cnt++),5); 
		    Reg["s11"]= binary(to_string(cnt++),5);
		    Reg["t3"] = binary(to_string(cnt++),5);
		    Reg["t4"] = binary(to_string(cnt++),5);
		    Reg["t5"] = binary(to_string(cnt++),5);
		    Reg["t6"] = binary(to_string(cnt++),5);
		}
		string get(string s){
			if(Reg.find(s)==Reg.end()) return "";
			else return Reg[s];
		}
}Register;
//Contains the type of instruction
class type{
	map<string,char> func;
	public:
		void initialize(){
		    func["lui"] = 'U';
		    func["auipc"]='U';
			func["jal"] = 'J';
			func["lb"]  = 'I';
			func["lh"]  = 'I';
			func["lw"]  = 'I';
			func["lbu"] = 'I';
			func["lhu"] = 'I';
			func["addi"]= 'I';
			func["slti"]= 'I';
			func["sltiu"]='I';
			func["slli"]= 'I';
			func["xori"]= 'I';
			func["srli"]= 'I';
			func["srai"]= 'I';
			func["ori"] = 'I';
			func["andi"]= 'I';
			func["jalr"]= 'I';
			func["add"] = 'R'; 
			func["sub"] = 'R'; 
			func["sll"] = 'R'; 
			func["xor"] = 'R'; 
			func["srl"] = 'R'; 
			func["sra"] = 'R';  
			func["or"]  = 'R';  
			func["and"] = 'R';
			func["slt"] = 'R';
			func["sltu"]= 'R'; 
			func["sb"]  = 'S'; 
			func["sh"]  = 'S'; 
			func["sw"]  = 'S'; 
			func["beq"] = 'B'; 
			func["bne"] = 'B';
			func["blt"] = 'B';
			func["bge"] = 'B';
			func["bltu"]= 'B'; 
			func["bgeu"]= 'B';
		}
		char get(string s){
			if(func.find(s)==func.end()) return '0';
			else return func[s];
		}
}Type;
// U-type
class U_type{
	map<string,string> opcode;
	public:
		void initialize(){
			opcode["lui"]  = "0110111";
			opcode["auipc"]= "0010111";
		}
		string get(string fun,string rd,string imm){
			string s1=binary(imm,20),s2=Register.get(rd),s3=opcode[fun];
			if(s1=="") return error(1);
			if(s2=="") return error(2);
			return s1+" "+s2+" "+s3;
		}
}U;
// UJ-type
class UJ_type{
	map<string,string> opcode;
	public:
		void initialize(){
			opcode["jal"]="1101111";
		}
		string get(string fun,string rd,string imm){
			string s=binary(imm,21);
			if(s=="") return error(1);
			string s1="",s2="",s3="",s4="";
			s1.pb(s[20-20]);
			for(int i=10;i>=1;i--) s2.pb(s[20-i]);
			s3.pb(s[20-11]);
			for(int i=19;i>=12;i--) s4.pb(s[20-i]);
			string s5=Register.get(rd),s6=opcode[fun];
			if(s5=="") return error(2);
			return s1+" "+s2+" "+s3+" "+s4+" "+s5+" "+s6;
		}
}UJ;
// I-type
class I_type{
	map<string,string> opcode,funct3;
	public:
		void initialize(){
			opcode["lb"] = "0000011"; funct3["lb"] = "000";
			opcode["lh"] = "0000011"; funct3["lh"] = "001";
			opcode["lw"] = "0000011"; funct3["lw"] = "010";
			opcode["lbu"]= "0000011"; funct3["lbu"]= "100";
			opcode["lhu"]= "0000011"; funct3["lhu"]= "101";
			
			opcode["addi"]="0010011"; funct3["addi"]="000";
			opcode["slti"]="0010011"; funct3["slti"]="010";
			opcode["sltiu"]="0010011";funct3["sltiu"]="011";
			opcode["slli"]="0010011"; funct3["slli"]="001";
			opcode["xori"]="0010011"; funct3["xori"]="100";
			opcode["srli"]="0010011"; funct3["srli"]="101";
			opcode["srai"]="0010011"; funct3["srai"]="101";
			opcode["ori"] ="0010011"; funct3["ori"] ="110";
			opcode["andi"]="0010011"; funct3["andi"]="111";
			opcode["jalr"]="1100111"; funct3["jalr"]="000";
		}
		string get(string fun,string rd,string rs1,string imm){
			string funct6="";
			if(fun=="srai") funct6="0100000";
			else if(fun=="slli" || fun=="srli") funct6="0000000";
			int bit=12-funct6.size();
			string s1=binary(imm,bit),s2=Register.get(rs1),s3=funct3[fun],s4=Register.get(rd),s5=opcode[fun];
			if(s1=="") return error(1);
			if(s2=="" || s4=="") return error(2);
			return funct6+" "+s1+" "+s2+" "+s3+" "+s4+" "+s5;
		}
}I;
// R-type
class R_type{
	map<string,string> opcode,funct3,funct7;
	public:
		void initialize(){
			opcode["add"]= "0110011"; funct3["add"]= "000"; funct7["add"]= "0000000";
			opcode["sub"]= "0110011"; funct3["sub"]= "000"; funct7["sub"]= "0100000";
			opcode["sll"]= "0110011"; funct3["sll"]= "001"; funct7["sll"]= "0000000";
			opcode["slt"]= "0110011"; funct3["slt"]= "010"; funct7["slt"]= "0000000";
			opcode["sltu"]="0110011"; funct3["sltu"]="011"; funct7["sltu"]="0000000";
			opcode["xor"]= "0110011"; funct3["xor"]= "100"; funct7["xor"]= "0000000";
			opcode["srl"]= "0110011"; funct3["srl"]= "101"; funct7["srl"]= "0000000";
			opcode["sra"]= "0110011"; funct3["sra"]= "101"; funct7["sra"]= "0100000";
			opcode["or"] = "0110011"; funct3["or"] = "110"; funct7["or"] = "0000000";
			opcode["and"]= "0110011"; funct3["and"]= "111"; funct7["and"]= "0000000";
		}
		string get(string fun,string rd,string rs1,string rs2){
			string s0=funct7[fun],s1=Register.get(rs2),s2=Register.get(rs1),s3=funct3[fun],s4=Register.get(rd),s5=opcode[fun];
			if(s1=="" || s2=="" || s4=="") return error(2);
			return s0+" "+s1+" "+s2+" "+s3+" "+s4+" "+s5;
		}
}R;
// S-type
class S_type{
	map<string,string> opcode,funct3;
	public:
		void initialize(){
			opcode["sb"] = "0100011"; funct3["sb"] = "000";
			opcode["sh"] = "0100011"; funct3["sh"] = "001";
			opcode["sw"] = "0100011"; funct3["sw"] = "010";
		}
		string get(string fun,string rs2,string imm,string rs1){
			string s=binary(imm,12);
			if(s=="") return error(5);
			string s1="",s5="";
			f(12) ((i<=6)? s1.pb(s[i]):s5.pb(s[i]));
			string s2=Register.get(rs2),s3=Register.get(rs1),s4=funct3[fun],s6=opcode[fun];
			if(s2=="" || s3=="") return error(2);
			return s1+" "+s2+" "+s3+" "+s4+" "+s5+" "+s6;
		}
}S;
// SB-type
class SB_type{
	map<string,string> opcode,funct3;
	public:
		void initialize(){
			opcode["beq"] = "1100011"; funct3["beq"] = "000";
			opcode["bne"] = "1100011"; funct3["bne"] = "001";
			opcode["blt"] = "1100011"; funct3["blt"] = "100";
			opcode["bge"] = "1100011"; funct3["bge"] = "101";
			opcode["bltu"]= "1100011"; funct3["bltu"]= "110";
			opcode["bgeu"]= "1100011"; funct3["bgeu"]= "111";
		}
		string get(string fun,string rs1,string rs2,string imm){
			string s=binary(imm,13);
			if(s=="") return error(1);
			string s1="",s2="",s6="",s7="";
			s1.pb(s[12-12]);
			for(int i=10;i>=5;i--) s2.pb(s[12-i]);
			for(int i=4;i>=1;i--) s6.pb(s[12-i]);
			s7.pb(s[12-11]);
			string s3=Register.get(rs2),s4=Register.get(rs1),s5=funct3[fun],s8=opcode[fun];
			if(s2=="" || s3=="") return error(2);
			return s1+" "+s2+" "+s3+" "+s4+" "+s5+" "+s6+" "+s7+" "+s8;
		}
}SB;
int main(){
	freopen("Assembly.txt","rb",stdin);
	freopen("Machine_language.txt","wb",stdout);
	vector<pair<int,string>> input;
	vector<string> ans;
	
	Register.initialize();
	Type.initialize(); // UJ-types are abbreviated to J and SB-types to B for ease of coding
	
	U.initialize();
	UJ.initialize();
	I.initialize();
	R.initialize();
	S.initialize();
	SB.initialize();
    
	int strike=0,line_no=0,location=0;
	map<string,int> address;
	while(true){
		line_no++;
		string line;
		getline(cin,line);
		if(line.size()) line.pop_back();
		
		if(line=="") strike++;
		else strike=0;
		int i=0,full=line.size();
		
		auto gap=[&](){
			while(i<full && line[i]==' ') i++;
			if(i<full && line[i]=='#') i=full; 
		};
		auto split=[&](){
			string str="";
			gap();
			while(i<full && line[i]!=' ') str.pb(line[i++]);
			gap();
			return str;
		};
		string fun=split();
		char c=Type.get(fun);
		// Call required type
		if(c!='0') input.pb({line_no,line});
		else if(strike==3) break;
		else if(fun=="") continue;
		else if(fun.back()==':' && i==full){
			fun.pop_back();
			address[fun]=location;
			continue;
		}
		else{
			input.pb({line_no,line});
			continue;
		}
		location+=4;
	}
	location=0;
	for(auto p:input){
		line_no=p.first;string line=p.second;
		int i=0,full=line.size();
		auto gap=[&](){
			while(i<full && line[i]==' ') i++;
			if(i<full && line[i]=='#') i=full; 
		};
		auto split=[&](char c=' '){
			string str="";
			gap();
			while(i<full && line[i]!=' ' && line[i]!=c) str.pb(line[i++]);
			gap();
			if(c!='~') f(str.size()) if(str[i]>='A' && str[i]<='Z') str[i]|=32;
			if(c!=' ' && c!='~'){
				if(i==full || line[i]!=c) return string("");
				i++;
				gap();
			}
			return str;
		};
		// Find the instruction
		string fun=split();
		// Binary code for the given line
		string output=to_string(line_no)+":  ";
		char c=Type.get(fun);
		// Call required type
		if(c=='U'){
			string rd=split(','),imm=split();
			if(i<full) output+=error(3);
			else output+=U.get(fun,rd,imm);
		}
		else if(c=='J'){
			string rd=split(','),imm=split('~');
			if(i<full) output+=error(3);
			else if(binary(imm,20)!="") output+=UJ.get(fun,rd,imm);
			else{ // Case if label is given
				if(address.find(imm)==address.end()) output+=error(6);
				else{
					imm=to_string(address[imm]-location);
					output+=UJ.get(fun,rd,imm);
				}
			}
		}
		else if(c=='I'){
			string rd=split(','),rs1,imm;
			if(fun[0]=='l' || fun=="jalr"){ // Load instructions
				imm=split('(');rs1=split(')');
			}
			else{ // Normal I types
				rs1=split(',');imm=split();
			}
			if(i<full) output+=error(3);
			else output+=I.get(fun,rd,rs1,imm);
		}
		else if(c=='R'){
			string rd=split(','),rs1=split(','),rs2=split();
			if(i<full) output+=error(3);
			else output+=R.get(fun,rd,rs1,rs2);
		} 
		else if(c=='S'){
			string rs2=split(','),imm=split('('),rs1=split(')');
			if(i<full) output+=error(3);
			else output+=S.get(fun,rs2,imm,rs1);
		}
		else if(c=='B'){
			string rs1=split(','),rs2=split(','),imm=split('~');
			if(i<full) output+=error(3);
			else if(binary(imm,12)!="") output+=SB.get(fun,rs1,rs2,imm);
			else{ // Case if label is given
				if(address.find(imm)==address.end()) output+=error(6);
				else{
					imm=to_string(address[imm]-location);
					output+=SB.get(fun,rs1,rs2,imm);
				}
			}	
		}
		else if(fun=="") continue;
		else if(fun.back()==':' && i==full) continue;
		else{
			output+=error(4);
			location-=4;
		}
		location+=4;
		ans.pb(output);
	}
	
	cout<<"Binary:-\n";
	for(string s:ans) cout<<s<<"\n";
	cout<<"\n";
	cout<<"Hexadecimal:-\n";
	for(string ss:ans){
		string final="",s="";
		bool dot=false;
		string line_number="";
		for(char c:ss){
			if(!dot) line_number.pb(c);
			if(c==':') dot=true;
			else if(dot && c!=' ') s.pb(c);
		}
		if(s=="" || s[0]=='E') continue;
		while(s.size()>=4){
			int num=0;
			f(4) num+=(1<<i)*(s[s.size()-1-i]-'0');
			f(4) s.pop_back();
			if(num<=9) final.pb(num+'0');
			else final.pb(num-10+'a');
		}
		reverse(final.begin(),final.end());
		cout<<line_number<<" "<<final<<"\n";
	}
	return 0;
}
