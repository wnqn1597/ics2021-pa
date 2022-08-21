#include "sdb.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL, *headtail = NULL, *freetail = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  headtail = NULL;
  free_ = wp_pool;
  freetail = &wp_pool[NR_WP-1];
}

/* TODO: Implement the functionality of watchpoint */
WP* new_up(){
  if(free_ == NULL){
    printf("Insufficient watchpoint.\n");
    return NULL;
  }
  if(head == NULL || headtail == NULL){
    head = free_;
    free_->NO = 1;
  }else{
    headtail->next = free_;
    free_->NO = headtail->NO + 1;
  }
  headtail = free_;
  free_ = free_->next;
  if(free_ == NULL) freetail = NULL;
  return headtail;
}

void free_wp(WP *wp){
  if(head == wp){
    head = wp->next;
    if(head == NULL) headtail = NULL;
  }else{
    WP *now = head;
    while(now != NULL){
      if(now->next == wp)break;
      now = now->next;
    }
    if(now == NULL){
      printf("Not using watchpoint\n");
      return;
    }
    now->next = wp->next;
  }
  wp->next = NULL;
  if(free_ == NULL || freetail == NULL) free_ = wp;
  else freetail->next = wp;
  freetail = wp;
}

void watchpoint_display(){
  if(head == NULL){
    printf("No created watchpoint.\n");
    return;
  }
  printf("No\tType\tAddress\t\tExpression\n");
  WP *now = head;
  while(now != NULL){
    printf("%s\n", now->expression);
    bool success;
    word_t addr = expr(now->expression, &success);
    if(!success) printf("%d\t%d\tUNDEFINED\t\t%s\n", now->NO, now->type, now->expression);  
    else printf("%d\t%d\t%x\t\t%s\n", now->NO, now->type, addr, now->expression);
    now = now->next;
  }

}
