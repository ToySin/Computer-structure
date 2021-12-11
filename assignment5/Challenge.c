#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**************************************/

struct Control
{
	unsigned char	RegDst;
	unsigned char	ALUOp;
	unsigned char	ALUSrc;
	unsigned char	Jump;
	unsigned char	MemRead;
	unsigned char	MemWrite;
	unsigned char	Branch;
	unsigned char	RegWrite;
	unsigned char	MemtoReg;
	unsigned char	IF_Flush;
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

struct IF_ID
{
	unsigned int	is_activated;

	unsigned int	pc_add_4;
	unsigned int	inst;

	unsigned int	inst_31_26_for_print;
};

struct ID_EX
{
	unsigned int	is_activated;

	unsigned char	RegDst;
	unsigned char	ALUOp;
	unsigned char	ALUSrc;
	unsigned char	Jump;
	unsigned char	MemRead;
	unsigned char	MemWrite;
	unsigned char	Branch;
	unsigned char	RegWrite;
	unsigned char	MemtoReg;

	unsigned int	RegRs;
	unsigned int	RegRt;
	unsigned int	RegRd;
	unsigned int	extended_immediate;
	struct Reg_Read	reg_read;

	unsigned int	inst_31_26_for_print;
};

struct EX_MEM
{
	unsigned int	is_activated;

	unsigned char	MemRead;
	unsigned char	MemWrite;
	unsigned char	Branch;
	unsigned char	RegWrite;
	unsigned char	MemtoReg;

	unsigned int	branch_addr;
	struct ALU		alu;
	unsigned int	Write_data;
	unsigned int	RegRd;

	unsigned int	inst_31_26_for_print;
};

struct MEM_WB
{
	unsigned int	is_activated;

	unsigned char	RegWrite;
	unsigned char	MemtoReg;

	unsigned int	MEM_result;
	unsigned int	ALU_result;
	unsigned int	RegRd;

