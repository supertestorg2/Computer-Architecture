#ifndef _ALU_H_
#define _ALU_H_

#include "cpu.h"
#include "byte.h"

// ALU operations
enum alu_op {
  ALU_NULL,
  ALU_ADD,
  ALU_SUB,
  ALU_INC,
  ALU_DEC,
	ALU_MUL,
  ALU_DIV,
  ALU_MOD,
  ALU_NOT,
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_SHL,
  ALU_SHR,
  ALU_CMP
	// Add more here
};

enum alu_op get_op(byte instruction);

void alu(struct cpu *cpu, enum alu_op op, byte regA, byte regB);

void handle_alu_op(struct cpu *cpu, byte instruction, byte regA, byte regB);

#endif
