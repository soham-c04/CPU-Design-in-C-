#include <iostream>
#include <vector>
#include <map>
using namespace std;
#define f(n) for(int i=0;i<(n);i++)
#define pb push_back
#define F first
#define S second

int previous=-1;
bool stall=false,op_forward=false;
vector<string> IM;
int GPR[32]={0};
map<int,int> DM;
int PC=0;
int binary_to_int(string bin){
    int decimal=0,n=bin.size();
    f(n) decimal|=(bin[i]-'0')<<(n-1-i);
    return decimal;
}
int signedExtend(int val,int bit){
    return (val^(1<<(bit-1)))-(1<<(bit-1));
}
int ImmGen(string instruction,bool change,int type){
	string s="";
	if(type==1){
		for(int i=31;i>=25;i--) s.pb(instruction[31-i]);
        for(int i=11;i>=7;i--) s.pb(instruction[31-i]);
		// Fixing imm for Branch and Jump
		if(change){
			s="";
			s.pb(instruction[31-31]);
			s.pb(instruction[31-7]);
			for(int i=30;i>=25;i--) s.pb(instruction[31-i]);
			for(int i=11;i>=8;i--) s.pb(instruction[31-i]);
		}
	}
	else{
		for(int i=31;i>=20;i--) s.pb(instruction[31-i]);
		if(change){
			s="";
			s.pb(instruction[31-31]);
			for(int i=19;i>=12;i--) s.pb(instruction[31-i]);
			s.pb(instruction[31-20]);
			for(int i=30;i>=21;i--) s.pb(instruction[31-i]);
		}
	}
	int imm=binary_to_int(s);
	imm=signedExtend(imm,s.size());
	return imm;
}
class type{
    public:
        map<string,char> func;
        map<string,string> opcode;
        void initialize(){
            func["lui"] = 'U';
            func["auipc"]='U';
            func["jal"] = 'J';
            func["lb"]  = 'L';
            func["lh"]  = 'L';
            func["lw"]  = 'L';
            func["lbu"] = 'L';
            func["lhu"] = 'L';
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
            
            opcode["lui"]="0110111";
            opcode["auipc"]="0010111";
            
            opcode["jal"]="1101111";
            
            opcode["lb"]  = "0000011"; 
            opcode["lh"]  = "0000011"; 
            opcode["lw"]  = "0000011";
            opcode["lbu"] = "0000011";
            opcode["lhu"] = "0000011"; 
            
            opcode["addi"]= "0010011"; 
            opcode["slti"]= "0010011"; 
            opcode["sltiu"]="0010011"; 
            opcode["slli"]= "0010011"; 
            opcode["xori"]= "0010011"; 
            opcode["srli"]= "0010011"; 
            opcode["srai"]= "0010011"; 
            opcode["ori"] = "0010011";
            opcode["andi"]= "0010011"; 
            opcode["jalr"]= "1100111"; 
            
            opcode["add"] = "0110011";
            opcode["sub"] = "0110011"; 
            opcode["sll"] = "0110011"; 
            opcode["slt"] = "0110011"; 
            opcode["sltu"]= "0110011";
            opcode["xor"] = "0110011"; 
            opcode["srl"] = "0110011"; 
            opcode["sra"] = "0110011";
            opcode["or"]  = "0110011"; 
            opcode["and"] = "0110011"; 
            
            opcode["sb"] = "0100011";
            opcode["sh"] = "0100011";
            opcode["sw"] = "0100011"; 
            
            opcode["beq"] = "1100011"; 
            opcode["bne"] = "1100011"; 
            opcode["blt"] = "1100011";
            opcode["bge"] = "1100011"; 
            opcode["bltu"]= "1100011"; 
            opcode["bgeu"]= "1100011"; 
        }
        char get(string s){
            for(auto i:opcode) if(i.S==s) return func[i.F];
            return 'N';
        }
}Type;
struct ControlWord{	
	bool ALU_src,Mem2Reg,RegRead,Reg_write,Mem_read,Mem_write;
    bool Branch,Jump;
    string ALU_op;
};
ControlWord controller(string opcode){
	struct ControlWord cw;
	char c=Type.get(opcode);
	cw.ALU_src=(c=='U' || c=='I' || c=='L' || c=='S'); cw.Mem2Reg=(c=='L'); cw.Reg_write=(c!='B' && c!='S' && c!='N'); cw.Mem_read=(c=='L');
    cw.Mem_write=(c=='S'); cw.Branch=(c=='B');
    cw.RegRead=true; cw.Jump=false;
    // Store in ALU_src ...
    if(c=='U' || c=='J' || c=='N') cw.RegRead=false; // U and J - type, NOP
    if(opcode=="1101111" || opcode=="1100111") cw.Jump=true; // jal and jalr
    switch(c){
        case 'B':
            cw.ALU_op="01";
            break;
        case 'R':
            cw.ALU_op="10";
            break;
        case 'I':
            cw.ALU_op="11";
            break;
        default:
            cw.ALU_op="00";
            break;
    }
    return cw;
}
string ALUControl(string op,string f3,string f7){
    string s="";
    if(op=="00") s="0010";
    else if(op=="01") s="0110";
    else if(op=="10"){
        if(f3=="000"){
            if(f7=="0000000") s="0010";
            else s="0110";
        }
        else if(f3=="110") s="0001";
        else if(f3=="111") s="0000";
        else if(f3=="100") s="0011";
        else if(f3=="001") s="1101";
    }
    else{
        if(f3=="000"){
            if(op=="11" || f7=="0000000") s="0010";
            else s="0110";
        }
        else if(f3=="110") s="0001";
        else if(f3=="111") s="0000";
        else if(f3=="100") s="0011";
        else if(f3=="001") s="1101";
    }
    return s;
}
int ALU(string ALUSelect,int rs1,int rs2){
    int ans=0;
    if(ALUSelect=="0000") ans=rs1&rs2;
    else if(ALUSelect=="0001") ans=rs1|rs2;
    else if(ALUSelect=="0010") ans=rs1+rs2;
    else if(ALUSelect=="0011") ans=rs1^rs2;
    else if(ALUSelect=="0110") ans=rs1-rs2;
    else if(ALUSelect=="1101") ans=rs1<<1;
    return ans;
}
int inst=0;
int COUNT=0;
class IFID{
    public:
        string IR="";
        int DPC=0;
        int NPC=0;
        void clock(){
        	COUNT++;
        	IR = IM[PC/4];
        	DPC= PC;
        	NPC= PC+4;
        	PC = NPC;
        	inst++;
		}
}IFID;
class IDEX{
    public:
        int DPC=0,NPC=0,JPC=0;
        int imm1=0,imm2=0;
        struct ControlWord cw;
        string funct3="",funct7="";
        int rs1=0,rs2=0,rdl=0;
        void clock(){
        	COUNT++;
        	string instruction=IFID.IR;
			funct3=""; funct7="";
			int rsl1,rsl2;
			string s_rdl="",opcode="",s_rsl1="",s_rsl2="";
            for(int i=31;i>=25;i--) funct7.pb(instruction[31-i]);
            for(int i=24;i>=20;i--) s_rsl2.pb(instruction[31-i]);
            for(int i=19;i>=15;i--) s_rsl1.pb(instruction[31-i]);
            for(int i=14;i>=12;i--) funct3.pb(instruction[31-i]);
            for(int i=11;i>=7;i--) s_rdl.pb(instruction[31-i]);
            for(int i=6;i>=0;i--) opcode.pb(instruction[31-i]);
            rsl1= binary_to_int(s_rsl1);
            rsl2= binary_to_int(s_rsl2);
            
            cw  = controller(opcode);
            imm1= ImmGen(instruction,cw.Branch,1);
            imm2= ImmGen(instruction,cw.Jump,2);
            rdl = binary_to_int(s_rdl);
            if(cw.Mem_write) rdl=rsl2;
            DPC = IFID.DPC;
            NPC = DPC+4;
            JPC = DPC+(imm2<<1);
				
			if(cw.RegRead){
			    if(!op_forward && rsl1==previous) stall=true;
				rs1 = GPR[rsl1];
				if(cw.ALU_src){
					if(cw.Mem_write || cw.Branch) rs2 = imm1;
					else rs2 = imm2;
				}
				else{
				    if(!op_forward && rsl2==previous) stall=true;
				    rs2 = GPR[rsl2]; 
				}
			}
			if(cw.Reg_write) previous=rdl;
			else previous=-1;
		}
}IDEX;
class EXMO{
    public:
        struct ControlWord cw;
        int ALUout=0;
        int rs2=0,rdl=0;
        void clock(){
        	COUNT++;
            cw = IDEX.cw;
            rs2 = IDEX.rs2;
            rdl = IDEX.rdl;
            
        	string ALUSelect=ALUControl(cw.ALU_op,IDEX.funct3,IDEX.funct7);
        	ALUout = ALU(ALUSelect,IDEX.rs1,rs2);
        	
			bool ALUZeroFlag=(IDEX.rs1==rs2);
			
			if(cw.Branch and ALUZeroFlag){
			    PC = (IDEX.imm1<<1)+IDEX.DPC;
			    IFID.clock(); // Flush till current PC
			}
			else if(cw.Jump){
			    PC = IDEX.JPC;
			    IFID.clock(); // Flush till current PC
			}
			
			op_forward=false;
			if(cw.Reg_write && !cw.Mem2Reg){ // Operand Forwarding
			    GPR[rdl] = ALUout;
			    cout<<"Register x"<<rdl<<": "<<GPR[rdl]<<"\n";
			    op_forward=true;
			}
		}
}EXMO;
class MOWB{
    public:
    	struct ControlWord cw;
    	int LDout=0,ALUout=0;
    	int rdl=0;
    	void clock(){
    		COUNT++;
    	    cw = EXMO.cw;
    	    rdl = EXMO.rdl;
    	    ALUout = EXMO.ALUout;
    	    if(cw.Mem_write){
    	            DM[ALUout] = GPR[rdl];
    	            cout<<"DM["<<ALUout<<"]: "<<GPR[rdl]<<"\n";
    	    }
    	    if(cw.Mem_read) LDout=DM[ALUout];
    	    if(cw.Reg_write && cw.Mem2Reg){
    	        GPR[rdl] = LDout;
    	        cout<<"Register x"<<rdl<<": "<<GPR[rdl]<<"\n";
    	    }
		}
}MOWB;

int main(){
    freopen("Binary.txt","rb",stdin);
    
    Type.initialize();
    string NOP="00000000000000000000000000000000";
    
    while(true){
        string instruction;
        cin>>instruction;
        int n=instruction.size();
        if(n!=32) break;
        IM.pb(instruction);
    }
    f(5) IM.pb(NOP);
    while(PC/4<IM.size()){
        MOWB.clock();
        if(!stall) EXMO.clock();
        if(stall){
            stall=false;
            previous=-1;
        }
        if(!stall) IDEX.clock();
        if(!stall) IFID.clock();
    }
    cout<<"\nNumber of cycles: "<<COUNT<<"\n";
    cout<<"Instruction: "<<inst<<"\n";
    return 0;
}
