#pragma once
#include <stdio.h>
#include <malloc.h>

#define STACK_SIZE 64

#define PUSH(vm, v) vm->stack[++vm->stackPointer] = v
#define POP(vm) vm->stack[vm->stackPointer--]
#define NCODE(vm) vm->code[vm->programCounter++]
#define FROMREG(vm, addr) vm->registers[addr]
#define TOREG(vm, addr, v) vm->registers[addr] = v

enum {
	ADD, /* Adds two integer values */
	SUB,
	MUL,
	DIV,
	LT,
	GT,
	EQ,
	JMP,
	JMPT,
	JMPF,
	CONST_I32,
	LOAD,
	GLOAD,
	STORE,
	GSTORE,
	PRINT,
	POP,
	HALT,
	CALL,
	RET,
	MOV
};

typedef union {
	int iCode;
	float fCode;
	double dCode;
} Opcode;

struct VirtualMachine {
	double* registers;
	Opcode* code;
	int* stack;
	int programCounter;
	int stackPointer;
	int framePointer;
};

VirtualMachine* newVM(Opcode* code, int programCounter, int dataSize) {
	VirtualMachine* vm = (VirtualMachine*)malloc(sizeof(VirtualMachine));
	vm->code = code;
	vm->programCounter = programCounter;
	vm->framePointer = 0;
	vm->stackPointer = -1;
	vm->registers = (double*)malloc(sizeof(double) * dataSize);
	vm->stack = (int*)malloc(sizeof(int) * STACK_SIZE);

	return vm;
}

void delVM(VirtualMachine* vm) {
	free(vm->registers);
	free(vm->code);
	free(vm->stack);
	free(vm);
}

void runVM(VirtualMachine* vm) {
	while (true) {
		int opcode = NCODE(vm).iCode;
		int address, altAddress, offset, argc, rval;
		double v, a, b;
		switch (opcode)
		{
		case HALT: return; //terminate
		case CONST_I32:
			PUSH(vm, NCODE(vm).iCode);
			break;
		case ADD:
			a = FROMREG(vm, NCODE(vm).iCode);
			b = FROMREG(vm, NCODE(vm).iCode);
			address = NCODE(vm).iCode;
			TOREG(vm, address, a + b);
			break;
		case SUB:
			a = FROMREG(vm, NCODE(vm).iCode);
			b = FROMREG(vm, NCODE(vm).iCode);
			address = NCODE(vm).iCode;
			TOREG(vm, address, a - b);
			break;
		case MUL:
			a = FROMREG(vm, NCODE(vm).iCode);
			b = FROMREG(vm, NCODE(vm).iCode);
			address = NCODE(vm).iCode;
			TOREG(vm, address, a * b);
			break;
		case DIV:
			a = FROMREG(vm, NCODE(vm).iCode);
			b = FROMREG(vm, NCODE(vm).iCode);
			address = NCODE(vm).iCode;
			TOREG(vm, address, a / b);
			break;
		case MOV:
			address = NCODE(vm).iCode;
			v = NCODE(vm).dCode;
			TOREG(vm, address, v);
			break;
		case LT: 
			a = FROMREG(vm, NCODE(vm).iCode);
			b = FROMREG(vm, NCODE(vm).iCode);
			PUSH(vm, (a < b) ? 1 : 0);
			break;
		case GT:
			a = FROMREG(vm, NCODE(vm).iCode);
			b = FROMREG(vm, NCODE(vm).iCode);
			PUSH(vm, (a > b) ? 1 : 0);
			break;
		case EQ:
			a = FROMREG(vm, NCODE(vm).iCode);
			b = FROMREG(vm, NCODE(vm).iCode);
			PUSH(vm, (a == b) ? 1 : 0);
			break;
		case JMP:
			vm->programCounter = NCODE(vm).iCode;
			break;
		case JMPT:
			address = NCODE(vm).iCode;
			altAddress = NCODE(vm).iCode;
			if (POP(vm)) {
				vm->programCounter = address;
			}
			else {
				vm->programCounter = altAddress;
			}
			break;
		case JMPF:
			address = NCODE(vm).iCode;
			altAddress = NCODE(vm).iCode;
			if (!POP(vm)) {
				vm->programCounter = address;
			}
			else {
				vm->programCounter = altAddress;
			}
			break;
		case LOAD:
			offset = NCODE(vm).iCode;
			PUSH(vm, vm->stack[vm->framePointer + offset]);
			break;
		case GLOAD:
			address = NCODE(vm).iCode;
			v = vm->registers[address];
			PUSH(vm, v);
			break;
		case STORE:
			v = POP(vm);
			offset = NCODE(vm).iCode;
			vm->registers[vm->framePointer + offset] = v;
			break;
		case GSTORE:
			v = POP(vm);
			address = NCODE(vm).iCode;
			vm->registers[address] = v;
			break;
		case CALL:
			address = NCODE(vm).iCode;
			argc = NCODE(vm).iCode;
			PUSH(vm, argc);
			PUSH(vm, vm->framePointer);
			PUSH(vm, vm->programCounter);
			vm->framePointer = vm->stackPointer;
			vm->programCounter = address;
			break;
		case RET:
			rval = POP(vm);
			vm->stackPointer = vm->framePointer;
			vm->programCounter = POP(vm);
			vm->framePointer = POP(vm);
			argc = POP(vm);
			vm->stackPointer -= argc;
			PUSH(vm, rval);
			break;
		case POP: 
			--vm->stackPointer;
			break;
		case PRINT:
			address = NCODE(vm).iCode;
			v = FROMREG(vm, address);
			std::cout << v << std::endl;
			break;
		default:
			break;
		}
	}
}