#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**************************************/

struct Control
{
	unsigned char	RegDst;
	unsigned char	Jump;
	unsigned char	Branch;
	unsigned char	MemRead;
	unsigned char	MemtoReg;
	unsigned char	ALUOp;
	unsigned char	MemWrite;
	unsigned char	ALUSrc;
	unsigned char	RegWrite;
};

struct Reg_Read
{
	unsigned int	Read_data_1;
	unsigned int	Read_data_2;
};

struct ALU
{
	unsigned char	zero; // 1: enable, 0: disable
	unsigned int	ALU_result;
};

struct Control	control;
struct Reg_Read	reg_read;
struct ALU		alu;
unsigned int	mem[64] = { 0 };
unsigned int	reg[32] = { 0 };

/**************************************/

unsigned int	Inst_Fetch(unsigned int read_addr);
void			Register_Read(unsigned int read_reg_1, unsigned int read_reg_2);
void			Control_Signal(unsigned int opcode);
unsigned char	ALU_Control_Signal(unsigned char signal);
void			ALU_func(unsigned char ALU_control, unsigned int a, unsigned int b);
unsigned int	Memory_Access(unsigned char MemWrite, unsigned char MemRead, unsigned int addr, unsigned int write_data);
void			Register_Write(unsigned char RegWrite, unsigned int Write_reg, unsigned int Write_data);
unsigned int	Sign_Extend(unsigned int inst_16);
unsigned int	Shift_Left_2(unsigned int inst);
unsigned int	Add(unsigned int a, unsigned int b);
unsigned int	Mux(char signal, unsigned int a_0, unsigned int b_1);
void			print_reg_mem(void);

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

	if ( !(fp = fopen("input.txt", "r")) )
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
	control.ALUOp = 0;
	control.MemWrite = 0;
	control.ALUSrc = 0;
	control.RegWrite = 0;
	/**************************************/

	print_reg_mem();

	printf("\n ***** Processor START !!! ***** \n");

	pc = 0;

	while (pc < 64)
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

		//printf("%x, %x, %x, %x, %x, %x", inst_31_26, inst_25_21, inst_20_16, inst_15_11, inst_15_0, inst_25_0);

		// register read


		// create control signal

		// create ALU control signal

		// ALU

		// memory access

		// register write


		total_cycle++;

		// result
		/********************************/
		printf("PC : %d \n", pc);
		printf("Total cycle : %d \n", total_cycle);
		print_reg_mem();
		/********************************/

		system("pause");
	}

	printf("\n ***** Processor END !!! ***** \n");



	return 0;
}

unsigned int Inst_Fetch(unsigned int read_addr)
{
	return (mem[read_addr]);
}

void Register_Read(unsigned int read_reg_1, unsigned int read_reg_2)
{

}

void Control_Signal(unsigned int opcode)
{
	unsigned int	opcode_5 = opcode & 0x20;
	unsigned int	opcode_4 = opcode & 0x10;
	unsigned int	opcode_3 = opcode & 0x08;
	unsigned int	opcode_2 = opcode & 0x04;
	unsigned int	opcode_1 = opcode & 0x02;
	unsigned int	opcode_0 = opcode & 0x01;

	unsigned int	R_format = !opcode_5 && !opcode_4 && !opcode_3 && !opcode_2 && !opcode_1 && !opcode_0;
	unsigned int	BEQ = !opcode_5 && !opcode_4 && !opcode_3 && opcode_2 && !opcode_1 && !opcode_0;
	unsigned int	Lw = opcode_5 && !opcode_4 && !opcode_3 && !opcode_2 && opcode_1 && opcode_0;
	unsigned int	Sw = opcode_5 && !opcode_4 && opcode_3 && !opcode_2 && opcode_1 && opcode_0;
	unsigned int	Jump = !opcode_5 && !opcode_4 && !opcode_3 && !opcode_2 && opcode_1 && !opcode_0;

	control.RegDst = R_format;
	control.Jump = Jump;
	control.Branch = BEQ;
	control.MemRead = Lw;
	control.MemtoReg = Lw;
	control.ALUOp = R_format << 1 + BEQ;
	control.MemWrite = Sw;
	control.ALUSrc = Lw || Sw;
	control.RegWrite = R_format || Lw;
}

unsigned char ALU_Control_Signal(unsigned char signal)
{
	unsigned char	ALU_control = 0;

	ALU_control += (control.ALUOp & 0x01) || ((control.ALUOp & 0x02) && (signal & 0x02));
	ALU_control = ALU_control << 1;
	ALU_control += !(control.ALUOp & 0x02) || !(signal & 0x04);
	ALU_control = ALU_control << 1;
	ALU_control += (control.ALUOp & 0x02) && ((signal & 0x08) || (signal & 0x01));
	return (ALU_control);
}

void ALU_func(unsigned char ALU_control, unsigned int a, unsigned int b)
{
	if (ALU_control & 0x08)
		!(a || b);
	else if (ALU_control & 0x04)
	{
		if (ALU_control & 0x01)
			//set on less than
		else
			a - b;
	}
	else if (ALU_control & 0x02)
		a + b;
	else if (ALU_control & 0x01)
		a || b;
	else
		a && b;
}

unsigned int Memory_Access(unsigned char MemWrite, unsigned char MemRead, unsigned int addr, unsigned int write_data)
{
	if (MemWrite)
		mem[addr] = write_data;
	if (MemRead)
		return (mem[addr]);
}

void	Register_Write(unsigned char RegWrite, unsigned int Write_reg, unsigned int Write_data)
{
	if (RegWrite)
		reg[Write_reg] = Write_data;
}

unsigned int Sign_Extend(unsigned int inst_16)
{
	unsigned int	inst_32 = 0;

	if ((inst_16 & 0x00008000)) // minus
		inst_32 = inst_16 | 0xffff0000;
	else // plus
		inst_32 = inst_16;
	return (inst_32);
}

unsigned int	Shift_Left_2(unsigned int inst)
{
	return (inst << 2);
}

unsigned int	Mux(char signal, unsigned int a_0, unsigned int b_1)
{
	return (signal ? b_1 : a_0);
}

unsigned int	Add(unsigned int a, unsigned int b)
{
	return (a + b);
}

void	print_reg_mem(void)
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

