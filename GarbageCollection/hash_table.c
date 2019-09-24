/*
hash_table.c - library used to reference count for variable;
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {false, true} bool;

typedef struct ref_count_item{
  bool removed; // whether the slot is removed
  unsigned long location;
  unsigned int ref_count;
  unsigned long* dep_items;
  } HashItem;

typedef struct ref_count_hash_table{
  unsigned int size;
  unsigned int var_num;
  HashItem* item_ls;
  } RefCountHashTable;
  
unsigned int hash_fun(unsigned long addr, unsigned int table_size){
  return (addr/sizeof(unsigned int)) % table_size;
};

void hashtable_insert_ls(HashItem* item_ls, unsigned long addr, unsigned ref_count, unsigned long* dep_item_addr_ls, unsigned int ls_size);
  
void hashtable_resize(RefCountHashTable* hashtable, unsigned int size){
  HashItem* new_item_ls = malloc(size * sizeof(HashItem));
  memset(new_item_ls, 0, size * sizeof(HashItem));

  for(int i=0;i<(hashtable->size);i++){
    
    hashtable_insert_ls(new_item_ls, (hashtable->item_ls)[i].location,
      (hashtable->item_ls)[i].ref_count,
      (hashtable->item_ls)[i].dep_items, size);
    
  }
  
  free(hashtable->item_ls);
  hashtable->item_ls = new_item_ls;
  
  hashtable->size = size;

};

void set_item_ref_count(HashItem* item, unsigned int new_ref_count){
  item->ref_count = new_ref_count;
}

HashItem* find_ref_count_item(unsigned long addr, RefCountHashTable* hashtable){
  unsigned int key = hash_fun(addr, hashtable->size);

  HashItem *result_item;
  result_item = 0; // set it to null initially

  unsigned int hashtable_size = hashtable->size;
  for(int i = 0; i<hashtable_size; i++){
    int new_key = (key+i) % hashtable_size;

    HashItem item_to_be_checked = (hashtable->item_ls)[new_key];

    if (item_to_be_checked.location == addr && item_to_be_checked.removed == false){
      
      result_item = &(hashtable->item_ls[new_key]);
      return result_item;
    }
  }

  printf("The item (addr: %lu) is not in the list.\n", addr);
  return result_item;
}

void hashtable_insert_with_dep(RefCountHashTable* hashtable, unsigned long addr, unsigned ref_count, unsigned long* dep_item_addr_ls);

void hash_table_insert(RefCountHashTable* hashtable, unsigned long addr, unsigned ref_count){
  unsigned long* null_dep_item_ls = malloc(sizeof(unsigned long));
  memset(null_dep_item_ls,0, sizeof(unsigned long));

  hashtable_insert_with_dep(hashtable, addr, ref_count, null_dep_item_ls);
  
}

void hash_table_delete(unsigned long addr, RefCountHashTable* hashtable){
  HashItem* to_be_deleted;
  to_be_deleted = find_ref_count_item(addr, hashtable);

  if (to_be_deleted != 0){
    to_be_deleted->removed = true; // the item is removed
    to_be_deleted->dep_items = 0;
    to_be_deleted->ref_count = 0;
    to_be_deleted->location = 0;

    hashtable->size -= 1;
  }
  else{
    printf("the item (addr: %lu is not found, so it can't be deleted.\n", addr);
  }
  
}


// insert item with dependent item list
void hashtable_insert_with_dep(RefCountHashTable* hashtable, unsigned long addr, unsigned ref_count, unsigned long* dep_item_addr_ls){
  if (hashtable->var_num + 1 == hashtable->size){
    hashtable_resize(hashtable,(hashtable->size)*2);
  }

    hashtable_insert_ls(hashtable->item_ls, addr, ref_count, dep_item_addr_ls, hashtable->size);
    
    hashtable->var_num += 1;
  }

void hashtable_insert_ls(HashItem* item_ls, unsigned long addr, unsigned ref_count, unsigned long* dep_item_addr_ls, unsigned int ls_size){
  int key = hash_fun(addr, ls_size);
  
  while (item_ls[key].location != 0){
      key = (key + 1) % (ls_size);
    }
    
    item_ls[key].location = addr;
    item_ls[key].removed = false; // reset to be unmoved
    item_ls[key].ref_count = ref_count;
    item_ls[key].dep_items = dep_item_addr_ls;
  
  }

unsigned int refcount_hashtable_size = 3;
RefCountHashTable refcount_hashtable;


// initialize the hash table
void initialize_hash_table() {
  
  
  refcount_hashtable.size = refcount_hashtable_size;
  refcount_hashtable.var_num = 0;
  HashItem* hashtable_items = malloc(refcount_hashtable.size * sizeof(HashItem)); 
  memset(hashtable_items, 0, refcount_hashtable.size * sizeof(HashItem));
  refcount_hashtable.item_ls = hashtable_items;
}