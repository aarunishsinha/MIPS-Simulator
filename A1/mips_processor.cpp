#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

void array_of_tokens(string instr, char *tokenized_inst[10]) {
	int len = instr.length();
	char str[len+1];
	strcpy(str, instr.c_str());
	char *token = strtok(str, " ");
	//char *tokenized_inst[10];
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
}

int powerof2 (int x) {
	int result = 1;
	for(int i = 1; i <= x; i++) {
		result = result*2;
	}
	return result;
}

int main() {
	ifstream infile;
	//input string
	string str;
	//instruction array
	string instruction[1000];
	string cycles[100];
	int cycle_for_command[20];
	for(int j = 0; j<20; j++){
		cycle_for_command[j] = 0;
	}
	int index = 0;
	int cycle_cnt = 0;

	infile.open("clockcycles.txt");
	while(getline(infile, str)) {
		cycles[index] = str;
		index++;
	}
	for (int i = 0; i < index; i++) {
		string data = cycles[i];
		char *token_cycle[10];
		array_of_tokens(data, token_cycle);
		if(strcmp(token_cycle[0], "lw") == 0){
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[0] = cnt;
		}
		else if(strcmp(token_cycle[0], "sw") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[1] = cnt;
		}
		else if(strcmp(token_cycle[0], "add") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[2] = cnt;
		}
		else if(strcmp(token_cycle[0], "sub") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[3] = cnt;
		}
		else if(strcmp(token_cycle[0], "srl") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[4] = cnt;
		}
		else if(strcmp(token_cycle[0], "sll") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[5] = cnt;
		}
		else if(strcmp(token_cycle[0], "jal") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[6] = cnt;
		}
		else if(strcmp(token_cycle[0], "j") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[7] = cnt;
		}
		else if(strcmp(token_cycle[0], "jr") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[8] = cnt;
		}
		else if(strcmp(token_cycle[0], "beq") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[9] = cnt;
		}
		else if(strcmp(token_cycle[0], "bne") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[10] = cnt;
		}
		else if(strcmp(token_cycle[0], "blez") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[11] = cnt;
		}
		else if(strcmp(token_cycle[0], "bgtz") == 0) {
			int cnt = atoi(token_cycle[1]);
			cycle_for_command[12] = cnt;
		}
	}
	infile.close();
	index = 0;
	infile.open("instructions.txt");

	//register file
	float register_file[32];
	float ipc = 0.0;
	//memory array
	float memory_array[4096];
	for(int i = 0; i < 4096; i++) {
		memory_array[i] = 0.0;
	}
	memory_array[1024] = 3.0;
	memory_array[1025] = 5.0;
	memory_array[1026] = 2.0;
	memory_array[1027] = 6.0;
	memory_array[1028] = 7.0;
	memory_array[1029] = 3.0;
	memory_array[1030] = 10.0;
	//read input from file
	while(getline(infile, str)) {
		instruction[index] = str;
		index++;
	}
	// for(int i = 0; i < index; i++) {
	// 	cout << i;
	// 	cout << "=";
	// 	cout << instruction[i]<<endl;
	// }
	int ra = 0;
	int i = 0;
	while ( i < index) {
		string instr = instruction[i];
		cout << instr <<endl;
		char *tokenized_inst[10];
		array_of_tokens(instr, tokenized_inst);
		if(strcmp(tokenized_inst[0], "lw") == 0) {
			int reg = atoi(tokenized_inst[1]);
			int position = atoi(tokenized_inst[2]);
			register_file[reg] = memory_array[position];
			cycle_cnt = cycle_cnt + cycle_for_command[0];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "sw") == 0) {
			int reg = atoi(tokenized_inst[1]);
			int position = atoi(tokenized_inst[2]);
			memory_array[position] = register_file[reg];
			cycle_cnt = cycle_cnt + cycle_for_command[1];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "add") == 0) {
			int dest_reg = atoi(tokenized_inst[1]);
			int reg1 = atoi(tokenized_inst[2]);
			int reg2 = atoi(tokenized_inst[3]);
			register_file[dest_reg] = register_file[reg1] + register_file[reg2];
			cycle_cnt = cycle_cnt + cycle_for_command[2];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "sub") == 0) {
			int dest_reg = atoi(tokenized_inst[1]);
			int reg1 = atoi(tokenized_inst[2]);
			int reg2 = atoi(tokenized_inst[3]);
			register_file[dest_reg] = register_file[reg1] - register_file[reg2];
			cycle_cnt = cycle_cnt + cycle_for_command[3];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "srl") == 0) {
			int dest_reg = atoi(tokenized_inst[1]);
			int reg = atoi(tokenized_inst[2]);
			int shamt = atoi(tokenized_inst[3]);
		 	float result = register_file[reg]*powerof2(shamt);
			register_file[dest_reg] = result;
			cycle_cnt = cycle_cnt + cycle_for_command[4];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "sll") == 0) {
			int dest_reg = atoi(tokenized_inst[1]);
			int reg = atoi(tokenized_inst[2]);
			int shamt = atoi(tokenized_inst[3]);
			int result = register_file[reg]/powerof2(shamt);
			register_file[dest_reg] = result;
			cycle_cnt = cycle_cnt + cycle_for_command[5];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "beq") == 0) {
			int reg1 = atoi(tokenized_inst[1]);
			int reg2 = atoi(tokenized_inst[2]);
			if(register_file[reg1] == register_file[reg2]) {
				ra = i+1;
				i = atoi(tokenized_inst[3]);
			}
			cycle_cnt = cycle_cnt + cycle_for_command[9];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "bne") == 0) {
			int reg1 = atoi(tokenized_inst[1]);
			int reg2 = atoi(tokenized_inst[2]);
			if(register_file[reg1] != register_file[reg2]) {
				ra = i+1;
				i = atoi(tokenized_inst[3]);
			}
			cycle_cnt = cycle_cnt + cycle_for_command[10];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "blez") == 0) {
			int reg1 = atoi(tokenized_inst[1]);
			if(register_file[reg1] <= 0) {
				ra = i+1;
				i = atoi(tokenized_inst[2]);
			}
			cycle_cnt = cycle_cnt + cycle_for_command[11];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "bgtz") == 0) {
			int reg1 = atoi(tokenized_inst[1]);
			if(register_file[reg1] > 0) {
				ra = i+1;
				i = atoi(tokenized_inst[2]);
			}
			cycle_cnt = cycle_cnt + cycle_for_command[12];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "j") == 0) {
			ra = i+1;
			i = atoi(tokenized_inst[1]);
			cycle_cnt = cycle_cnt + cycle_for_command[7];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "jal") == 0) {
			ra = i+1;
			i = atoi(tokenized_inst[1]);
			cycle_cnt = cycle_cnt + cycle_for_command[6];
			ipc++;
		}
		else if(strcmp(tokenized_inst[0], "jr") == 0) {
			i = ra-1;
			cycle_cnt = cycle_cnt + cycle_for_command[8];
			ipc++;
		}
		else if (strcmp(tokenized_inst[0], "exit") == 0) {
			break;
		}
		i = i + 1;
		for (int j = 0; j < 20; j++) {
			printf("reg = %d , val = %f\n", j, register_file[j]);
		}
	}
	infile.close();
	printf("cycle_cnt = %d\n", cycle_cnt);
	printf("instructions_per_cycle = %f\n", ipc/cycle_cnt);
}