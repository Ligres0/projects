#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX 100

/*  veri yapıları  */
typedef struct BHNode {
    int pid;
    int insertion_count;    //heap e kacıncı gelisi 
    double original_execution_time; //istatistik için ilk hali
    double execution_time;   //kalan sure       
    double arrival_time;    //sisteme gelis saati        
    double priority;        //hesaplanan öncelik         
           
    
    double total_waiting_time;
    double last_enqueue_time;   //kuyruga son giriş saati    

    int degree;
    struct BHNode *child;
    struct BHNode *parent;
    struct BHNode *sibling;
} BHNode;

typedef struct BinomialHeap {
    BHNode *head;
} BinomialHeap;

/* yardımcı fonksiyonlar */
void oncelikhesapla(BHNode *node, double e_max) {     
    if (node->insertion_count == 0) {
        node->priority = node->execution_time;
    } 
    else {
        double pay = 2 * node->execution_time;
        double payda = 3 * e_max;
        double taban = pay / payda;
        double us = -pow(taban, 3);
        double carpan = 1.0 / exp(us); 
        node->priority = carpan * node->execution_time;
    }
}

int oncelikkiyasla(BHNode *node1, BHNode *node2) {
    if (node1->priority < node2->priority) return 1;
    if (node1->priority > node2->priority) return 0;
    if (node1->arrival_time < node2->arrival_time) return 1;
    return 0; 
}

/* binomial heap*/
BHNode *nodeInit(BHNode data) {
    BHNode *node = (BHNode *)malloc(sizeof(BHNode));
    if (node == NULL) return NULL;

    node->pid = data.pid;
    node->original_execution_time = data.original_execution_time;
    node->execution_time = data.execution_time;
    node->arrival_time = data.arrival_time;
    node->priority = data.priority;
    node->insertion_count = data.insertion_count;
    node->total_waiting_time = data.total_waiting_time;
    node->last_enqueue_time = data.last_enqueue_time;

    node->degree = 0;
    node->child = NULL;
    node->parent = NULL;
    node->sibling = NULL;
    return node;
}

BinomialHeap *heapInit(void) {
    BinomialHeap *heap = (BinomialHeap *)malloc(sizeof(BinomialHeap));
    if (heap == NULL) return NULL;
    heap->head = NULL;
    return heap;
}

BHNode *heapMerge(BinomialHeap *heap1, BinomialHeap *heap2) {
    BHNode *head;
    BHNode *tail;
    BHNode *h1It;
    BHNode *h2It;

    if (heap1->head == NULL) return heap2->head;
    if (heap2->head == NULL) return heap1->head;

    h1It = heap1->head;
    h2It = heap2->head;

    if (h1It->degree <= h2It->degree) {
        head = h1It;
        h1It = h1It->sibling;
    } else {
        head = h2It;
        h2It = h2It->sibling;
    }

    tail = head;
    while (h1It != NULL && h2It != NULL) {
        if (h1It->degree <= h2It->degree) {
            tail->sibling = h1It;
            h1It = h1It->sibling;
        } else {
            tail->sibling = h2It;
            h2It = h2It->sibling;
        }
        tail = tail->sibling;
    }

    tail->sibling = (h1It != NULL) ? h1It : h2It;
    return head;
}

BHNode *heapUnion(BinomialHeap *original, BinomialHeap *uni) {
    BHNode *new_head;
    BHNode *prev;
    BHNode *aux;
    BHNode *next;

    new_head = heapMerge(original, uni);
    original->head = NULL;
    uni->head = NULL;

    if (new_head == NULL) return NULL;

    prev = NULL;
    aux = new_head;
    next = aux->sibling;

    while (next != NULL) {
        if (aux->degree != next->degree ||
            (next->sibling != NULL && next->sibling->degree == aux->degree)) {
            prev = aux;
            aux = next;
        } else {
            if (oncelikkiyasla(aux, next)) { 
                aux->sibling = next->sibling;
                next->parent = aux;
                next->sibling = aux->child;
                aux->child = next;
                aux->degree++;
            } else {
                if (prev == NULL) new_head = next;
                else prev->sibling = next;

                aux->parent = next;
                aux->sibling = next->child;
                next->child = aux;
                next->degree++;
                aux = next;
            }
        }
        next = aux->sibling;
    }
    return new_head;
}

void heapInsert(BinomialHeap *heap, BHNode data) {
    BinomialHeap *temp;
    BHNode *node;

    node = nodeInit(data);
    if (node == NULL) return;

    temp = heapInit();
    if (temp == NULL) return;

    temp->head = node;
    heap->head = heapUnion(heap, temp);
    free(temp);
}

