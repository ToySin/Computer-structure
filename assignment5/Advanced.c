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
unsigned int	Mux(char signal, unsigned int a_0, unsigned int b_1);

unsigned int	Shift_Left_2(unsigned int inst);
unsigned int	Add(unsigned int a, unsigned int b);
unsigned int	Sub(unsigned int a, unsigned int b);
unsigned int	AND(unsigned int a, unsigned int b);
unsigned int	OR(unsigned int a, unsigned int b);
unsigned int	Slt(unsigned int a, unsigned int b);
unsigned int	NOR(unsigned int a, unsigned int b);


unsigned int	Branch(unsigned int pc_add_4, unsigned int addr);
unsigned int	Jump(unsigned int pc_add_4, unsigned int inst_25_0);

void			print_reg_mem(void);

void			print_control_state(void);
void			print_oper(unsigned int inst_31_26);
void			print_alu_control(unsigned int ALU_control);

/**************************************/

int main(void)
{
	FILE			*fp;
	unsigned int	pc = 0;
	unsigned int	inst = 0;
	unsigned int	inst_31_26 = 0;
	unsigned int	inst_25_21 = 0;
	unsigned int	inst_20_16 = 0;
	unsigned int	inst_15_11 = 0;
	unsigned int	inst_15_0 = 0;
	unsigned int	inst_ext_32 = 0;
	unsigned int	inst_ext_shift = 0;
	unsigned int	pc_add_4 = 0;
	unsigned int	pc_add_inst = 0;
	unsigned int	mux_result = 0;
	unsigned char	ALU_control = 0;
	unsigned int	inst_25_0 = 0;
	unsigned int	branch_addr = 0;
	unsigned int	jump_addr = 0;
	unsigned int	mem_result = 0;
	int				total_cycle = 0;
	int				file_num;

	// register initialization
	/**************************************/
	reg[8] = 41621;
	reg[9] = 41621;
	reg[16] = 40;
	/**************************************/

	// memory initialization
	/**************************************/
	mem[40] = 3578;


	printf("File number(1 ~ 5): ");
	scanf("%d", &file_num);
	switch (file_num)
	{
		case 1:
		fp = fopen("input_file/1.txt", "r");
		break;

		case 2:
		fp = fopen("input_file/2.txt", "r");
		break;

		case 3:
		fp = fopen("input_file/3.txt", "r");
		break;

		case 4:
		fp = fopen("input_file/4.txt", "r");
		break;

		case 5:
		fp = fopen("input_file/5.txt", "r");
		break;
	}
	if (!fp)
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
	printf("PC : %d \n", pc);
	printf("Total cycle : %d \n", total_cycle);

	while (pc < 64)
	{
		// pc +4
		pc_add_4 = Add(pc, 4);

		// instruction fetch
		inst = Inst_Fetch(pc);
		printf("Instruction = %08x\n", inst);
		printf("\n");

		// instruction decode
		inst_31_26 = inst >> 26;
		inst_25_21 = (inst & 0x03e00000) >> 21;
		inst_20_16 = (inst & 0x001f0000) >> 16;
		inst_15_11 = (inst & 0x0000f800) >> 11;
		inst_15_0 = inst & 0x0000ffff;
		inst_25_0 = inst & 0x03ffffff;

		//printf("%x, %x, %x, %x, %x, %x", inst_31_26, inst_25_21, inst_20_16, inst_15_11, inst_15_0, inst_25_0);

		// register read
		Register_Read(inst_25_21, inst_20_16);

		// create control signal
		Control_Signal(inst_31_26);

		// create ALU control signal
		ALU_control = ALU_Control_Signal(inst_15_0 & 0x003f);

		// ALU
		ALU_func(ALU_control, reg_read.Read_data_1, \
				Mux(control.ALUSrc, reg_read.Read_data_2, Sign_Extend(inst_15_0)));

		// memory access
		mem_result = Memory_Access(control.MemWrite, control.MemRead, alu.ALU_result, reg_read.Read_data_2);

		// register write
		Register_Write(control.RegWrite, \
				Mux(control.RegDst, inst_20_16, inst_15_11), \
				Mux(control.MemtoReg, alu.ALU_result, mem_result));

		branch_addr = Branch(pc_add_4, Sign_Extend(inst_15_0));
		jump_addr = Jump(pc_add_4, inst_25_0);
		pc = Mux(control.Jump, Mux(control.Branch && alu.zero, pc_add_4, branch_addr), jump_addr);

		total_cycle++;

		// status print
		/********************************/
		printf("Operation:       %d\n", inst_31_26);
		printf("Register rs:     %d\n", inst_25_21);
		printf("Register rt:     %d\n", inst_20_16);
		printf("Register rd:     %d\n", inst_15_11);
		printf("Shift amount:    %d\n", inst_15_0 & 0x07c0);
		printf("R-function:      %d\n", inst_15_0 & 0x003f);
		printf("Imme/Addr(15-0): %d\n", inst_15_0);
		printf("Addr(26-0):      %d\n", inst_25_0);
		printf("\n");

		print_control_state();
		printf("\n");

		print_oper(inst_31_26);
		print_alu_control(ALU_control);
		printf("\n");

		printf("reg_read.Read_reg_1: %d\n", reg_read.Read_data_1);
		printf("reg_read.Read_reg_2: %d\n", reg_read.Read_data_2);
		printf("\n");

		printf("alu.ALU_result: %d\n", alu.ALU_result);
		printf("mem_result:     %d\n", mem_result);
		printf("Branch addr:    %d\n", branch_addr);
		printf("Jump addr:      %d\n", jump_addr);
		printf("\n");
		/********************************/

		// result
		/********************************/
		print_reg_mem();
		printf("PC :          %d\n", pc);
		printf("Total cycle : %d\n", total_cycle);
		/********************************/
	}

	printf("\n ***** Processor END !!! ***** \n");
	return (0);
}

