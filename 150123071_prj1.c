#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAKSBIRIM 100
#define MAX_VALUE 20 
#define OPERATORS "+-*/^"



typedef struct Node {
    char token[100];
    int type;        /* 0: Number, 1: Operator*/
    struct Node *left;
    struct Node *right;

} Node;

typedef struct {
    char birimler[MAKSBIRIM][100];
    int count;

} yigin;




typedef struct {
    Node* items[MAKSBIRIM];
    int top;
    

} NodeStack;

typedef struct {
    char items[MAKSBIRIM][100];
    int top;
} Stack;

typedef struct {
    char a[10];
    char b[10];
    char c[10];
} Solution;

Solution solutions[100];
int solution_count = 0;

// yardımcı fonksiyonlar
int stringtoint(const char *str) {
    if(str == NULL) return 0; 

    int i = 0;
    int sign = 1; 
    int result = 0;

   
    if(str[0] == '-') {
        sign = -1;
        i++;
    }

   
    for(; str[i] != '\0'; i++) {
        if(str[i] < '0' || str[i] > '9') {
            
            break;
        }
        result = result * 10 + (str[i] - '0');
    }

    return sign * result;
}

Node* create_node(char* token, int type) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return 0;
    }
    strcpy(new_node->token, token);
    new_node->type = type;
    new_node->left =NULL;
     new_node->right = NULL;
    return new_node;
}