void heapRemove(BinomialHeap *heap, BHNode *node, BHNode *before) {
    BinomialHeap *temp;
    BHNode *child;
    BHNode *new_head;
    BHNode *next;

    if (node == heap->head)
        heap->head = node->sibling;
    else if (before != NULL)
        before->sibling = node->sibling;

    new_head = NULL;
    child = node->child;

    while (child != NULL) {
        next = child->sibling;
        child->sibling = new_head;
        child->parent = NULL;
        new_head = child;
        child = next;
    }

    temp = heapInit();
    if (temp == NULL) return;

    temp->head = new_head;
    heap->head = heapUnion(heap, temp);
    free(temp);
}

BHNode *heapMin(BinomialHeap *heap) {
    BHNode *min;
    BHNode *min_prev;
    BHNode *curr;
    BHNode *prev;

    if (heap->head == NULL) return NULL;

    min = heap->head;
    min_prev = NULL;
    prev = min;
    curr = min->sibling;

    while (curr != NULL) {
        if (oncelikkiyasla(curr, min)) { 
            min = curr;
            min_prev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    heapRemove(heap, min, min_prev);
    if (min) {
        min->child = NULL;
        min->sibling = NULL;
        min->parent = NULL;
    }
    return min;
}

void heapFree(BinomialHeap *heap) {
    BHNode *min;
    while ((min = heapMin(heap)) != NULL) {
        free(min);
    }
    free(heap);
}

/* tablo */
void collect_nodes(BHNode *node, BHNode **list, int *count) {
    if (node == NULL) return;
    list[*count] = node;
    (*count)++;
    collect_nodes(node->child, list, count);
    collect_nodes(node->sibling, list, count);
}

void sort_nodes_by_pid(BHNode **list, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j]->pid > list[j+1]->pid) {
                BHNode *temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
            }
        }
    }
}

