#ifndef ISADEFS_H
#define ISADEFS_H
#include <assert.h>


// For different types, always source, then immediate
typedef uint8_t Opcode;
enum {
  INVALID = 0,
  NOOP,
  LOAD,
  STORE,
  B,
  BEQ,
  BNEQ,
  CMP_SRC,
  CMP_IMM,

  MOV_REG,
  MOV_IMM_TO_LO,
  MOV_IMM_TO_HI,

  ADD_SRC_SRC,
  ADD_SRC_IMM,
  SUB_SRC_SRC,
  SUB_SRC_IMM,
  MUL_SRC_SRC,
  MUL_SRC_IMM,
  DIV_SRC_SRC,
  DIV_SRC_IMM,
  MOD_SRC_SRC,
  MOD_SRC_IMM,
  LSH_SRC_SRC,
  LSH_SRC_IMM,
  RSH_SRC_SRC,
  RSH_SRC_IMM,
  AND_SRC_SRC,
  AND_SRC_IMM,
  OR_SRC_SRC,
  OR_SRC_IMM,
  XOR_SRC_SRC,
  XOR_SRC_IMM,

  OPCODE_COUNT,
};

typedef struct Instruction {
  Opcode opcode;
  uint8_t regA;// Destination
  uint8_t regB;
  uint8_t regC;
  uint32_t val;
} Instruction;

typedef union InstructionBits {
  uint64_t raw;
  Instruction ins;
} InstructionBits;

static_assert(OPCODE_COUNT < 256, "Too many opcodes, won't fit in one byte");
static_assert(sizeof(Instruction) == 8, "Instruction struct must be 8 bytes");
static_assert(sizeof(InstructionBits) == 8,
    "InstructionBits union must be 8 bytes");



#endif // ISADEFS_H
