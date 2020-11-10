#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>

using namespace std;

struct INSTRUCTION {
	string instruction;
	string function;
	int rd;
	int rs;
	int rt;
	int shamt;
	int offset;
	tuple <string, int, int, float> ALUResult;
};

int PC = -1;
int ra = 0;
int clock_cycles = 0;

string instruction_memory[1000];
float register_file[32];
float memory_array[4096];
struct INSTRUCTION pipeline_array[5];

int inWriteRegArr[32];
float DataForwardingReg[32];

int powerof2 (int x) {
	int result = 1;
	for(int i = 1; i <= x; i++) {
		result = result*2;
	}
	return result;
}

int hit_miss (int x) {								// Hit or Miss predictor
	int n = rand() % 101;
	int hit;
	if(n <= x) {
		hit = 1;
	}
	else {
		hit = 0;
	}
	return hit;
}

string IF() {
	return instruction_memory[PC];
}

INSTRUCTION ID(struct INSTRUCTION instruct) {
	string instr = instruct.instruction;
	char *tokenized_inst[10];
	int len = instr.length();
	char str[len+1];
	strcpy(str, instr.c_str());
	char *token = strtok(str, " ");
	int cnt = 0;
	while(token != NULL) {
		if(strchr(token, ',')) {
			token[strlen(token)-1] = '\0';
		}
		if(strcmp(token, "$ra") != 0 && strchr(token, '$')) {
			for(int i = 1; i < strlen(token); i++) {
				token[i-1] = token[i];
			}
			token[strlen(token)-1] = '\0';
		}
		tokenized_inst[cnt] = token;
		cnt++;
		token = strtok(NULL, " ");
	}
	if(strcmp(tokenized_inst[0], "lw") == 0 || strcmp(tokenized_inst[0], "sw") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = 0;
		instruct.rs = 0;
		instruct.rt = atoi(tokenized_inst[1]);
		instruct.shamt = 0;
		instruct.offset = atoi(tokenized_inst[2]);
		inWriteRegArr[instruct.rt] = 1;
	}
	else if(strcmp(tokenized_inst[0], "add") == 0 || strcmp(tokenized_inst[0], "sub") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = atoi(tokenized_inst[1]);
		instruct.rs = atoi(tokenized_inst[2]);
		instruct.rt = atoi(tokenized_inst[3]);
		instruct.shamt = 0;
		instruct.offset = 0;
		inWriteRegArr[instruct.rd] = 1;
	}
	else if(strcmp(tokenized_inst[0], "sll") == 0 || strcmp(tokenized_inst[0], "srl") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = atoi(tokenized_inst[1]);
		instruct.rs = 0;
		instruct.rt = atoi(tokenized_inst[2]);
		instruct.shamt = atoi(tokenized_inst[3]);
		instruct.offset = 0;
		inWriteRegArr[instruct.rd] = 1;
	}
	else if(strcmp(tokenized_inst[0], "beq") == 0 || strcmp(tokenized_inst[0], "bne") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = 0;
		instruct.rs = atoi(tokenized_inst[1]);
		instruct.rt = atoi(tokenized_inst[2]);
		instruct.shamt = 0;
		instruct.offset = atoi(tokenized_inst[3]);
	}
	else if(strcmp(tokenized_inst[0], "blez") == 0 || strcmp(tokenized_inst[0], "bgtz") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = 0;
		instruct.rs = atoi(tokenized_inst[1]);
		instruct.rt = 0;
		instruct.shamt = 0;
		instruct.offset = atoi(tokenized_inst[2]);
	}
	else if(strcmp(tokenized_inst[0], "jal") == 0 || strcmp(tokenized_inst[0], "j") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = 0;
		instruct.rs = 0;
		instruct.rt = 0;
		instruct.shamt = 0;
		instruct.offset = atoi(tokenized_inst[1]);
	}
	else if(strcmp(tokenized_inst[0], "jr") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = 0;
		instruct.rs = 31;
		instruct.rt = 0;
		instruct.shamt = 0;
		instruct.offset = 0;
	}
	else if(strcmp(tokenized_inst[0], "exit") == 0) {
		instruct.function = tokenized_inst[0];
		instruct.rd = 0;
		instruct.rs = 0;
		instruct.rt = 0;
		instruct.shamt = 0;
		instruct.offset = 0;
	}
	return instruct;
}

