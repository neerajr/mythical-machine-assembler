#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct dict {
	char *word;
	int value;
} codes[] = {"hlt",0, "ld",1, "sto",2, "ld#",3, "ldi",4, "add",5, "sub",6, "mul",7, "div",8, "jmp",10, "jz",11,},
	lookup[100] = {"r0",0, "r1",1, "r2",2, "r3",3, "r4",4, "r5",5, "r6",6, "r7",7, "r8",8, "r9",9, "0",0};

int pReg;

int getVal(char *s)
{
//	printf("In getVal s is %s\n",s);
	int a = -1;
	int i = 0;
	if(s == NULL)
		return 0;
	while(strcmp(s,lookup[i].word) != 0 && i<15)
		i++;
//	printf("value of i %d\n",i);
	if(strcmp(s,lookup[i].word) == 0)
		a = lookup[i].value;
//	printf("value of a %d\n",a);
	if(a==-1)
		return atoi(s);
	else 
		return a;
}


void insertSymb(char *label, int value)
{
//	printf("In insert\n");
	static int lookupcount = 11;
	lookup[lookupcount].word = label;
	lookup[lookupcount].value = value;
	lookupcount++;
}

void pass1(char *program)
{
	FILE *fil;
	int i, j;
	char symb[200];
	pReg = 100;

	fil = fopen(program, "r");
	if(fil != NULL) {
	//	printf("In pass1\n");
		char lin[500];
		while(fgets(lin, sizeof lin, fil) != NULL){
			i = 0;
			if(lin[0] > ' ') {
				j = 0;
				while(lin[i] != ' ') {
					symb[j] = lin[i]; 
					i++, j++;
				}
				symb[j] = '\0';
	//			printf("symb : %s pReg : %d\n",symb,pReg);
				insertSymb(symb, pReg);
				pReg += 1;
			}
			else 
				pReg += 1;
		}
	}
	fclose(fil);
}

int assemble(char *flds1, char *flds2)
{
//	printf("In assemble\n");
	int opval = -1;
	int i,j,result;
	char parts1[100], parts2[100];
	i = 0;
	while(strcmp(flds1,codes[i].word) != 0)
		i++;
//	printf("In assemble again\n");
	if(strcmp(flds1,codes[i].word) == 0)
		opval = codes[i].value;
//	printf("opval %d\n",opval);
	if(opval == -1)
		return atoi(flds1);
	if(opval == 0)
		return 0;
	i = 0, j=0;
	while(flds2[i] != ','){
		parts1[j] = flds2[i];
		i++, j++;
	}
	parts1[j] = '\0';
//	printf("parts1 in assemble: %s ivalue: %d\n",parts1,i);
	i++;
	j =0;
//	printf("In assemble again\n");
	while(flds2[i] > ' '){
		parts2[j] = flds2[i];
		i++, j++;
//		printf("testing ivalue %d\n",i);
	}
	parts2[j] = '\0';
//	printf("parts2 in assemble: %s ivalue: %d\n",parts2,i);
	if(parts2[0] == '\0'){
		strcpy(parts2,parts1);
		parts1[0] = (char)'0';
		parts1[1] = '\0';
	}
//	printf("parts1:%s\tparts2:%s\n",parts1,parts2);
	result=opval*10000 + getVal(parts1)*1000 + getVal(parts2);
//	printf("%d %d\n", pReg, result);
	return result;
	//	return opval*10000 + getVal(parts1)*1000 + getVal(parts2);
}


void pass2(char *program)	
{
	FILE *fil;
	int i, j, instruction;
	char flds1[200], flds2[200];
	pReg = 100;
	fil = fopen(program, "r");
	if(fil != NULL){
//			printf("In pass2 again\n");
		char lin[500];
		while(fgets(lin, sizeof lin, fil) != NULL) {
			i = 0;
			if(lin[0] > ' ') {
				while(lin[i] != ' ')
					i++;
				while(lin[i] == ' ')
					i++;
			}
//			printf("valueof i in pass2: %d charactr: %c",i,lin[i]);
			j = 0;
			if(lin[i] == ' ') {
				while(lin[i] == ' ')
					i++;
			}
			if(lin[i] > ' ') {
				while(lin[i] != ' ') {
					flds1[j] = lin[i];
					i++; j++;
				}
				flds1[j] = '\0';
			}
//			printf("flds1 in pass2 : %s ivalue : %d\n",flds1,i);
			j = 0;
//			printf("%c %c\n",lin[i],lin[i+1]);
			while(lin[i] == ' ')
				i++;
			while(lin[i] > ' ') {
				flds2[j] = lin[i];
				i++; j++;
			}
			flds2[j] = '\0';
//			printf("flds2 in pass2 : %s ivalue : %d\n",flds2,i);
	//		printf("In pass2 again\n");
			instruction = assemble(flds1, flds2);
			printf("Instruction is %d\n",instruction);
			printf("%d %d %s\n", pReg, instruction, lin);
			pReg += 1;
		}
	}
	fclose(fil);
}


main(int argc, char *argv[])
{
	pass1(argv[1]);
	pass2(argv[1]);
}