void print_trace_line(double time, BinomialHeap *heap, double e_max) {
   
    
    
    
    if (heap == NULL || heap->head == NULL) {
        
        return;
    }

    BHNode *node_list[MAX]; 
    int count = 0;
    collect_nodes(heap->head, node_list, &count);
    sort_nodes_by_pid(node_list, count);

    printf("%-5g ", time); 

    char buffer[200] = "";
    for (int i = 0; i < count; i++) {
        char temp[10];
        sprintf(temp, "P%d", node_list[i]->pid);
        strcat(buffer, temp);
        if (i < count - 1) strcat(buffer, ", ");
    }
    printf("%-20s ", buffer);

    for (int i = 0; i < count; i++) {
        BHNode *n = node_list[i];
        if (i > 0) printf(", "); 

        if (n->insertion_count == 0) {
            printf("P%d: %g", n->pid, n->priority);
        } 
        else {
            printf("P%d: (1/exp-(2*%.0f/(3*%.0f))^3)*%.0f = %.3f", 
                   n->pid, 
                   n->execution_time, 
                   e_max, 
                   n->execution_time, 
                   n->priority);
        }
    }
    printf("\n");
}
double simulasyon(int q, BHNode *orijinal_veri, int total_process_count, double e_max) {
    
    printf("\n==========================================\n");
    printf("Quantum (q) = %d\n", q);
    printf("==========================================\n");
    printf("Time  Processes in BH      Priority value of processes in BH\n");
    printf("------------------------------------------------------------\n");

    
    double final_waiting_times[MAX + 1] = {0}; 
    BHNode temp_list[MAX];
    int is_arrived[MAX] = {0}; 

    
    for (int i = 0; i < total_process_count; i++) {
        temp_list[i] = orijinal_veri[i];
      
        temp_list[i].child = NULL; 
        temp_list[i].parent = NULL;
        temp_list[i].sibling = NULL; 
        temp_list[i].degree = 0;
        temp_list[i].insertion_count = 0; 
        temp_list[i].total_waiting_time = 0;
    }

    BinomialHeap *BH = heapInit();
    BHNode *cpu_process = NULL;
    double current_time = 0;
    int completed_processes = 0;
    double process_run_time = 0; 
    double total_awt_sum = 0;    

    while (completed_processes < total_process_count) {
        
        
        for (int i = 0; i < total_process_count; i++) {
            if (!is_arrived[i] && temp_list[i].arrival_time <= current_time) {
                oncelikhesapla(&temp_list[i], e_max);
                
                temp_list[i].last_enqueue_time = temp_list[i].arrival_time; 
                heapInsert(BH, temp_list[i]);
                is_arrived[i] = 1; 
            }
        }

        print_trace_line(current_time, BH, e_max);

        
        if (cpu_process == NULL) {
            cpu_process = heapMin(BH);
            if (cpu_process != NULL) {
                
                cpu_process->total_waiting_time += (current_time - cpu_process->last_enqueue_time);
                process_run_time = 0; 
            }
        }

       
        if (cpu_process != NULL) {
            cpu_process->execution_time -= 1.0;
            process_run_time += 1.0;

            if (cpu_process->execution_time <= 0) {
                final_waiting_times[cpu_process->pid] = cpu_process->total_waiting_time;
                total_awt_sum += cpu_process->total_waiting_time;
                completed_processes++;
                free(cpu_process);
                cpu_process = NULL;
                current_time += 0.1; 
            }
            else if (process_run_time >= q) { 
                
                
                int heap_dolu = (BH->head != NULL);
                double bitis_zamani = current_time + 1.0; 
                int kapidabekleyenvar = 0;
                
                for(int k=0; k < total_process_count; k++) {
                    
                    if (!is_arrived[k] && temp_list[k].arrival_time <= (bitis_zamani + 0.0001)) {
                        kapidabekleyenvar = 1;
                        break;
                    }
                }

                int baskabekleyenvar = (heap_dolu || kapidabekleyenvar); 

                cpu_process->insertion_count++;
                oncelikhesapla(cpu_process, e_max);
                
                double gecikme = (baskabekleyenvar) ? 0.1 : 0.0;
                cpu_process->last_enqueue_time = bitis_zamani + gecikme;
                
                heapInsert(BH, *cpu_process);
                free(cpu_process);
                cpu_process = NULL;
                
                if (baskabekleyenvar) current_time += 0.1; 
            }
        }
        current_time += 1.0;
    }
    
    print_trace_line(current_time, BH, e_max);

    printf("\nPID\tWaiting time\n");
    printf("---\t------------\n");
    for(int p=1; p<=total_process_count; p++) {
        
        int pid = orijinal_veri[p-1].pid;
        printf("P%d\t%g\n", pid, final_waiting_times[pid]);
    }
    
    double awt = total_awt_sum / total_process_count;
    printf("\nAWT = %.3f\n", awt);

    heapFree(BH);
    return awt; 
}
/*main*/
int main(void) {
    
    
    BHNode input_processes[MAX]; 
    int total_process_count = 0; 

    
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("hata dosya bulunamadi!\n");
        
        return 1;
    }

   
    int p_id_counter = 1; 

    
    while (fscanf(file, "%lf,%lf", 
                  &input_processes[total_process_count].execution_time, 
                  &input_processes[total_process_count].arrival_time) != EOF) {
        
        
        
        input_processes[total_process_count].pid = p_id_counter++;
        
        input_processes[total_process_count].original_execution_time = input_processes[total_process_count].execution_time;
        input_processes[total_process_count].priority = 0; 
        input_processes[total_process_count].insertion_count = 0;
        input_processes[total_process_count].total_waiting_time = 0;
        input_processes[total_process_count].last_enqueue_time = 0;
        
        input_processes[total_process_count].degree = 0;
        input_processes[total_process_count].child = NULL;
        input_processes[total_process_count].parent = NULL;
        input_processes[total_process_count].sibling = NULL;

      
        total_process_count++;

        
        if (total_process_count >= MAX) {
            printf("Hata\n", MAX);
            break;
        }
    }
    fclose(file);

    
    if (total_process_count == 0) {
        printf("HATA.\n");
        return 1;
    }

    

    double e_max = 0;
    for (int i = 0; i < total_process_count; i++) {
        if (input_processes[i].execution_time > e_max)
            e_max = input_processes[i].execution_time;
    }

    int max_q_limit = (int)e_max + 2; 

    double min_awt = 99.0;
    int optimal_q = -1;

    for (int q = 1; q <= max_q_limit; q++) { 
        
        
       double sonuc_awt = simulasyon(q, input_processes, total_process_count, e_max);
        
        if (sonuc_awt < min_awt) {
            min_awt = sonuc_awt;
            optimal_q = q;
        }
    }

    printf("\n**************************************************\n");
    printf("OPTIMIZASYON SONUCU:\n");
    printf("En Dusuk AWT: %.3f\n", min_awt);
    printf("Ideal Quantum (q) Degeri: %d\n", optimal_q);
    

    return 0;
}