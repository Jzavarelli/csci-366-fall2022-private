#include "lmsm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//======================================================
//  Instruction Implementation
//======================================================

void lmsm_i_call(lmsm *our_little_machine)
{
    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    new->value = 0;

    our_little_machine->call_stack->value = our_little_machine->program_counter;
    our_little_machine->program_counter = current->value;
    our_little_machine->accumulator->value = new->value;
}

void lmsm_i_return(lmsm *our_little_machine)
{
    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    new->value = 0;

    our_little_machine->program_counter = our_little_machine->call_stack->value;
    our_little_machine->call_stack->value = new->value;
}

void lmsm_i_push(lmsm *our_little_machine)
{
    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    new->value = 0;
    new->next = current;
    our_little_machine->accumulator = new;
}

void lmsm_i_pop(lmsm *our_little_machine) /* POP Function - May Produce Errors Later, do not know if everything should cause an error on POP. */
{
    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;

    if (current == NULL)
    {
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;
    }
    else if(current != NULL && next == NULL)
    {
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;
    }

    if (next != NULL && our_little_machine->status != STATUS_HALTED)
    {
        our_little_machine->accumulator = next;
    }
    else if(next == NULL && current == NULL)
    {
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;
    }

}

void lmsm_i_dup(lmsm *our_little_machine)
{
    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    new->value = current->value;
    new->next = current;
    our_little_machine->accumulator = new;
}

int check_stack(lmsm *our_little_machine)
{
    lmsm_stack *current = our_little_machine->accumulator;

    if (current == NULL)
    {
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;

        return 0;
    }

    lmsm_stack *next = current->next;

    if (next == NULL)
    {
        our_little_machine->status = STATUS_HALTED;
        our_little_machine->error_code = ERROR_EMPTY_STACK;

        return 0;
    }

    return 1;
}

void lmsm_i_sadd(lmsm *our_little_machine)
{
    if (!check_stack(our_little_machine))
    {
        return;
    }

    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    new->value = current->value + next->value;
    new->next = next->next;
    our_little_machine->accumulator = new;

    free(current);
    free(next);
}

void lmsm_i_ssub(lmsm *our_little_machine)
{
    if (!check_stack(our_little_machine))
    {
        return;
    }

    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    new->value = next->value - current->value;
    new->next = next->next;
    our_little_machine->accumulator = new;

    free(current);
    free(next);
}

void lmsm_i_smax(lmsm *our_little_machine)
{
    if (!check_stack(our_little_machine))
    {
        return;
    }

    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));

    if(current->value > next->value)
    {
        new->value = current->value;
    }
    else
    {
        new->value = next->value;
    }

    new->next = next->next;
    our_little_machine->accumulator = new;

    free(current);
    free(next);
}

void lmsm_i_smin(lmsm *our_little_machine)
{
    if (!check_stack(our_little_machine))
    {
        return;
    }

    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));

    if(current->value < next->value)
    {
        new->value = current->value;
    }
    else
    {
        new->value = next->value;
    }

    new->next = next->next;
    our_little_machine->accumulator = new;

    free(current);
    free(next);
}

void lmsm_i_smul(lmsm *our_little_machine)
{
    if (!check_stack(our_little_machine))
    {
        return;
    }

    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));
    new->value = current->value * next->value;
    new->next = next->next;
    our_little_machine->accumulator = new;

    free(current);
    free(next);
}

void lmsm_i_sdiv(lmsm *our_little_machine)
{
    if (!check_stack(our_little_machine))
    {
        return;
    }

    lmsm_stack *current = our_little_machine->accumulator;
    lmsm_stack *next = current->next;
    lmsm_stack *new = malloc(sizeof(lmsm_stack));

    new->value = (next->value / current->value);
    new->next = next->next;
    our_little_machine->accumulator = new;

    free(current);
    free(next);
}

