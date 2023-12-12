#!/usr/bin/python3

"""
CS-UY 2214
Adapted from Jeff Epstein
Starter code for E20 simulator
sim.py
"""

from collections import namedtuple
import re
import argparse

# Some helpful constant values that we'll be using.
Constants = namedtuple("Constants",["NUM_REGS", "MEM_SIZE", "REG_SIZE"])
constants = Constants(NUM_REGS = 8,
                      MEM_SIZE = 2**13,
                      REG_SIZE = 2**16)

def load_machine_code(machine_code, mem):
    """
    Loads an E20 machine code file into the list
    provided by mem. We assume that mem is
    large enough to hold the values in the machine
    code file.
    sig: list(str) -> list(int) -> NoneType
    """
    machine_code_re = re.compile("^ram\[(\d+)\] = 16'b(\d+);.*$")
    expectedaddr = 0
    for line in machine_code:
        match = machine_code_re.match(line)
        if not match:
            raise ValueError("Can't parse line: %s" % line)
        addr, instr = match.groups()
        addr = int(addr,10)
        instr = int(instr,2)
        if addr != expectedaddr:
            raise ValueError("Memory addresses encountered out of sequence: %s" % addr)
        if addr >= len(mem):
            raise ValueError("Program too big for memory")
        expectedaddr += 1
        mem[addr] = instr

def print_state(pc, regs, memory, memquantity):
    """
    Prints the current state of the simulator, including
    the current program counter, the current register values,
    and the first memquantity elements of memory.
    sig: int -> list(int) -> list(int) - int -> NoneType
    """
    print("Final state:")
    print("\tpc="+format(pc,"5d"))
    for reg, regval in enumerate(regs):
        print(("\t$%s=" % reg)+format(regval,"5d"))
    line = ""
    for count in range(memquantity):
        line += format(memory[count], "04x")+ " "
        if count % 8 == 7:
            print(line)
            line = ""
    if line != "":
        print(line)

def three_reg_instruction(mem, regs, regA, regB, dst, imm, pc, pcIncr):
    """
    Performs operations for three register operations. The operation is determined by the value of the immediate.
    Returns the program counter increment (1 is default)
    Input: 
        mem: memory array (list)
        regs: register array (list)
        regA: first input register number (integer)
        regB: second input register number (integer)
        dst: destination output register number (integer)
        imm: immedate value (integer)
        pc: current program counter (integer)
        pcIncr: current program counter increment (integer)
    Output:
        pcIncr: program counter increment (1 by default)
    """
    if imm == 0:
        regs[dst] = regs[regA] + regs[regB]
    elif imm == 1:
        regs[dst] = regs[regA] - regs[regB]
    elif imm == 2:
        regs[dst] = regs[regA] | regs[regB]
    elif imm == 3:
        regs[dst] = regs[regA] & regs[regB]
    elif imm == 4:
        regs[dst] = int(regs[regA] < regs[regB])
    elif imm == 8:
        pcIncr = regs[regA] - pc
        return pcIncr
    return pcIncr

def two_reg_instruction(mem, opCode, regs, regA, regB, imm, pcIncr):
    """
    Performs two register operations, except addi operation. The opCode will determine which operation to perform.
    Returns the program counter increment (1 by default). 
    Input:
        mem: memory array (list)
        opCode: operation code (integer)
        regs: register array (list)
        regA: first register number (integer)
        regB: second register number (integer)
        imm: immediate value (integer)
        pcIncr: program counter increment (integer)
    """
    if opCode == 7:
        regs[regB] = int(regs[regA] < imm)

    elif opCode == 4:
        addr = (regs[regA] + imm) % constants.MEM_SIZE
        regs[regB] = mem[addr]

    elif opCode == 5:
        addr = (regs[regA] + imm) % constants.MEM_SIZE
        mem[addr] = regs[regB]

    elif opCode == 6:
        if (regs[regA] == regs[regB]):
            pcIncr += imm
            return pcIncr
    return pcIncr

def twos_complement(value, num_bit):
    """
    Converts positive integer into negative number if the value is greater than (2 ** (num_bit - 1) - 1).
    Returns the 2's complement value.
    Input: 
        value: integer
        num_bit: integer
    Output: 
        value: 2's complement (integer)
    """
    if (value > (2**(num_bit - 1) - 1)):
        return value - 2**num_bit
    return value

def main():
    parser = argparse.ArgumentParser(description='Simulate E20 machine')
    parser.add_argument('filename', help='The file containing machine code, typically with .bin suffix')
    cmdline = parser.parse_args()

    with open(cmdline.filename) as file:
        mem = [0] * constants.MEM_SIZE
        load_machine_code(file, mem)

    pc = 0
    regs = [0] * constants.NUM_REGS
    """
    When jump instruction has an immediate value same as the pc, break out of the while loop.
    Since the opcode for jmp is 010, subtract the 2**14 from the instruction. This only leaves
    the immediate value. Compare the immediate with the pc.
    """
    while (pc != (mem[pc] - 2**14)):
        """
        When extracting the opcode, registers, and immediate values, use >> and << for bitwise
        masking. After extracting each part of the instruction, based on the opcode run the 
        operations needed. Update the regs list and mem list if needed based on opcode. 
        """
        opCode = (mem[pc] & (7 << 13)) >> 13
        pcIncr = 1
        if (opCode == 0):
            srcReg1 = (mem[pc] & (7 << 10)) >> 10
            srcReg2 = (mem[pc] & (7 << 7)) >> 7
            dstReg = (mem[pc] & (7 << 4)) >> 4
            imm = (mem[pc] & 15)
            pcIncr = three_reg_instruction(mem, regs, srcReg1, srcReg2, dstReg, imm, pc, pcIncr)
        elif (opCode == 1):
            regA = (mem[pc] & (7 << 10)) >> 10
            regB = (mem[pc] & (7 << 7)) >> 7
            imm = (mem[pc] & 127)
            regs[regB] = regs[regA] + twos_complement(imm, 7)
        elif (opCode > 3):
            regA = (mem[pc] & (7 << 10)) >> 10
            regB = (mem[pc] & (7 << 7)) >> 7
            imm = (mem[pc] & 127)
            pcIncr = two_reg_instruction(mem, opCode, regs, regA, regB, twos_complement(imm, 7), pcIncr)
        elif (opCode <= 3):
            imm = (mem[pc] & 8191)
            if (opCode == 2):
                pcIncr = imm - pc
            elif (opCode == 3):
                regs[7] = pc + 1
                pcIncr = imm - pc

        regs[0] = 0 # In case register 0 has been updated, return the value of register 0 to 0.

        for i in range(len(regs)): 
            # All values in the register are unsigned. 
            # If the value is negative, add 2**16 (since register values are 16 bits wide).
            if regs[i] < 0:
                regs[i] += 2**16
            # If register value is larger than 16 bits, find the modulus of the value and the size of the register (2**16).
            regs[i] = regs[i] % constants.REG_SIZE
        pc += pcIncr
        pc = pc % constants.MEM_SIZE
        
    print_state(pc, regs, mem, 128)

if __name__ == "__main__":
    main()
#ra0Eequ6ucie6Jei0koh6phishohm9
