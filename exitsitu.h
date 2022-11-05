#ifndef __EXITSITU__
#define __EXITSITU__

/*this file ment to keep all the posible reasons to exit from the running progress*/

#define NULL_PTR_EXIT 1
#define NULL_PTR_EXEPTION "null pointer error\n"

#define BASE_EXIT 2
#define BASE_EXEPTION "false base calculation\n"

#define STACK_OVER_FLEW_EXIT 3
#define STACK_OVER_FLEW_EXEPTION "stack over flew!\n"

#define LABEL_MEM_EXIT 4
#define LABEL_MEM_EXEPTION "not enough memory for this lable!\n"

#define LABEL_VARIABLE_EXIT 5
#define LABEL_VARIABLE_EXEPTION "label's variable exeption!\n"

#define INVALID_LABEL_EXIT 6
#define INVALID_LABEL_EXEPTION "invild label!\n"

#define INVALID_INST_EXIT 7
#define INVALID_INST_EXEPTION "invild instruction!\n"

#define NUM_OF_COMMA_EXIT 8
#define NUM_OF_COMMA_EXEPTION "too much comma in line!\n"

#define INVALID_OPERAND_EXIT 9
#define INVALID_OPERAND_EXEPTION "there is an invalid operand !\n"

#define INVALID_OPERAND_TYPE_EXIT 10
#define INVALID_OPERAND_TYPE_EXEPTION "invalid operand type!\n"

#define INVALID_NODE_EXIT 11
#define INVALID_NODE_EXEPTION "invalid node!\n"

#define INVALID_MACRO_EXIT 12
#define INVALID_MACRO_EXEPTION "invalid macro!\n"

#define NUM_OF_ARGUMENTS_EXIT 13
#define NUM_OF_ARGUMENTS_EXEPTION "invalid num of arrguments!\n"

#define DUPLICATION_OF_ARGUMENTS_EXIT 14
#define DUPLICATION_OF_ARGUMENTS_EXEPTION "duplications of arguments!\n"

#define INVALID_NUM_OF_ARGUMENT_EXIT 15
#define INVALID_NUM_OF_ARGUMENT_EXEPTION "invalid num of arrguments!\n"

#define FIRST_TRANSITION_EXIT 16
#define FIRST_TRANSITION_EXEPTION "error in src file in the first transition!\n"

#define SECOND_TRANSITION_EXIT 17
#define SECOND_TRANSITION_EXEPTION "second transition didn't work!\n"

#define ALLOCATION_ERROR_EXIT 18
#define ALLOCATION_ERROR_EXEPTION "alocation error"

#define EXTERN_ERROR_EXIT 19
#define EXTERN_ERROR_EXEPTION "fail to create an extern operand"

#endif
