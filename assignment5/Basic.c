#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**************************************/

struct Control
{
	unsigned char RegDst;
	unsigned char Jump;
	unsigned char Branch;
	unsigned char MemRead;
	unsigned char MemtoReg;
	unsigned char ALUOp;
	unsigned char MemWrite;
	unsigned char ALUSrc;
	unsigned char RegWrite;
};

struct Reg_Read
{
	unsigned int Read_data_1;
	unsigned int Read_data_2;
};

struct ALU
{
	unsigned char zero; // 1: enable, 0: disable
	unsigned int ALU_result;
};

struct Control control;
struct Reg_Read reg_read;
struct ALU alu;
unsigned int mem[64] = { 0 };
unsigned int reg[32] = { 0 };

/**************************************/

unsigned int Inst_Fetch(unsigned int pc);
unsigned int Add(unsigned int pc, unsigned int immediate);
unsigned int Sign_Extend(unsigned int inst_16);
unsigned int Shift_Left_2(unsigned int inst);

void print_reg_mem(void);

void print_oper(unsigned int inst_31_26);
void print_control_state(void);

/**************************************/

int main(void)
{
	unsigned int pc = 0;
	FILE *fp;
	unsigned int inst = 0;
	unsigned int inst_31_26 = 0;
	unsigned int inst_25_21 = 0;
	unsigned int inst_20_16 = 0;
	unsigned int inst_15_11 = 0;
	unsigned int inst_15_0 = 0;
	unsigned int inst_ext_32 = 0;
	unsigned int inst_ext_shift = 0;
	unsigned int pc_add_4 = 0;
	unsigned int pc_add_inst = 0;
	unsigned int mux_result = 0;
	unsigned char ALU_control = 0;
	unsigned int inst_25_0 = 0;
	unsigned int jump_addr = 0;
	unsigned int mem_result = 0;
	int total_cycle = 0;

	// register initialization
	/**************************************/
	reg[8] = 41621;
	reg[9] = 41621;
	reg[16] = 40;
	/**************************************/

	// memory initialization
	/**************************************/
	mem[40] = 3578;

	if ( !(fp = fopen("input_file/3.txt", "r")) )
	{
		printf("error: file open fail !!\n");
		exit(1);
	}

	while (feof(fp) == false)
	{
		fscanf(fp, "%x", &inst);
		mem[pc] = inst;
		pc = pc + 4;
	}
	/**************************************/

	// control initialization
	/**************************************/
	control.RegDst = 0;
	control.Jump = 0;
	control.Branch = 0;
	control.MemRead = 0;
	control.MemtoReg = 0;
	control.ALUOp = 0;
	control.MemWrite = 0;
	control.ALUSrc = 0;
	control.RegWrite = 0;
	/**************************************/

	print_reg_mem();

	printf("\n ***** Processor START !!! ***** \n");

	pc = 0;

	while (pc < 20)
	{
		// pc +4
		pc_add_4 = Add(pc, 4);

		// instruction fetch
		inst = Inst_Fetch(pc);
		printf("Instruction = %08x \n", inst);


		// instruction decode
		inst_31_26 = inst >> 26;
		inst_25_21 = (inst & 0x03e00000) >> 21;
		inst_20_16 = (inst & 0x001f0000) >> 16;
		inst_15_11 = (inst & 0x0000f800) >> 11;
		inst_15_0 = inst & 0x0000ffff;
		inst_25_0 = inst & 0x03ffffff;

		printf("inst_31_26: %x\n", inst_31_26);
		printf("inst_25_21: %x\n", inst_25_21);
		printf("inst_20_16: %x\n", inst_20_16);
		printf("inst_15_11: %x\n", inst_15_11);
		printf("inst_15_00: %x\n", inst_15_0);
		printf("inst_25_00: %x\n", inst_25_0);



		/********************************/

		// implementation
		control.RegDst = (inst_31_26 == 0);
		control.Jump = (inst_31_26 == 2);
		control.Branch = (inst_31_26 == 4);
		control.MemRead = (inst_31_26 == 35);
		control.MemtoReg = (inst_31_26 == 35);
		control.ALUOp = ((inst_31_26 == 0) << 1) + (inst_31_26 == 4);
		control.MemWrite = (inst_31_26 == 43);
		control.ALUSrc = (inst_31_26 == 35) || (inst_31_26 == 43);
		control.RegWrite = (inst_31_26 == 0) || (inst_31_26 == 35);
		/********************************/

		print_oper(inst_31_26);
		print_control_state();

		total_cycle++;

		// result
		/********************************/
		printf("PC : %d \n", pc);
		printf("Total cycle : %d \n", total_cycle);
		print_reg_mem();
		/********************************/
		pc = pc_add_4;
		//system("pause");
	}

	printf("\n ***** Processor END !!! ***** \n");



	return 0;
}

unsigned int Add(unsigned int pc, unsigned int immediate)
{
	return pc + immediate;
}

unsigned int Inst_Fetch(unsigned int pc)
{
	return mem[pc];
}

unsigned int Sign_Extend(unsigned int inst_16)
{
	unsigned int inst_32 = 0;
	if ((inst_16 & 0x00008000)) // minus
	{
		inst_32 = inst_16 | 0xffff0000;
	}
	else // plus
	{
		inst_32 = inst_16;
	}

	return inst_32;
}

unsigned int Shift_Left_2(unsigned int inst)
{
	return inst << 2;
}

void print_reg_mem(void)
{
	int reg_index = 0;
	int mem_index = 0;

	printf("\n===================== REGISTER =====================\n");

	for (reg_index = 0; reg_index < 8; reg_index++)
	{
		printf("reg[%02d] = %08d        reg[%02d] = %08d        reg[%02d] = %08d        reg[%02d] = %08d \n",
			reg_index, reg[reg_index], reg_index+8, reg[reg_index+8], reg_index+16, reg[reg_index+16], reg_index+24, reg[reg_index+24] );
	}

	printf("===================== REGISTER =====================\n");

	printf("\n===================== MEMORY =====================\n");

	for (mem_index = 0; mem_index < 32; mem_index = mem_index + 4)
	{
		printf("mem[%02d] = %012d        mem[%02d] = %012d \n",
			mem_index, mem[mem_index], mem_index + 32, mem[mem_index + 32]);
	}
	printf("===================== MEMORY =====================\n");
}

void print_oper(unsigned int inst_31_26)
{
	printf("Oper: ");
	switch (inst_31_26)
	{
		case 0:
		printf("R-format\n");
		break;

		case 2:
		printf("Jump\n");
		break;

		case 4:
		printf("BEQ\n");
		break;

		case 35:
		printf("LW\n");
		break;

		case 43:
		printf("SW\n");
		break;
	}
}

void print_control_state(void)
{
	printf("RegDst: %d\n", control.RegDst);
	printf("Jump: %d\n", control.Jump);
	printf("Branch: %d\n", control.Branch);
	printf("MemRead: %d\n", control.MemRead);
	printf("MemtoReg: %d\n", control.MemtoReg);
	printf("ALUOp: %d\n", control.ALUOp);
	printf("MemWrite: %d\n", control.MemWrite);
	printf("ALUSrc: %d\n", control.ALUSrc);
	printf("RegWrite: %d\n", control.RegWrite);
}