unsigned int Inst_Fetch(unsigned int read_addr)
{
	return (mem[read_addr]);
}

void Register_Read(unsigned int read_reg_1, unsigned int read_reg_2)
{
	reg_read.Read_data_1 = reg[read_reg_1];
	reg_read.Read_data_2 = reg[read_reg_2];
}

void Control_Signal(unsigned int opcode)
{
	unsigned int	opcode_5 = opcode & 0x20;
	unsigned int	opcode_4 = opcode & 0x10;
	unsigned int	opcode_3 = opcode & 0x08;
	unsigned int	opcode_2 = opcode & 0x04;
	unsigned int	opcode_1 = opcode & 0x02;
	unsigned int	opcode_0 = opcode & 0x01;

	unsigned int	R_format	= !opcode_5 && !opcode_4 && !opcode_3 && !opcode_2 && !opcode_1 && !opcode_0;
	unsigned int	BEQ			= !opcode_5 && !opcode_4 && !opcode_3 && opcode_2 && !opcode_1 && !opcode_0;
	unsigned int	Lw			= opcode_5 && !opcode_4 && !opcode_3 && !opcode_2 && opcode_1 && opcode_0;
	unsigned int	Sw			= opcode_5 && !opcode_4 && opcode_3 && !opcode_2 && opcode_1 && opcode_0;
	unsigned int	Jump		= !opcode_5 && !opcode_4 && !opcode_3 && !opcode_2 && opcode_1 && !opcode_0;

	control.RegDst		= R_format;
	control.Jump		= Jump;
	control.Branch		= BEQ;
	control.MemRead		= Lw;
	control.MemtoReg	= Lw;
	control.ALUOp		= (R_format << 1) + BEQ;
	control.MemWrite	= Sw;
	control.ALUSrc		= Lw || Sw;
	control.RegWrite	= R_format || Lw;
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
		alu.ALU_result = NOR(a, b);
	else if (ALU_control & 0x04)
		alu.ALU_result = Sub(a, b);
	else if (ALU_control & 0x02)
		alu.ALU_result = Add(a, b);
	else if (ALU_control & 0x01)
		alu.ALU_result = OR(a, b);
	else
		alu.ALU_result = AND(a, b);
	alu.zero = Slt(a, b);
}

unsigned int Memory_Access(unsigned char MemWrite, unsigned char MemRead, unsigned int addr, unsigned int write_data)
{
	if (MemWrite)
		mem[addr] = write_data;
	if (MemRead)
		return (mem[addr]);
	return (write_data);
}

void	Register_Write(unsigned char RegWrite, unsigned int Write_reg, unsigned int Write_data)
{
	if (RegWrite)
		reg[Write_reg] = Write_data;
}

unsigned int Sign_Extend(unsigned int inst_16)
{
	unsigned int	inst_32 = 0;

	if ((inst_16 & 0x00008000))
		inst_32 = inst_16 | 0xffff0000;
	else
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

unsigned int	Sub(unsigned int a, unsigned int b)
{
	return (a - b);
}

unsigned int	AND(unsigned int a, unsigned int b)
{
	return (a & b);
}

unsigned int	OR(unsigned int a, unsigned int b)
{
	return (a | b);
}

unsigned int	NOR(unsigned int a, unsigned int b)
{
	return (!(a | b));
}

unsigned int	Slt(unsigned int a, unsigned int b)
{
	return (a < b);
}

unsigned int	Branch(unsigned int pc_add_4, unsigned int addr)
{
	return (pc_add_4 + (addr << 2));
}

unsigned int	Jump(unsigned int pc_add_4, unsigned int inst_25_0)
{
	return (pc_add_4 & 0xf0000000 | (inst_25_0 << 2));
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




void print_control_state(void)
{
	printf("RegDst:   %d\n", control.RegDst);
	printf("Jump:     %d\n", control.Jump);
	printf("Branch:   %d\n", control.Branch);
	printf("MemRead:  %d\n", control.MemRead);
	printf("MemtoReg: %d\n", control.MemtoReg);
	printf("ALUOp:    %d\n", control.ALUOp);
	printf("MemWrite: %d\n", control.MemWrite);
	printf("ALUSrc:   %d\n", control.ALUSrc);
	printf("RegWrite: %d\n", control.RegWrite);
}

void print_oper(unsigned int inst_31_26)
{
	printf("Oper:        ");
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

		default:
		printf("Wrong Opcode\n");
		break;
	}
}

void	print_alu_control(unsigned int ALU_control)
{
	printf("ALU_control: ");
	switch (ALU_control)
	{
		case 0:
		printf("AND\n");
		break;

		case 1:
		printf("OR\n");
		break;

		case 2:
		printf("ADD\n");
		break;

		case 6:
		printf("SUB\n");
		break;

		case 7:
		printf("SLT\n");
		break;

		case 12:
		printf("NOR\n");
		break;
	}
}