tuple<string, int, int, float> EX(struct INSTRUCTION instruct) {
	if(instruct.function == "lw") {
		DataForwardingReg[instruct.rt] = memory_array[instruct.offset];
		return make_tuple("lw", instruct.rt, instruct.offset, 0.0);
	}
	else if(instruct.function == "sw") {
		return make_tuple("sw", instruct.rt, instruct.offset, 0.0);
	}
	else if(instruct.function == "add") {
		float result;
		if(inWriteRegArr[instruct.rs] == 1) {
			result = DataForwardingReg[instruct.rs] + register_file[instruct.rt];
		}
		else if(inWriteRegArr[instruct.rt] == 1) {
			result = register_file[instruct.rs] + DataForwardingReg[instruct.rt];
		}
		else {
			result = register_file[instruct.rs] + register_file[instruct.rt];
		}
		DataForwardingReg[instruct.rd] = result;
		return make_tuple("add", instruct.rd, 0, result);
	}
	else if (instruct.function == "sub") {
		float result;
		if(inWriteRegArr[instruct.rs] == 1) {
			result = DataForwardingReg[instruct.rs] - register_file[instruct.rt];
		}
		else if(inWriteRegArr[instruct.rt] == 1) {
			result = register_file[instruct.rs] - DataForwardingReg[instruct.rt];
		}
		else {
			result = register_file[instruct.rs] - register_file[instruct.rt];
		}
		DataForwardingReg[instruct.rd] = result;
		return make_tuple("sub", instruct.rd, 0, result);
	}
	else if(instruct.function == "sll") {
		float result;
		if(inWriteRegArr[instruct.rt] == 1) {
			result = DataForwardingReg[instruct.rt]*powerof2(instruct.shamt);
		}
		else {
			result = register_file[instruct.rt]*powerof2(instruct.shamt);
		}
		DataForwardingReg[instruct.rd] = result;
		return make_tuple("sll", instruct.rd, 0, result);
	}
	else if(instruct.function == "srl") {
		int result;
		if(inWriteRegArr[instruct.rt] == 1) {
			result = DataForwardingReg[instruct.rt]/powerof2(instruct.shamt);
		}
		else {
			result = register_file[instruct.rt]/powerof2(instruct.shamt);
		}
		DataForwardingReg[instruct.rd] = result;
		return make_tuple("srl", instruct.rd, 0, result);
	}
	else if(instruct.function == "beq") {
		if(register_file[instruct.rs] == register_file[instruct.rt]) {
			PC = instruct.offset;
			return make_tuple("beq", 0, instruct.offset, 0.0);
		}
	}
	else if(instruct.function == "bne") {
		if(register_file[instruct.rs] != register_file[instruct.rt]) {
			PC = instruct.offset;
			return make_tuple("bne", 0, instruct.offset, 0.0);
		}
	}
	else if(instruct.function == "blez") {
		if(register_file[instruct.rs] <= 0) {
			PC = instruct.offset;
			return make_tuple("blez", 0, instruct.offset, 0.0);
		}
	}
	else if(instruct.function == "bgtz") {
		if(register_file[instruct.rs] > 0) {
			PC = instruct.offset;
			return make_tuple("bgtz", 0, instruct.offset, 0.0);
		}
	}
	return make_tuple("none", 0, 0, 0.0);
}

void MEM(tuple <string,int,int,float> ALURes) {
	if(get<0>(ALURes) == "lw") {									// Check Hit or Miss
		register_file[get<1>(ALURes)] = memory_array[get<2>(ALURes)];
		inWriteRegArr[get<1>(ALURes)] = 0;
	}
	else if(get<0>(ALURes) == "sw") {
		memory_array[get<2>(ALURes)] = register_file[get<1>(ALURes)];
	}
}

void WB(tuple <string,int,int,float> ALURes) {
	if(get<0>(ALURes) == "add" || get<0>(ALURes) == "sub" || get<0>(ALURes) == "sll" || get<0>(ALURes) == "srl") {
		register_file[get<1>(ALURes)] = get<3>(ALURes);
		inWriteRegArr[get<1>(ALURes)] = 0;
	}
}

void display() {
	if(pipeline_array[0].instruction != "begin") {
		cout <<"IF : "<<pipeline_array[0].instruction<<endl;
	}
	if(pipeline_array[1].instruction != "begin") {
		cout <<"ID : "<<pipeline_array[1].instruction<<endl;
	}
	if(pipeline_array[2].instruction != "begin") {
		cout <<"EX : "<<pipeline_array[2].instruction<<endl;
	}
	if(pipeline_array[3].instruction != "begin") {
		cout <<"MEM : "<<pipeline_array[3].instruction<<endl;
	}
	if(pipeline_array[4].instruction != "begin") {
		cout <<"WB : "<<pipeline_array[4].instruction<<endl;
	}
}

int Not_Completed() {
	if(pipeline_array[0].instruction == "begin" && pipeline_array[1].instruction == "begin" && pipeline_array[2].instruction == "begin" && pipeline_array[3].instruction == "begin" && pipeline_array[4].instruction == "begin") {
		return 1;
	}
	return 0;
}

void BRANCH_STALLING(int storePC, struct INSTRUCTION initialInstr) {
	cout <<"BRANCH STALLING"<<endl;
	ra = storePC+1;
	pipeline_array[0] = initialInstr;
	int k = 0;
	while (k != 2) {
		clock_cycles++;
		cout <<"CLOCK CYCLE : "<<clock_cycles<<endl;
		pipeline_array[1] = ID(pipeline_array[1]);
		pipeline_array[2].ALUResult = EX(pipeline_array[2]);
		MEM(pipeline_array[3].ALUResult);
		WB(pipeline_array[4].ALUResult);
		display();
		pipeline_array[4] = pipeline_array[3];
		pipeline_array[3] = pipeline_array[2];
		pipeline_array[2] = pipeline_array[1];
		pipeline_array[1] = pipeline_array[0];
		pipeline_array[0] = initialInstr;
		k++;
	}
	if(get<0>(pipeline_array[3].ALUResult) == "none") {
			PC = storePC+1;
	}
}