void lmsm_i_out(lmsm *our_little_machine)
{
    char tempBuffer[100];
    snprintf(tempBuffer, (sizeof tempBuffer), "%d ", our_little_machine->accumulator->value);

    strcat(our_little_machine->output_buffer, tempBuffer);
}

void lmsm_i_inp(lmsm *our_little_machine)
{
    int input;
    printf("Enter an input value: ");
    scanf("%d", &input);

    our_little_machine->accumulator->value = input;
}

void lmsm_i_load(lmsm *our_little_machine, int location)
{
    our_little_machine->accumulator->value = our_little_machine->memory[location];
}

void lmsm_i_add(lmsm *our_little_machine, int location)
{
    our_little_machine->accumulator->value += our_little_machine->memory[location];
}

void lmsm_i_sub(lmsm *our_little_machine, int location)
{
    our_little_machine->accumulator->value -= our_little_machine->memory[location];
}

void lmsm_i_load_immediate(lmsm *our_little_machine, int value)
{
    our_little_machine->accumulator->value = value;
}

void lmsm_i_store(lmsm *our_little_machine, int location)
{
    our_little_machine->memory[location] = our_little_machine->accumulator->value;
}

void lmsm_i_halt(lmsm *our_little_machine)
{
    if(our_little_machine->status != STATUS_HALTED)
    {
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_branch_unconditional(lmsm *our_little_machine, int location)
{
    lmsm_stack *current = our_little_machine->accumulator;

    if(our_little_machine->status != STATUS_HALTED)
    {
        our_little_machine->program_counter = location;
    }
}

void lmsm_i_branch_if_zero(lmsm *our_little_machine, int location)
{
    lmsm_stack *current = our_little_machine->accumulator;

    if(our_little_machine->status != STATUS_HALTED)
    {
        if(current->value == 0)
        {
            our_little_machine->program_counter = location;
        }
    }
}

void lmsm_i_branch_if_positive(lmsm *our_little_machine, int location)
{
    lmsm_stack *current = our_little_machine->accumulator;

    if(our_little_machine->status != STATUS_HALTED)
    {
        if(current->value >= 0)
        {
            our_little_machine->program_counter = location;
        }
    }
}

void lmsm_cap_accumulator_value(lmsm *our_little_machine)
{
    int currentAccumVal = our_little_machine->accumulator->value;

    if(currentAccumVal > 999)
    {
        our_little_machine->accumulator->value = 999;
    }
    if(currentAccumVal < -999)
    {
        our_little_machine->accumulator->value = -999;
    }
}

void lmsm_step(lmsm *our_little_machine)
{
    if(our_little_machine->status != STATUS_HALTED)
    {
        int next_instruction = our_little_machine->memory[our_little_machine->program_counter];
        our_little_machine->program_counter++;

        our_little_machine->current_instruction = next_instruction;
        lmsm_exec_instruction(our_little_machine, next_instruction);
    }
}

//======================================================
//  LMSM Implementation
//======================================================

void lmsm_exec_instruction(lmsm *our_little_machine, int instruction)
{

    // COMPLETE - dispatch the rest of the instruction set and implement
    //        the instructions above

    if (instruction == 0)
    {
        lmsm_i_halt(our_little_machine); //HALT
    }
    else if (100 <= instruction && instruction <= 199)
    {
        lmsm_i_add(our_little_machine, instruction - 100); //ADD
    }
    else if (200 <= instruction && instruction <= 299)
    {
        lmsm_i_sub(our_little_machine, instruction - 200); //SUB
    }
    else if (300 <= instruction && instruction <= 399)
    {
        lmsm_i_store(our_little_machine, instruction - 300); //STA
    }
    else if (400 <= instruction && instruction <= 499)
    {
        lmsm_i_load_immediate(our_little_machine, instruction - 400); //LDI
    }
    else if (500 <= instruction && instruction <= 599)
    {
        lmsm_i_load(our_little_machine, instruction - 500); //LDA
    }
    else if (600 <= instruction && instruction <= 699)
    {
        lmsm_i_branch_unconditional(our_little_machine, instruction - 600); //BRA
    }
    else if (700 <= instruction && instruction <= 799)
    {
        lmsm_i_branch_if_zero(our_little_machine, instruction - 700); //BRZ
    }
    else if (800 <= instruction && instruction <= 899)
    {
        lmsm_i_branch_if_positive(our_little_machine, instruction - 800); //BRP
    }
    else if (instruction == 901)
    {
        lmsm_i_inp(our_little_machine); //INP
    }
    else if (instruction == 902)
    {
        lmsm_i_out(our_little_machine); //OUT
    }
    else if (instruction == 910)
    {
        lmsm_i_call(our_little_machine); //CALL
    }
    else if (instruction == 911)
    {
        lmsm_i_return(our_little_machine); //RET
    }
    else if (instruction == 920)
    {
        lmsm_i_push(our_little_machine); //SPUSH
    }
    else if (instruction == 921)
    {
        lmsm_i_pop(our_little_machine); //SPOP
    }
    else if (instruction == 922)
    {
        lmsm_i_dup(our_little_machine); //SDUP
    }
    else if (instruction == 923)
    {
        lmsm_i_sadd(our_little_machine); //SADD
    }
    else if (instruction == 924)
    {
        lmsm_i_ssub(our_little_machine); //SSUB
    }
    else if (instruction == 925)
    {
        lmsm_i_smax(our_little_machine); //SMAX
    }
    else if (instruction == 926)
    {
        lmsm_i_smin(our_little_machine); //SMIN
    }
    else if (instruction == 927)
    {
        lmsm_i_smul(our_little_machine); //SMUL
    }
    else if (instruction == 928)
    {
        lmsm_i_sdiv(our_little_machine); //SDIV
    }
    else
    {
        our_little_machine->error_code = ERROR_UNKNOWN_INSTRUCTION;
        our_little_machine->status = STATUS_HALTED;
    }

    lmsm_cap_accumulator_value(our_little_machine);
}

void lmsm_load(lmsm *our_little_machine, int *program, int length)
{
    for (int i = 0; i < length; ++i)
    {
        our_little_machine->memory[i] = program[i];
    }
}

void lmsm_delete_stack(lmsm_stack *current)
{
    if (current->next)
    {
        lmsm_delete_stack(current->next);
    }

    free(current);
}

void lmsm_init(lmsm *the_machine)
{
    the_machine->accumulator->value = 0;
    the_machine->accumulator->next = NULL;
    the_machine->status = STATUS_READY;
    the_machine->error_code = ERROR_NONE;
    the_machine->program_counter = 0;
    the_machine->current_instruction = 0;
    the_machine->call_stack->value = 0;
    the_machine->call_stack->next = NULL;

    memset(the_machine->output_buffer, 0, sizeof(char) * 1000);
    memset(the_machine->memory, 0, sizeof(int) * 100);
}

void lmsm_reset(lmsm *our_little_machine)
{
    lmsm_delete_stack(our_little_machine->accumulator);
    our_little_machine->accumulator = malloc(sizeof(lmsm_stack));

    lmsm_delete_stack(our_little_machine->call_stack);
    our_little_machine->call_stack = malloc(sizeof(lmsm_stack));

    lmsm_init(our_little_machine);
}

void lmsm_run(lmsm *our_little_machine)
{
    our_little_machine->status = STATUS_RUNNING;

    while (our_little_machine->status != STATUS_HALTED)
    {
        lmsm_step(our_little_machine);
    }
}

lmsm *lmsm_create()
{
    lmsm *the_machine = malloc(sizeof(lmsm));
    the_machine->accumulator = malloc(sizeof(lmsm_stack));
    the_machine->call_stack = malloc(sizeof(lmsm_stack));

    lmsm_init(the_machine);
    return the_machine;
}

void lmsm_delete(lmsm *the_machine)
{
    lmsm_delete_stack(the_machine->accumulator);
    lmsm_delete_stack(the_machine->call_stack);
    free(the_machine);
}