void free_tree(Node* root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void node_push(NodeStack *stack, Node* new_node) {
    if(stack->top < MAKSBIRIM - 1) {
        stack->top++;
        stack->items[stack->top] = new_node;
    }
}

Node* node_pop(NodeStack *s) {
    if(s->top >= 0) {
        return s->items[s->top--];
    }
    return NULL;
}

void push_str(Stack *stack, char* new_item) {
    if(stack->top < MAKSBIRIM - 1) {
        stack->top = stack->top + 1;
        strcpy(stack->items[stack->top], new_item);
    }
}

char* pop_str(Stack *s) {
    if(s->top >= 0) {
        return s->items[s->top--];
    }
    return NULL;
}

int is_number(char* token) { 
    int start;
    size_t i; 

    if(token == NULL ) {
        return 0;
    }
     start = (token[0] == '-');

    for(i = start; i < strlen(token); i++) {
        
        if(!isdigit(token[i])) return 0;
    }
    if (start && strlen(token) == 1) return 0; 
    return 1;
}

int is_operator(char* token) {
    
    
    return strlen(token) == 1 && strchr(OPERATORS, token[0]);
}

void bilinmeyenleribul(yigin *ygn, char bilinmeyen[3][2], int *bilinmeyensayisi) {
    int i;
    *bilinmeyensayisi = 0;
    for( i = 0; i < ygn->count; i++) {
        if(!is_number(ygn->birimler[i]) && !is_operator(ygn->birimler[i]) && strcmp(ygn->birimler[i], "=") != 0 ) {//strcmp yerine if ile karşılaştırsaydım uzardı
            
            int found = 0;
            int j;
            for( j = 0; j < *bilinmeyensayisi; j++) {
                if(strcmp(bilinmeyen[j], ygn->birimler[i]) == 0) {
                    found = 1;
                    break;
                }
            }
            if(!found) {
                strcpy(bilinmeyen[*bilinmeyensayisi], ygn->birimler[i]);
                (*bilinmeyensayisi)++;
                if(*bilinmeyensayisi >= 3) break;
            }
        }
    }
}

/* option fonksiyonları*/

void option1(yigin *ygn){

    int sayi= 0;
    int i;
    for( i = 0; i< ygn->count ; i++){
        if(!is_number(ygn->birimler[i]) && !is_operator(ygn->birimler[i]) && strcmp(ygn->birimler[i],"=") != 0){
            if (sayi== 0) {
                 strcpy(ygn->birimler[i],"+");
                  sayi++;
                 }
            else if(sayi ==1) {
                 strcpy(ygn->birimler[i],"+"); 
                 sayi++;
             }
            else if(sayi ==2) {
             strcpy(ygn->birimler[i],"+"); 
             sayi++;
             }
        }
    }
}
void option2(yigin *ygn){
    int sayi= 0;
    int i;
    for( i = 0 ; i< ygn ->count; i++){
        if(!is_number(ygn->birimler[i]) && !is_operator(ygn->birimler[i])&& strcmp(ygn->birimler[i], "=") != 0){
            if(sayi==0) { 
            strcpy(ygn->birimler[i], "+"); 
            sayi++;
         }
            else if( sayi==1) { 
                strcpy(ygn->birimler[i],"+"); 
                sayi++; 
            }
            else if(sayi==2) { 
                strcpy(ygn->birimler[i],"1"); 
                sayi++;
             }
        }
    }
}
void option3(yigin * ygn){

    int sayi = 0;
    int i;
    for(i = 0 ; i< ygn->count; i++){
        if(!is_number(ygn->birimler[i]) && !is_operator(ygn->birimler[i])&& strcmp(ygn->birimler[i], "=") != 0){
            if(sayi == 0) { 
                strcpy(ygn->birimler[i], "+");                
                sayi++; 
            }
            else if(sayi == 1) {
                 strcpy(ygn->birimler[i], "1");
                sayi++; 
            }
            else if(sayi == 2) { 
                strcpy(ygn->birimler[i], "+"); 
                sayi++;
    
            }
        }
    }
}
void option4(yigin *ygn){

    int sayi = 0;
    int i;
    for( i = 0 ; i< ygn->count; i++){
        if(!is_number(ygn->birimler[i]) && !is_operator(ygn->birimler[i])&& strcmp(ygn->birimler[i], "=") != 0){
            if(sayi == 0) { 
            
                strcpy(ygn->birimler[i], "+"); 
                sayi++; 
            }
            else if(sayi == 1) {
                 strcpy(ygn->birimler[i], "1"); 
                sayi++; 
            }
            else if(sayi == 2) { 
                strcpy(ygn->birimler[i], "1"); 
                sayi++;
        
            }
        }
    }
}
void option5 (yigin * ygn){

    int sayi = 0;
    int i;
    for( i = 0 ; i< ygn->count; i++){
        if(!is_number(ygn->birimler[i]) && !is_operator(ygn->birimler[i])&& strcmp(ygn->birimler[i], "=") != 0){
            if(sayi == 0) { 
                strcpy(ygn->birimler[i],"1"); 
                sayi++; 
            }
            else if(sayi == 1) { 
                strcpy(ygn->birimler[i], "+"); 
                sayi++;
            }
            else if(sayi == 2) { 
                strcpy(ygn->birimler[i], "+"); 
                sayi++; 
            }
        }
    }
}
void option6 (yigin *ygn){
    int sayi = 0;
    int i;
    for(i = 0 ; i< ygn->count; i++){
        if(!is_number(ygn->birimler[i]) &&!is_operator(ygn->birimler[i])&& strcmp(ygn->birimler[i], "=") != 0){
            if(sayi == 0) {
                 strcpy(ygn->birimler[i], "1"); 
                 sayi++; 
            }
            else if(sayi == 1) {
                 strcpy(ygn->birimler[i], "+"); 
                 sayi++; 
            }
            else if(sayi == 2) { 
                strcpy(ygn->birimler[i], "1"); 
                sayi++; 
            }
        }
    }
}
void option7(yigin *ygn){

    int sayi = 0;
    int i;
    for( i = 0 ; i< ygn->count; i++){
        if(!is_number(ygn->birimler[i]) && !is_operator(ygn->birimler[i])&& strcmp(ygn->birimler[i], "=") != 0){
            if(sayi ==0) {
                 strcpy(ygn->birimler[i],"1"); 
                 sayi++; 
            }
            else if(sayi == 1) {
                 strcpy(ygn->birimler[i], "1"); 
                 sayi++; 
            }
            else if(sayi == 2) {
                 strcpy(ygn->birimler[i], "+"); 
                 sayi++; 
            }
        }
    }
}
void option8(yigin *ygn){
    int sayi = 0;
    int i;
    for( i = 0 ; i< ygn->count; i++){
        if(!is_number(ygn->birimler[i])&& !is_operator(ygn->birimler[i])&&strcmp(ygn->birimler[i], "=") != 0){
            if(sayi == 0) { 
                strcpy(ygn->birimler[i], "1"); 
                sayi++;
            }
            else if(sayi == 1) { 
                strcpy(ygn->birimler[i], "1"); 
                sayi++; 
            }
            else if(sayi ==2) {
                 strcpy(ygn->birimler[i], "1"); 
                sayi++; 
            }
        }
    }
}

int hesapla(int a,int b,char op, int *valid) { 
    *valid = 1;
    long long result = 0; 
    int i;
    
    switch(op){
        case '-': return a -b;
        case '+': return a + b;
        case '*': return a* b;
        case '/':
            if (b == 0 || a %b != 0){
                 *valid = 0;
                  return 0;
                }
            return a/ b;
        case '^':
            if(b <0) {
                 *valid =0; 
                 return 0; 
             }

            if (a == 0 && b == 0) {
                 *valid = 0;
                  return 0; 
            }
            
            if (b == 0) return 1;
            result = 1;
            for ( i = 0; i< b; i++) {
                result*= a;
               
            }
            return (int)result;
        default: *valid = 0; return 0;
    
    }
}

void input_ayristir(char* input, yigin *ygn) {
    char *birim ;
    ygn->count = 0;
    birim = strtok(input, " \n");
    
    while (birim != NULL && ygn->count < MAKSBIRIM){
       
        strcpy(ygn->birimler[ygn->count], birim);
        ygn->count++;
        birim = strtok(NULL, " \n");
    }
}

void yigini_kisalt(yigin *ygn){
    
    int i;
    Stack s;
    s.top = -1;
   
    for( i = 0; i <ygn->count; i++) {
        if(strcmp(ygn->birimler[i], "=") == 0) {
            int j;
            for(j = i; j < ygn->count; j++) push_str(&s, ygn->birimler[j]);
            break;
       
        }
        if(is_number(ygn->birimler[i]) ||(!is_operator(ygn->birimler[i])&& strcmp(ygn->birimler[i], "=") != 0 )) {
            push_str(&s, ygn->birimler[i]);
       
        } 
        else if (is_operator(ygn->birimler[i])){
            
            if(s.top >= 1&& is_number(s.items[s.top]) && is_number(s.items[s.top-1])){
                int b = stringtoint(pop_str(&s));
                int a =stringtoint(pop_str(&s));//todo
                int valid;
                int result = hesapla(a,b, ygn->birimler[i][0], &valid);
                if(valid){
                    char result2[100];
                    sprintf(result2, "%d", result);
                    push_str(&s, result2);
                } else {
                    char a_str[20], b_str[20];
                    sprintf(a_str, "%d", a);
                    sprintf(b_str, "%d", b);
                    push_str(&s, a_str);
                    push_str(&s, b_str);
                    push_str(&s, ygn->birimler[i]);
                }
            } else {
                push_str(&s, ygn->birimler[i]);
            }
        }
    }
    ygn->count = s.top + 1;
    for(i = 0; i <= s.top; i++){
     strcpy(ygn->birimler[i], s.items[i]);
    }
}

/*  EXPRESSION TREE && BACKTRACKING */

Node* expression_tree(yigin *ygn) {
    NodeStack s;
    s.top = -1;
    int i;
    for (i = 0; i <ygn->count; i++) {
        if (strcmp(ygn->birimler[i],"=") ==0){
            break;
        
        } 
      
        if (is_number(ygn->birimler[i])) {
            node_push(&s,create_node(ygn->birimler[i],0)); 
        } 
        
        else if (is_operator(ygn->birimler[i])) {
            if (s.top < 1) {
                 while(s.top >=0) {
                    free(node_pop(&s));
                 }

                 return NULL;
             }
            Node* op2 = node_pop(&s);
            Node* op1 =node_pop(&s);
            Node* new_op = create_node(ygn->birimler[i], 1);
            new_op->right = op2;
            new_op->left= op1;
            node_push(&s,new_op);
        } 
       
        else {
             while(s.top >= 0) {
                free(node_pop(&s));
             }
         return NULL; 
        }
    }
    if (s.top == 0){ 
        return node_pop(&s);
    }
    while(s.top>= 0){
        
        free(node_pop(&s));
    } 
    return NULL; 
}

int tree_olustur(Node* root,int *result) {
    if (root == NULL) {
    return 0;
   
    }
    if (root->type == 0) {
         *result = stringtoint(root->token);
          return 1;
    }
    if (root->type == 1) {
        int left_value, right_value;
        
        if (!tree_olustur(root->left, &left_value)) {
            return 0;
        }
        if (!tree_olustur(root->right, &right_value)){
             return 0;
        }
        int valid_op;
        *result = hesapla(left_value, right_value,root->token[0], &valid_op);
        if (!valid_op) {
            return 0;
        }
        return 1;
    }
    return 0;
}


void option_bul(yigin *base, char bilinmeyen[3][2],int bilinmeyensayisi, int option_num, int assignment_types[3]) {
    
    yigin temp = *base;
    int current_unknown = 0;
    int i ;
    
   
    switch(option_num) {

        case 1: option1(&temp); break;
        case 2: option2(&temp); break;
        case 3: option3(&temp); break;
        case 4: option4(&temp); break;
        case 5: option5(&temp); break;
        case 6: option6(&temp); break;
        case 7: option7(&temp); break;
        case 8: option8(&temp); break;
    }

   

    
    
    for(int i=0; i<base->count&& current_unknown < bilinmeyensayisi; i++) {
        
        int is_unk = 0;
        if(strcmp(base->birimler[i], bilinmeyen[current_unknown]) == 0) {
             is_unk = 1;
        }
        
        if(is_unk) {
           
            if(strcmp(temp.birimler[i], "+") == 0) {
                assignment_types[current_unknown]= 0; // Operator
            } else if(strcmp(temp.birimler[i], "1")== 0) {
                assignment_types[current_unknown] = 5; // Operand
            }
            current_unknown++;
        }
    }
}

void backtrack(yigin *ygn,char unknowns[3][2],int unknown_count,int index, char current_assignment_values[3][100], int assignment_types[3],int equal) {
    if (index == unknown_count) {
        yigin ygn_eval = *ygn;
        Node* root;
        int result;
        int i;
        for(i = 0; i < ygn_eval.count; i++) {
            int j;
            for( j = 0; j < unknown_count; j++) {
                
                if(strcmp(ygn_eval.birimler[i], unknowns[j])== 0) {
                    strcpy(ygn_eval.birimler[i], current_assignment_values[j]);
                    break;
                }
            }
        }
         root =expression_tree(&ygn_eval);
        if (!root) return;
        
        
        if (tree_olustur(root, &result) && result == equal) {
            if (solution_count < 100) {
              
                strcpy(solutions[solution_count].a, current_assignment_values[0]);
                strcpy(solutions[solution_count].b, current_assignment_values[1]);
                strcpy(solutions[solution_count].c, current_assignment_values[2]);
                solution_count++;
            }
        }
        free_tree(root);
        return;
    }

    if (assignment_types[index] == 5) { /* Operand*/
        int val;
        for ( val = 1; val <= MAX_VALUE; val++) {
           
            char val_str[10];
            sprintf(val_str, "%d", val);
            strcpy(current_assignment_values[index], val_str);
            backtrack(ygn, unknowns, unknown_count, index + 1, current_assignment_values, assignment_types,equal);
        }
    }
     else { /*Operator*/
        
        char ops[] = "+-*/^";
        int op_index;
        for (op_index = 0; op_index < 5; op_index++) {
            char op_str[2];
            op_str[0] = ops[op_index];
            op_str[1] = '\0';
            strcpy(current_assignment_values[index], op_str);
            backtrack(ygn, unknowns, unknown_count, index + 1, current_assignment_values, assignment_types,equal);
        }
    }
}

void print_expression(yigin *ygn) {
    int i;
    for(i = 0; i < ygn->count; i++) printf("%s ", ygn->birimler[i]);
    printf("\n");
}

int main(){
    char input[1000];
    yigin ygn;
    yigin ygn_kisaltilmis;
    char bilinmeyenler[3][2];
    int bilinmeyensayisi;
    int option;
    int i;
    int equal = 0;                                  /*I take input from user, these are the inputs.*/
    printf("Enter the postfix equation: ");         /*3 1 + 2 3 2 ^ a 2 / 4 3 - b - c * 2 4 1 + ^ 3 4 * 2 2 * - / 6 2 * - + = 0
                                                       18 15 a 8 b 5 c = 0           
                                                       20 10 a 8 b + + 3 c = 0
                                                       a b c = 2                                                              */
    fgets(input, sizeof(input), stdin);

    input_ayristir(input, &ygn);
    
   
     ygn_kisaltilmis = ygn;
    yigini_kisalt(&ygn_kisaltilmis);
    printf("Shortened expression: ");
    print_expression(&ygn_kisaltilmis);

    
    bilinmeyenleribul(&ygn_kisaltilmis, bilinmeyenler, &bilinmeyensayisi);
    
    if (bilinmeyensayisi != 3) {
        printf("Hata bilinmeyen sayisi 3 degil.\n");
        
        return 0;
    }

    
    for( i = 0; i< ygn_kisaltilmis.count;i++){
        if(strcmp(ygn_kisaltilmis.birimler[i], "=")== 0){
            if(i+1 < ygn_kisaltilmis.count&& is_number(ygn_kisaltilmis.birimler[i+1])){
                equal = stringtoint(ygn_kisaltilmis.birimler[i+1]);
            }
        }
    }
    
    
    
    
    for( option = 1; option <= 8; option++) {
        int assignment_types[3];
        char current_assignment_values[3][100];
        
        option_bul(&ygn_kisaltilmis,bilinmeyenler, bilinmeyensayisi, option, assignment_types);
        
        
        
      

        backtrack(&ygn_kisaltilmis, bilinmeyenler, bilinmeyensayisi,0,current_assignment_values, assignment_types,equal);
    }
    
    
    if (solution_count== 0) {
        printf("There is no solution.\n");
    } else {
        printf("Possible value combinations for (%s, %s, %s)\n", bilinmeyenler[0], bilinmeyenler[1], bilinmeyenler[2]);
        for(int i= 0; i< solution_count; i++) {
            
            printf("(%s, %s, %s)\n", solutions[i].a, solutions[i].b, solutions[i].c);
        }
    }

    return 0;
}