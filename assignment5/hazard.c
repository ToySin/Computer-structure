#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

struct IF
{
	unsigned int	pc;
	unsigned int	pc_add_4;
	unsigned int	inst;
};

struct IF_ID
{
	unsigned int	pc_add_4;
	unsigned int	inst;
};

struct ID
{
	unsigned int	opcode;
	unsigned int	RegRs;
	unsigned int	RegRt;
	unsigned int	RegRd;
	unsigned int	Immediate;
	unsigned int	Branch_addr;
	unsigned int	Jump_addr;
	struct Control	control;
};

struct ID_EX
{
	unsigned int	Reg_data_rs;
	unsigned int	Reg_data_rt;
	unsigned int	Expended_addr;
	unsigned int	RegRs;
	unsigned int	RegRt;
	unsigned int	RegRd;
	struct Control	control;
};

struct EX
{

}


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
