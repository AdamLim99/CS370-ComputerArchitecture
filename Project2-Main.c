// Name: Ze Jia Lim
// Description: CS370 - Project 2
// Last Modified: 2/21/2020

#include <stdio.h>
#include <stdlib.h>

//Variables declarations
short magicNum = 0;
short version = 0;
short numSections = 0;
short type = 0;
int address = 0;
int size = 0;
int instruc = 0;
int function = 0;
int regs = 0;
FILE* inFile = NULL;

// Functions declarations
void functions (int instr, int reg, int funct);
void registers (int reg);

int main (int argc, char** argv) {;
  if(argc==2){
    inFile = fopen(argv[1], "r");
  }
  else{
    fprintf(stderr, "Incorrect number of arguments.\nExiting the program...\n");
    return 0;
  }

  if(inFile==NULL){
    fprintf(stderr, "Unable to open file.\nExiting the program...\n");
    return 0;
  }

  fread(&magicNum, 1, 1, inFile);
  if(magicNum!=76){
    return 0;
  }
  fread(&magicNum, 1, 1, inFile);
  if(magicNum!=97){
    return 0;
  }
  fread(&magicNum, 1, 1, inFile);
  if(magicNum!=88){
    return 0;
  }
  fread(&magicNum, 1, 1, inFile);
  if(magicNum!=0){
    return 0;
  }
  fread(&version, 2, 1, inFile);
  fread(&numSections, 1, 1, inFile);

  int i, j;
  for(i=0; i<numSections; i++){
    fread(&type, 1, 1, inFile);
    fread(&address, 4, 1, inFile);
    fread(&size, 4, 1, inFile);
    if(type==8){
      printf(".data 0x%08x\n", address); 
      for(j=0; j<size; j++){
	fread(&instruc, 1, 1, inFile);
	printf(".byte %d\n", instruc);
      }
    }
    else if(type==7){
      printf(".text 0x%08x\n", address);
      size=size/4;
      for(j=0; j<size; j++){
	fread(&instruc, 4, 1, inFile);
	regs=(instruc>>26)&0x3f;
	function=(instruc)&0x0000003f;
	functions(instruc, regs, function);
      }
    }
    else{
      fprintf(stderr, "Error: Not .data nor .text segment.\nExiting the program...\n");
      return 0;
    }
  }
  fclose(inFile);
  return 0;
}

