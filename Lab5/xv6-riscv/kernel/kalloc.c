// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
  // --- Juan: --- //
  int refcount[PHYSTOP / PGSIZE]; 
} kmem;



/*
 * Convert physical address to refcount index
 */
static inline int
pa2idx(uint64 pa)
{
  return pa / PGSIZE;
}

/*
 * increment refcount for page at physical address 'pa'
 */
void
kref_inc(uint64 pa)
{
  int idx = pa2idx(pa);
  acquire(&kmem.lock);
  // sanity bounds check
  if (idx >= 0 && idx < (PHYSTOP / PGSIZE))
    kmem.refcount[idx]++;
  release(&kmem.lock);
}

/*
 * decrement refcount for page at physical address 'pa';
 * if reaches 0, free the page
 */
void
kref_dec(uint64 pa)
{
  int idx = pa2idx(pa);
  int rc = 0;

  acquire(&kmem.lock);
  if (idx >= 0 && idx < (PHYSTOP / PGSIZE)){
    if (kmem.refcount[idx] > 0)
      kmem.refcount[idx]--;
    rc = kmem.refcount[idx];
  } else {
    // out of range: treat as immediate free
    rc = 0;
  }
  release(&kmem.lock);

  if (rc == 0) {
    // actually free the underlying page
    kfree((void*)pa);
  }
}

// --------

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
  for (int i = 0; i < (PHYSTOP / PGSIZE); i++)
    kmem.refcount[i] = 0;
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  // -- Juan: changed so wont free pages with refcount > 0-- // 
  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  int idx = pa2idx((uint64)pa);
  acquire(&kmem.lock);

  // only free if refcount == 0
  if(kmem.refcount[idx] != 0) {
    release(&kmem.lock);
    return; 
  }

  // Safe to add back to free list
  struct run *r;
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  //acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}




// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r) { // --- Juan: set refcount to 1 --- //
    memset((char*)r, 5, PGSIZE); // fill with junk

    int idx = pa2idx((uint64)r);
    kmem.refcount[idx]=1; 

  }

  return (void*)r;
}