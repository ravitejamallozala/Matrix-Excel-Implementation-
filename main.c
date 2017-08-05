#include "FunctionHeaders.h"

void modify(int row_index, int col_index);
void parse(char *exp);
int expEval(char *exp, int row_index, int col_index);

typedef struct sll{
	int row_index;
	int col_index;
	struct sll *next;
}sll;

struct cell{
	int value;
	char exp[30];
    sll *head = NULL;
	sll *last = NULL;
}grid[5][5];

struct sll *createNode(int row_index,int col_index){
    sll *newnode = (sll*)malloc(sizeof(sll));
	newnode->row_index = row_index;
	newnode->col_index = col_index;
	newnode->next = NULL;
	return newnode;
}

void insert(int rhs_row, int rhs_col, int lhs_row, int lhs_col){

	sll *temp = grid[rhs_row][rhs_col].head;
	while (temp){
		if (temp->col_index == lhs_col && temp->row_index == lhs_row){
			return;
		}
		temp = temp->next;
	}
	
	sll *newnode = createNode(lhs_row, lhs_col);
	if (grid[rhs_row][rhs_col].head == NULL)
	{
		grid[rhs_row][rhs_col].head = newnode;
		grid[rhs_row][rhs_col].last = grid[rhs_row][rhs_col].head;
	}
	else{
		grid[rhs_row][rhs_col].last->next = newnode;
		grid[rhs_row][rhs_col].last = newnode;
	}
	
}

int evaluate(int op1, int op2, char ope){
	if (ope == '*')
		return op1*op2;
	if (ope == '+')
		return op1 + op2;
	if (ope == '/'){
		if (op2 == 0)
			return -1;
		return op1 / op2;
	}
	if (ope == '-')
		return op1 - op2;
	return 0;
}

void parse(char *exp){

	int col_index = exp[0] - 'a';
	int row_index, value;
	if (exp[2] == '='){//single digit
		row_index = exp[1] - '1';
		value = expEval(exp + 3, row_index, col_index);
	}
	else{//double digit
		row_index = (exp[1] - '1') * 10 + (exp[2] - '1');
		value = expEval(exp + 4, row_index, col_index);
	}
	grid[row_index][col_index].value = value;
	modify(row_index, col_index);
}

void modify(int row_index, int col_index){
	sll *temp = grid[row_index][col_index].head;
	char exp[30];

	while (temp){
		int r1 = temp->row_index;
		int c1 = temp->col_index;
		strcpy(exp, grid[r1][c1].exp);
		grid[r1][c1].value = expEval(exp,r1,c1);
		modify(r1, c1);
		temp = temp->next;
	}
}


int expEval(char *exp,int row_index,int col_index){

	if (exp[0] >= '0' && exp[0] <= '9')  // ex : a1 = 5 , a1=510---;
	{
		int index = 0,result=0;
		while (exp[index] != '\0' ){
			result = result * 10 + (exp[index] - '0');
			index++;
		}
		return result;
	}
	else{ // ex :  a1=a2+a3 , a1=a22+b33
		strcpy(grid[row_index][col_index].exp, exp);

		int cell_col_index = exp[0] - 'a';
		int cell_row_index,i=0;

		if (exp[2] >= '0' && exp[2] <= '9'){ // ex : a1
			cell_row_index = (exp[1] - '1')*10 + (exp[2]-'1');
			i = 2;
		}
		else{
			cell_row_index = exp[1] - '1'; 
			i = 1;
		}

		insert(cell_row_index, cell_col_index, row_index, col_index);

		int op1 = grid[cell_row_index][cell_col_index].value;
		char opr = exp[++i];
		cell_col_index = exp[++i] - 'a';
		 
		if (exp[i+2] >= '0' && exp[i+2] <= '9'){ // ex : a1
			cell_row_index = (exp[i+1] - '1') * 10 + (exp[i+2] - '1');
			i = i+2;
		}
		else{
			cell_row_index = exp[i+1] - '1';
			i = i+ 1;
		}

		int op2 = grid[cell_row_index][cell_col_index].value;
		insert(cell_row_index, cell_col_index, row_index, col_index);
		
		return evaluate(op1,op2,opr);
	}
}


void printmatrix(int rows, int cols){
	int i, j;
	for (i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
			printf("%d ", grid[i][j].value);
		}
		printf("\n");
	}
}

int main(){

	char exp[10];
	while (1)
	{
		printf("\nEnter expression : ");
		scanf("%s", exp);
		parse(exp);
		printmatrix(5, 5);
	}
	return 0;
}