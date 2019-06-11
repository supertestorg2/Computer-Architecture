#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"

#define DATA_MAX RAM_LEN

byte cpu_ram_read(struct cpu *cpu, int index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, byte value) {
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_name) {
  FILE *fp = fopen(file_name, "r");
  char *line = NULL;
  int address = 0, j = 0, line_len;
  size_t len = 0;
  byte data[DATA_MAX];

  while ((line_len = getline(&line, &len, fp)) != -1) {
    for (int i = 0; i < line_len; i++)
      if (line[i] == '\n' || line[i] == ' ' || line[i] == '#') {
        line[i] = '\0';
        break;
      }
    
    // printf("\n%s\n", line);
    if (strlen(line) != 0)
      data[j++] = strtoul(line, NULL, 2);
  }

  for (int i = 0; i < j; i++)
    cpu->ram[address++] = data[i];

  fclose(fp);
}

/**
 * ALU
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void alu(struct cpu *cpu, enum alu_op op, byte regA, byte regB) {
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
  }
}
#pragma GCC diagnostic pop

int is_bit_set(byte b, int n) {
  return b & (1 << n);
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu) {
  byte instruction, operand1, operand2;
  int running = 1, num_operands;

  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    cpu->ir = cpu->pc++;
    instruction = cpu_ram_read(cpu, cpu->ir);

    // 2. Figure out how many operands this next instruction requires
    num_operands = is_bit_set(instruction, 7) ? 2 
      : is_bit_set(instruction, 6) ? 1 
      : 0;

    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands > 0) {
      operand1 = cpu_ram_read(cpu, cpu->pc++);
      if (num_operands > 1)
        operand2 = cpu_ram_read(cpu, cpu->pc++);
    }

    // 4. switch() over it to decide on a course of action.
    switch (instruction) {
      case LDI:
        cpu->registers[operand1] = operand2;
        break;
      
      case PRN:
        printf("%d\n", cpu->registers[operand1]);
        break;
      
      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operand1, operand2);
        break;
      
      case ADD:
        alu(cpu, ALU_ADD, operand1, operand2);
        break;

      // TODO:
      // implement more instructions
      
      default:
        printf("An instruction occurred that has not yet been implemented.\n");
        break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu) {
  cpu->pc = 0;
  cpu->ir = 0;
  cpu->mar = 0;
  cpu->mdr = 0;
  cpu->fl = 0;

  // TODO: Initialize the stack pointer separately,
  // per https://github.com/johnoro/Computer-Architecture/blob/master/LS8-spec.md#registers
  // and https://github.com/johnoro/Computer-Architecture/tree/master/ls8#step-3-implement-the-core-of-cpu_init
  memset(cpu->registers, 0, REGISTERS_LEN);
  memset(cpu->ram, 0, RAM_LEN);
}