	unsigned int	inst_31_26_for_print;
};

struct ALU_FWD_Unit
{
	unsigned char	Forwarding_Rs;
	unsigned char	Forwarding_Rt;
};

struct Branch_FWD_Unit
{
	unsigned char	Forwarding_Rs;
	unsigned char	Forwarding_Rt;
};

struct Hazard_Detection_Unit
{
	unsigned int	PCWrite;
	unsigned int	IF_IDWrite;
	unsigned int	Control_nop;
};

struct Control					control = { 0 };
struct Reg_Read					reg_read = { 0 };
struct ALU						alu = { 0 };
struct IF_ID					if_id = { 0 };
struct ID_EX					id_ex = { 0 };
struct EX_MEM					ex_mem = { 0 };
struct MEM_WB					mem_wb = { 0 };
struct ALU_FWD_Unit				alu_fwd_unit = { 0 };
struct Branch_FWD_Unit			branch_fwd_unit = { 0 };
struct Hazard_Detection_Unit	hd_unit = { 0 };
unsigned int					mem[64] = { 0 };
unsigned int					reg[32] = { 0 };
char							*None = "None";

/**************************************/

unsigned int	Inst_Fetch(unsigned int read_addr);

void			Hazard_Signal(unsigned int inst_25_21, unsigned int inst_20_16);
void			Register_Read(unsigned int read_reg_1, unsigned int read_reg_2);
void			Control_Signal(unsigned int opcode);
unsigned char	ALU_Control_Signal(unsigned char signal);
void			ALU_FWD_Signal(void);
void			Branch_FWD_Signal(void);
void			ALU_func(unsigned char ALU_control, unsigned int a, unsigned int b);
unsigned int	Memory_Access(unsigned char MemWrite, unsigned char MemRead, unsigned int addr, unsigned int write_data);
void			Register_Write(unsigned char RegWrite, unsigned int Write_reg, unsigned int Write_data);

unsigned int	Sign_Extend(unsigned int inst_16);
unsigned int	Mux(char signal, unsigned int a_0, unsigned int b_1);
unsigned int	Mux421(char signal, unsigned int a_00, unsigned int b_01, unsigned int c_10, unsigned int d_11);
unsigned int	Shift_Left_2(unsigned int inst);
unsigned int	Add(unsigned int a, unsigned int b);
unsigned int	Sub(unsigned int a, unsigned int b);
unsigned int	AND(unsigned int a, unsigned int b);
unsigned int	OR(unsigned int a, unsigned int b);
unsigned int	Slt(unsigned int a, unsigned int b);
unsigned int	NOR(unsigned int a, unsigned int b);


unsigned int	Branch(unsigned int pc_add_4, unsigned int addr);
unsigned int	Jump(unsigned int pc_add_4, unsigned int inst_25_0);

void			Set_IF_ID(unsigned char IF_Flush, unsigned int pc_add_4, unsigned int inst);
void			Set_ID_EX(unsigned int zero_signal, unsigned int inst_25_21, unsigned int inst_20_16, unsigned int inst_15_11, unsigned int extended);
void			Set_EX_MEM(unsigned int branch_addr, unsigned int mem_write_data);
void			Set_MEM_WB(unsigned int mem_result);

void			print_reg_mem(void);
void			print_pipeline_regi_state(unsigned int pc);

void			print_control_state(void);
void			print_oper(unsigned int inst_31_26);
void			print_alu_control(unsigned int ALU_control);

/**************************************/

int main(void)
{
	FILE			*fp;
	int				file_num;
	int				total_cycle = 0;

	unsigned int	pc = 0;
	unsigned int	PCSrc = 0;
	unsigned int	pc_add_4 = 0;
	unsigned int	pc_add_inst = 0;

	unsigned int	inst = 0;
	unsigned int	inst_31_26 = 0;
	unsigned int	inst_25_21 = 0;
	unsigned int	inst_20_16 = 0;
	unsigned int	inst_15_11 = 0;
	unsigned int	inst_15_0 = 0;
	unsigned int	inst_25_0 = 0;
	unsigned int	inst_ext_32 = 0;
	unsigned int	inst_ext_shift = 0;

	unsigned char	ALU_control = 0;
	unsigned int	branch_addr = 0;
	unsigned int	jump_addr = 0;
	unsigned int	mem_result = 0;
	unsigned int	reg_write_data = 0;
	unsigned int	mem_write_data = 0;

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

	pc = 0;

	print_reg_mem();
	printf("\n ***** Processor START !!! ***** \n");

	while (pc < 64 && total_cycle < 8 /*| mem_wb.is_activated*/)
	{
		// WB execute
		reg_write_data = Mux(mem_wb.MemtoReg, mem_wb.ALU_result, mem_wb.MEM_result);
		Register_Write(mem_wb.RegWrite, mem_wb.RegRd, reg_write_data);
		printf("reg_write_data: %d\n", reg_write_data);

		// MEM execute
		PCSrc = ex_mem.Branch && ex_mem.alu.zero;
		mem_result = Memory_Access(ex_mem.MemWrite, ex_mem.MemRead, \
				ex_mem.alu.ALU_result, ex_mem.Write_data);
		printf("mem_result: %d\n", mem_result);

		// EX execute

		ALU_control = ALU_Control_Signal(id_ex.extended_immediate & 0x003f);
		ALU_FWD_Signal();
		mem_write_data = Mux421(alu_fwd_unit.Forwarding_Rt, id_ex.reg_read.Read_data_2, reg_write_data, ex_mem.alu.ALU_result, 0);
		ALU_func(ALU_control, \
				Mux421(alu_fwd_unit.Forwarding_Rs, id_ex.reg_read.Read_data_1, reg_write_data, ex_mem.alu.ALU_result, 0), \
				Mux(id_ex.ALUSrc, mem_write_data, id_ex.extended_immediate));
		printf("Forwarding A B: %d %d\n", alu_fwd_unit.Forwarding_Rs, alu_fwd_unit.Forwarding_Rt);

		// ID execute
		inst_31_26 = if_id.inst >> 26;
		inst_25_21 = (if_id.inst & 0x03e00000) >> 21;
		inst_20_16 = (if_id.inst & 0x001f0000) >> 16;
		inst_15_11 = (if_id.inst & 0x0000f800) >> 11;
		inst_15_0 = if_id.inst & 0x0000ffff;
		inst_25_0 = if_id.inst & 0x03ffffff;
		inst_ext_32 = Sign_Extend(inst_15_0);
		inst_ext_shift = Shift_Left_2(inst_ext_32);

		Register_Read(inst_25_21, inst_20_16);
		Hazard_Signal(inst_25_21, inst_20_16);
		Control_Signal(inst_31_26);

		branch_addr = Branch(if_id.pc_add_4, inst_ext_shift);
		jump_addr = Jump(pc_add_4, inst_25_0);

		// IF execute
		if (pc < 64 && hd_unit.PCWrite)
		{
			pc = Mux(control.Jump, Mux(PCSrc, pc_add_4, ex_mem.branch_addr), jump_addr);
			pc_add_4 = Add(pc, 4);
			inst = Inst_Fetch(pc);
		}

		printf("PC :          %d \n", pc);
		printf("Total cycle : %d \n", total_cycle);
		printf("Instruction : %08x\n", inst);
		printf("\n");

		print_pipeline_regi_state(pc);
		printf("\n");

		// Update pipeline register
		Set_MEM_WB(mem_result);
		Set_EX_MEM(branch_addr, mem_write_data);
		Set_ID_EX(hd_unit.Control_nop, inst_25_21, inst_20_16, inst_15_11, inst_ext_32);
		if (hd_unit.IF_IDWrite)
			Set_IF_ID(control.IF_Flush, pc_add_4, inst);

		total_cycle++;

		// result
		/********************************/
		print_reg_mem();
		/********************************/
	}

	printf("PC :          %d \n", pc);
	printf("Total cycle : %d \n", total_cycle);
	print_pipeline_regi_state(pc);


	printf("\n ***** Processor END !!! ***** \n");
	return (0);
}

unsigned int	Inst_Fetch(unsigned int read_addr)
{
	return (mem[read_addr]);
}

void	Hazard_Signal(unsigned int inst_25_21, unsigned int inst_20_16)
{
	hd_unit.Control_nop = 0;
	hd_unit.IF_IDWrite = 1;
	hd_unit.PCWrite = 1;

	// lw hazard bubble
	if (id_ex.MemRead)
	{
		if ((id_ex.RegRt == inst_25_21) || (id_ex.RegRt == inst_20_16))
		{
			hd_unit.Control_nop = 1;
			hd_unit.IF_IDWrite = 0;
			hd_unit.PCWrite = 0;
		}
	}

	// branch lw hazard bubble (need 2 stall)
	if (control.Branch)
	{
		if (id_ex.MemRead)
		{
			if ((id_ex.RegRt == inst_25_21) || (id_ex.RegRt == inst_20_16))
			{
				hd_unit.Control_nop = 1;
				hd_unit.IF_IDWrite = 0;
				hd_unit.PCWrite = 0;
			}
		}
		else if (ex_mem.MemRead)
		{
			if ((ex_mem.RegRd == inst_25_21) || (ex_mem.RegRd == inst_20_16))
			{
				hd_unit.Control_nop = 1;
				hd_unit.IF_IDWrite = 0;
				hd_unit.PCWrite = 0;
			}
		}
	}
}

void	Register_Read(unsigned int read_reg_1, unsigned int read_reg_2)
{
	reg_read.Read_data_1 = reg[read_reg_1];
	reg_read.Read_data_2 = reg[read_reg_2];
}

void	Control_Signal(unsigned int opcode)
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

void	FWD_Signal(void)
{
	fwd_unit.Forwarding_A = 0;
	fwd_unit.Forwarding_B = 0;
	if (ex_mem.RegWrite && !ex_mem.RegRd)
	{
		if (ex_mem.RegRd == id_ex.RegRs)
			fwd_unit.Forwarding_A = 2;
		else if (ex_mem.RegRd == id_ex.RegRt)
			fwd_unit.Forwarding_B = 2;
	}
	else if (mem_wb.RegWrite && !mem_wb.RegRd) // if-else 구조로 double forwarding 문제를 해결함
	{
		if (mem_wb.RegRd == id_ex.RegRs)
			fwd_unit.Forwarding_A = 1;
		else if (mem_wb.RegRd == id_ex.RegRt)
			fwd_unit.Forwarding_B = 1;
	}
}

unsigned char	ALU_Control_Signal(unsigned char signal)
{
	unsigned char	ALU_control = 0;

	ALU_control += (id_ex.ALUOp & 0x01) || ((id_ex.ALUOp & 0x02) && (signal & 0x02));
	ALU_control = ALU_control << 1;
	ALU_control += !(id_ex.ALUOp & 0x02) || !(signal & 0x04);
	ALU_control = ALU_control << 1;
	ALU_control += (id_ex.ALUOp & 0x02) && ((signal & 0x08) || (signal & 0x01));
	return (ALU_control);
}

void	ALU_func(unsigned char ALU_control, unsigned int a, unsigned int b)
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
	alu.zero = Slt(Sub(a, b), 1);
}

