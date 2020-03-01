// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "spinlock.h"
#include "rand.h"
#include "rand.c"

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

int frame[1000];
int counter = 0;

extern char end[]; // first address after kernel loaded from ELF file

// Initialize free list of physical pages.
void
kinit(void)
{
  char *p;

  initlock(&kmem.lock, "kmem");
  p = (char*)PGROUNDUP((uint)end);
  for(; p + PGSIZE <= (char*)PHYSTOP; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || (uint)v >= PHYSTOP) 
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;
  struct run *ret;

  acquire(&kmem.lock);
 
  //start from head of list 
  r = kmem.freelist;
  //counter number of elements in free list 
  int freeCounter = 0;

  //if head of list is not null 
  if(r){
    //count number of elements in free list 
    while(r != NULL){
      freeCounter++;
      r = r->next;
    }

    //get index of page to remove 
    unsigned int index = xv6_rand() % (freeCounter);

    if(index == 0){
      //reset head 
      r = kmem.freelist;
      frame[counter] = (int)(char*)r;
      counter++;
      ret = r;
      kmem.freelist = r->next; 
    }
    else{
      r = kmem.freelist;
      for(int i = 0; i < index -1; ++i){
        r = r->next;
      }

      ret = r->next;

      if(ret){
        r->next = r->next->next;
        frame[counter] = (int)(char*)ret;
        counter++;
      }
     
    }
  }
  release(&kmem.lock);
  return (char*)ret;
}

int dump_allocated(int *frames, int numframes) {
  
  if(numframes == 0 || numframes > counter){
    return -1;
  }
  
  int j = 0;
  
  for(int i = counter - 1; i >= (counter - numframes); i--){
    frames[j] = frame[i];
    j++;
  }

  return 0;
}
