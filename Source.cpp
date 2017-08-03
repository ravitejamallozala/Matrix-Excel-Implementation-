#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node {
	char s[2];
	struct node *next;
};
struct cell{
	char expr[30];
	int value;
	struct node *depends=NULL;
	struct node *last=NULL;
};
struct cell matrix[3][3];
struct node* getNode(){
	struct node * newn = (struct node*)malloc(sizeof(struct node));
	newn->next = NULL;
	return newn;
}
int performoperation(int op1, int op2, char ope){
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

int evaluateexpr(char *expr,int r,int c){
	
	int r1 = expr[0] - 'a';
	int c1 = expr[1] - '0';
	char ope = expr[2];
	int r2 = expr[3] - 'a';
	int c2 = expr[4] - '0';
	struct node *newn = getNode();
	newn->s[0] = r +'a';
	newn->s[1] = c + '0';
	if (matrix[r1][c1].depends == NULL){
		matrix[r1][c1].depends = newn;
		matrix[r1][c1].last = newn;
	}
	else{
		matrix[r1][c1].last->next = newn;
		matrix[r1][c1].last = newn;
	}
	struct node *newn1 = getNode();
	newn1->s[0] = r + 'a';
	newn1->s[1] = c + '0';
	if (matrix[r2][c2].depends == NULL){
		matrix[r2][c2].depends = newn1;
		matrix[r2][c2].last = newn1;
	}
	else{
		matrix[r2][c2].last->next = newn1;
		matrix[r2][c2].last = newn1;
	}
	
	int res = performoperation(matrix[r1][c1].value, matrix[r2][c2].value, ope);
	return res;
}
int evaluateexpr1(char *expr){
	int r1 = expr[0] - 'a';
	int c1 = expr[1] - '0';
	char ope = expr[2];
	int r2 = expr[3] - 'a';
	int c2 = expr[4] - '0';
	return performoperation(matrix[r1][c1].value, matrix[r2][c2].value, ope);
}
void modifymatrix(int r, int c){
	struct node *temp;
	char expr[30];
	int r1, c1;
	temp = matrix[r][c].depends;
	while (temp != NULL){
		r1 = temp->s[0] - 'a';
		c1 = temp->s[1] - '0';
		strcpy(expr,matrix[r1][c1].expr);
		matrix[r1][c1].value = evaluateexpr1(expr + 3);
		modifymatrix(r1, c1);
		temp = temp->next;
	}
}
int stringtoint(char *expr){
	int i = 0, num = 0;
	while (expr[i] != '\0'){
		num = num * 10 + (expr[i] - '0');
		i++;
	}
	return num;
}
void printmatrix(int rows,int cols){
	int i, j;
	for (i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
			printf("%d ", matrix[i][j].value);
		}
		printf("\n");
	}
}
int main(){
	
	char expr[30];
	int r, c;
	int rows=3, cols = 3;
	//matrix = (struct cell **)malloc(sizeof(struct cell*) * 13);
	while (1){
		scanf("%s", expr);
		r = int(expr[0]) - int('a');
		c = expr[1] - '0';
		strcpy(matrix[r][c].expr, expr);
		if (expr[3] >= '0' && expr[3] <= '9'){
			int v = stringtoint(expr + 3);
			matrix[r][c].value = v;
		}
		else
			matrix[r][c].value = evaluateexpr(expr + 3,r,c);
		

		modifymatrix(r, c);
		printmatrix(rows,cols);
	}
}