unsigned int	Memory_Access(unsigned char MemWrite, unsigned char MemRead, unsigned int addr, unsigned int write_data)
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

unsigned int	Sign_Extend(unsigned int inst_16)
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

unsigned int	Mux421(char signal, unsigned int a_00, unsigned int b_01, unsigned int c_10, unsigned d_11)
{
	return (signal & 0x01 ? (signal & 0x02 ? d_11 : b_01) : (signal & 0x02 ? c_10 : a_00));
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

void	Set_IF_ID(unsigned char IF_Flush, unsigned int pc_add_4, unsigned int inst)
{
	if (0 <= pc_add_4 && pc_add_4 < 64 && !IF_Flush)
		if_id.is_activated = 1;
	else
		if_id.is_activated = 0;

	if_id.pc_add_4 = IF_Flush ? 0 : pc_add_4;
	if_id.inst = IF_Flush ? 0 : inst;

	if_id.inst_31_26_for_print = IF_Flush ? 0 : inst >> 26;
}

void	Set_ID_EX(unsigned int zero_signal, unsigned int inst_25_21, unsigned int inst_20_16, unsigned int inst_15_11, unsigned int extended)
{
	id_ex.is_activated = zero_signal ? 0 : if_id.is_activated;

	id_ex.RegDst = control.RegDst;
	id_ex.ALUOp = control.ALUOp;
	id_ex.ALUSrc = control.ALUSrc;
	id_ex.Jump = zero_signal ? 0 : control.Jump;
	id_ex.MemRead = control.MemRead;
	id_ex.MemWrite = zero_signal ? 0 : control.MemWrite;
	id_ex.Branch = zero_signal ? 0 : control.Branch;
	id_ex.RegWrite = zero_signal ? 0 : control.RegWrite;
	id_ex.MemtoReg = control.MemtoReg;

	id_ex.RegRs = inst_25_21;
	id_ex.RegRt = inst_20_16;
	id_ex.RegRd = inst_15_11;
	id_ex.extended_immediate = extended;
	id_ex.reg_read = reg_read;

	id_ex.inst_31_26_for_print = if_id.inst_31_26_for_print;
}

void	Set_EX_MEM(unsigned int branch_addr, unsigned int mem_write_data)
{
	ex_mem.is_activated = id_ex.is_activated;

	ex_mem.MemRead = id_ex.MemRead;
	ex_mem.MemWrite = id_ex.MemWrite;
	ex_mem.Branch = id_ex.Branch;
	ex_mem.RegWrite = id_ex.RegWrite;
	ex_mem.MemtoReg = id_ex.MemtoReg;

	ex_mem.branch_addr = branch_addr;
	ex_mem.alu = alu;
	ex_mem.Write_data = mem_write_data;
	ex_mem.RegRd = Mux(id_ex.RegDst, id_ex.RegRt, id_ex.RegRd);

	ex_mem.inst_31_26_for_print = id_ex.inst_31_26_for_print;
}

void	Set_MEM_WB(unsigned int mem_result)
{
	mem_wb.is_activated = ex_mem.is_activated;

	mem_wb.MemtoReg = ex_mem.MemtoReg;
	mem_wb.RegWrite = ex_mem.RegWrite;

	mem_wb.MEM_result = mem_result;
	mem_wb.ALU_result = ex_mem.alu.ALU_result;
	mem_wb.RegRd = ex_mem.RegRd;

	mem_wb.inst_31_26_for_print = ex_mem.inst_31_26_for_print;
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

void	print_pipeline_regi_state(unsigned int pc)
{
	printf("             | ==== IF/ID ==== | ==== Control ==== | ==== ID/EX ==== | ==== EX/MEM ==== | ==== MEM/WB ==== \n");
	printf("-------------+-----------------+-------------------+-----------------+------------------+------------------┐ \n");
	printf("   Activated | %4d   | %18d                  | %8d        | %9d         | %4d   |\n", pc < 64, if_id.is_activated, id_ex.is_activated, ex_mem.is_activated, mem_wb.is_activated);
	printf("-------------+-----------------+-------------------+-----------------+------------------+------------------┐ \n");
	printf("      RegDst | %15s | %17d | %15d | %16s | %16s |\n", None, control.RegDst, id_ex.RegDst, None, None);
	printf("        Jump | %15s | %17d | %15d | %16s | %16s |\n", None, control.Jump, id_ex.Jump, None, None);
	printf("      Branch | %15s | %17d | %15d | %16d | %16s |\n", None, control.Branch, id_ex.Branch, ex_mem.Branch, None);
	printf("     MemRead | %15s | %17d | %15d | %16d | %16s |\n", None, control.MemRead, id_ex.MemRead, ex_mem.MemRead, None);
	printf("    MemtoReg | %15s | %17d | %15d | %16d | %16d |\n", None, control.MemtoReg, id_ex.MemtoReg, ex_mem.MemtoReg, mem_wb.MemtoReg);
	printf("       ALUOp | %15s | %17d | %15d | %16s | %16s |\n", None, control.ALUOp, id_ex.ALUOp, None, None);
	printf("    MemWrite | %15s | %17d | %15d | %16d | %16s |\n", None, control.MemWrite, id_ex.MemWrite, ex_mem.MemWrite, None);
	printf("      ALUSrc | %15s | %17d | %15d | %16s | %16s |\n", None, control.ALUSrc, id_ex.ALUSrc, None, None);
	printf("    RegWrite | %15s | %17d | %15d | %16d | %16d |\n", None, control.RegWrite, id_ex.RegWrite, ex_mem.RegWrite, mem_wb.RegWrite);
	printf("-------------+-----------------+-------------------+-----------------+------------------+------------------┤ \n");
	printf("      Opcode | %15d | %17d | %15d | %16d | %16d |\n", if_id.inst_31_26_for_print, if_id.inst_31_26_for_print, id_ex.inst_31_26_for_print, ex_mem.inst_31_26_for_print, mem_wb.inst_31_26_for_print);
	printf("  Reg_read_1 | %15s | %17s | %15d | %16s | %16s |\n", None, None, id_ex.reg_read.Read_data_1 , None, None);
	printf("  Reg_read_2 | %15s | %17s | %15d | %16d | %16s |\n", None, None, id_ex.reg_read.Read_data_2 , ex_mem.Write_data, None);
	printf("       RegRs | %15s | %17s | %15d | %16s | %16s |\n", None, None, id_ex.RegRs, None, None);
	printf("       RegRt | %15s | %17s | %15d | %16s | %16s |\n", None, None, id_ex.RegRt, None, None);
	printf("       RegRd | %15s | %17s | %15d | %16d | %16d |\n", None, None, id_ex.RegRd, ex_mem.RegRd , mem_wb.RegRd);
	printf("  ALU_result | %15s | %17s | %15s | %16d | %16d |\n", None, None, None, ex_mem.alu.ALU_result, mem_wb.ALU_result);
	printf("    ALU_zero | %15s | %17s | %15s | %16d | %16s |\n", None, None, None, ex_mem.alu.zero, None);
	printf("  MEM_result | %15s | %17s | %15s | %16s | %16d |\n", None, None, None, None, mem_wb.MEM_result);
	printf(" Branch_addr | %15s | %17s | %15s | %16d | %16s |\n", None, None, None, ex_mem.branch_addr, None);
	printf("-------------+-----------------+-------------------+-----------------+------------------+------------------┘ \n");
}