void STALL(int N, struct initialInstr){
	pipeline_array[0] = initialInstr;
	int k = 0;
	int sPC = PC;
	while(k != N-1) {
		clock_cycles++;
		cout <<"CLOCK CYCLE : "<<clock_cycles<<endl;
		pipeline_array[1] = ID(pipeline_array[1]);
		pipeline_array[2].ALUResult = EX(pipeline_array[2]);
		MEM(pipeline_array[3].ALUResult);
		WB(pipeline_array[4].ALUResult);
		display();
		pipeline_array[4] = pipeline_array[3];
		pipeline_array[3] = pipeline_array[2];
		pipeline_array[2] = pipeline_array[1];
		pipeline_array[1] = initialInstr;
		pipeline_array[0] = initialInstr;
		k++;
	}
	PC = sPC+1;
}

int main(int argc, char* argv[]) {
	ifstream infile;
	infile.open(argv[1]);
	string str;
	int x = atoi(argv[2]);
	int N = atoi(argv[3]);

	int index = 0;

	while(getline(infile, str)) {
		instruction_memory[index] = str;
		index++;
	}

	for (int i = 0; i < 32; i++) {
		register_file[i] = 0.0;
		inWriteRegArr[i] = 0;
		DataForwardingReg[i] = 0.0;
	}

	memory_array[1024] = 3;
	memory_array[1025] = 5;
	memory_array[1026] = 2;
	memory_array[1027] = 6;
	memory_array[1028] = 2;
	memory_array[1029] = 3;
	memory_array[1030] = 10;

	for (int i = 0; i < 5; i++) {
		pipeline_array[i].instruction = "begin";
		pipeline_array[i].function = "begin";
		pipeline_array[i].rs = -1;
		pipeline_array[i].rd = -1;
		pipeline_array[i].rt = -1;
		pipeline_array[i].offset = -1;
		pipeline_array[i].ALUResult = make_tuple("initial", 0, 0, 0.0);
	}

	struct INSTRUCTION initialInstr = pipeline_array[0];

	cout <<"Simulation Started :"<<endl;

	while(pipeline_array[0].instruction != "exit") {
		pipeline_array[4] = pipeline_array[3];
		pipeline_array[3] = pipeline_array[2];
		pipeline_array[2] = pipeline_array[1];
		pipeline_array[1] = pipeline_array[0];
		struct INSTRUCTION idex = ID(pipeline_array[1]);
		int storePC = PC;
		if(idex.function == "jr") {
			PC = ra;
		}
		else if(idex.function == "jal" || idex.function == "j") {
			ra = PC+1;
			PC = idex.offset;
		}
		else if(idex.function == "beq" || idex.function == "bne" || idex.function == "blez" || idex.function == "bgtz") {
			BRANCH_STALLING(storePC, initialInstr);
		}
		else if(idex.funtion == "lw") {
			int hit = hit_miss(x);
			if (hit == 1){
				cout<<"HIT"<<endl;
				PC++
			}
			else{
				cout<<"MISS"<<endl;
				STALL(N, initialInstr);
			}
		}
		else {
			PC++;
		}
		pipeline_array[0].instruction = IF();
		pipeline_array[1] = ID(pipeline_array[1]);
		pipeline_array[2].ALUResult = EX(pipeline_array[2]);
		MEM(pipeline_array[3].ALUResult);
		WB(pipeline_array[4].ALUResult);
		clock_cycles++;
		cout <<"CLOCK CYCLE : "<<clock_cycles<<endl;
		cout <<"PC = "<<PC<<endl;
		display();
	}

	cout <<"Completing the Simulation :"<<endl;

	while(Not_Completed() == 0) {
		pipeline_array[4] = pipeline_array[3];
		pipeline_array[3] = pipeline_array[2];
		pipeline_array[2] = pipeline_array[1];
		pipeline_array[1] = pipeline_array[0];
		pipeline_array[0] = initialInstr;
		pipeline_array[1] = ID(pipeline_array[1]);
		pipeline_array[2].ALUResult = EX(pipeline_array[2]);
		MEM(pipeline_array[3].ALUResult);
		WB(pipeline_array[4].ALUResult);
		clock_cycles++;
		cout <<"CLOCK CYCLE : "<<clock_cycles<<endl;
		display();
	}

	int total_number_of_clock_cycles = clock_cycles - 2;

	for (int i = 0; i < 32; i++) {
			cout <<"Reg "<<i<<" : "<<register_file[i]<<endl;
	}

	for (int i = 1024; i < 1050; i++) {
		if(memory_array[i] != 0) {
			cout <<"MEM "<<i<<" : "<<memory_array[i]<<endl;
		}
	}

	cout <<"TOTAL NUMBER OF CLOCK CYCLES = "<<total_number_of_clock_cycles<<endl;
}