void functions (int instr, int reg, int funct){
  // R-Type
  if(reg==0){
    if(funct==32){printf("add ");}
    else if(funct==33){printf("addu ");}
    else if(funct==36){printf("and ");}
    else if(funct==39){printf("nor ");}
    else if(funct==37){printf("or ");}
    else if(funct==42){printf("slt ");}
    else if(funct==43){printf("sltu ");}
    else if(funct==0){printf("sll ");}
    else if(funct==3){printf("sra ");}
    else if(funct==2){printf("srl ");}
    else if(funct==34){printf("sub ");}
    else if(funct==35){printf("subu ");}
    else if(funct==38){printf("xor ");}
    else if(funct==8){printf("jr ");}
    else if(funct==12){
      printf("syscall\n");
      return;
    }
    else{
      fprintf(stderr, "Error: Not a MIPS instruction.\nExiting the program...\n");
      exit(0);
    }

    int rs, rt, rd, shamt;
    rs=(instr>>21)&0x01f;
    rt=(instr>>16)&0x001f;
    rd=(instr>>11)&0x00001f;
    shamt=(instr>>6)&0x000001f;
    // Shift cases
    if(funct==0 || funct==3 || funct==2){
      if(rs!=0){
	fprintf(stderr, "Error: rs should be 0.\nExiting the program...\n");
        exit(0);
      }
      if(shamt==0){
	fprintf(stderr, "Error: shamt should not be 0.\nExiting the program...\n");
	exit(0);
      }
      registers(rd);
      printf(", ");
      registers(rt);
      printf(", %d\n", shamt);
    }
    // jr
    else if(funct==8){
      if(rt!=0 || rd!=0 || shamt!=0){
	if(rt!=0){fprintf(stderr, "Error: rt should be 0.\nExiting the program...\n");}
	if(rd!=0){fprintf(stderr, "Error: rd should be 0.\nExiting the program...\n");}
	fprintf(stderr, "Error: shamt should be 0.\nExiting the program...\n");
	exit(0);
      }
      registers(rs);
      printf("\n");
    }
    else{
      if(shamt!=0){
	fprintf(stderr, "Error: shamt should be 0.\nExiting the program...\n");
	exit(0);
      }
      registers(rd);
      printf(", ");
      registers(rs);
      printf(", ");
      registers(rt);
      printf("\n");
    }
  }
  // J-Type
  else if(reg==2 || reg==3){
    if(reg==2){
      printf("j 0x%08x\n", (instr<<2)&0x0fffffff);
    }
    else{
      printf("jal 0x%08x\n", (instr<<2)&0x0fffffff);
    }
  }
  // I-Type
  else{
    if(reg==8){printf("addi ");}
    else if(reg==9){printf("addiu ");}
    else if(reg==12){printf("andi ");}
    else if(reg==15){printf("lui ");}
    else if(reg==13){printf("ori ");}
    else if(reg==10){printf("slti ");}
    else if(reg==11){printf("sltiu ");}
    else if(reg==36){printf("lbu ");}
    else if(reg==37){printf("lhu ");}
    else if(reg==35){printf("lw ");}
    else if(reg==40){printf("sb ");}
    else if(reg==41){printf("sh ");}
    else if(reg==43){printf("sw ");}
    else if(reg==4){printf("beq ");}
    else if(reg==5){printf("bne ");}
    else{
      fprintf(stderr, "Error: Not a MIPS instruction.\nExiting the program...\n");
      exit(0);
    }

    int rs, rt, imm;
    rs=(instr>>21)&0x01f;
    rt=(instr>>16)&0x001f;
    imm=(instr)&0x0000ffff;
    short sImm;
    sImm=(instr)&0x0000ffff;
    // beq & bne
    if(reg==4 || reg==5){
      int pc=address+4+(imm<<2);
      registers(rt);
      printf(", ");
      registers(rs);
      printf(", 0x%08x\n", pc);
    }
    // Loads and stores
    else if(reg==36 || reg==37 || reg==35 || reg==40 || reg==41 || reg==43){
      registers(rt);
      printf(", %d(", sImm);
      registers(rs);
      printf(")\n");
    }
    // lui
    else if(reg==15){
      if(rs!=0){
	fprintf(stderr, "Error: rs should be 0.\nExiting the program...\n");
	exit(0);
      }
      registers(rt);
      printf(", 0x%04x\n", sImm);
    }
    else{
      registers(rt);
      printf(", ");
      registers(rs);
      if(reg==12 || reg==13){printf(", 0x%04x\n", sImm);}
      else{printf(", %d\n", sImm);}
    }
  }
}

void registers (int reg) {
  if(reg==0){printf("$zero");}
  else if(reg==1){printf("$at");}
  else if(reg==2){printf("$v0");}
  else if(reg==3){printf("$v1");}
  else if(reg==4){printf("$a0");}
  else if(reg==5){printf("$a1");}
  else if(reg==6){printf("$a2");}
  else if(reg==7){printf("$a3");}
  else if(reg==8){printf("$t0");}
  else if(reg==9){printf("$t1");}
  else if(reg==10){printf("$t2");}
  else if(reg==11){printf("$t3");}
  else if(reg==12){printf("$t4");}
  else if(reg==13){printf("$t5");}
  else if(reg==14){printf("$t6");}
  else if(reg==15){printf("$t7");}
  else if(reg==16){printf("$s0");}
  else if(reg==17){printf("$s1");}
  else if(reg==18){printf("$s2");}
  else if(reg==19){printf("$s3");}
  else if(reg==20){printf("$s4");}
  else if(reg==21){printf("$s5");}
  else if(reg==22){printf("$s6");}
  else if(reg==23){printf("$s7");}
  else if(reg==24){printf("$t8");}
  else if(reg==25){printf("$t9");}
  else if(reg==26){printf("$k0");}
  else if(reg==27){printf("$k1");}
  else if(reg==28){printf("$gp");}
  else if(reg==29){printf("$sp");}
  else if(reg==30){printf("$fp");}
  else{printf("$ra");}
}
