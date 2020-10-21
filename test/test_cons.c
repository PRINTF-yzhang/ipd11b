#include "../src/cons.h"

/*11.b.1
 * Q: Can you sketch some ADT laws for malloc() and free()? What makes this difficult?
 *  Pointer as an Abstract Data Type
 *  Attributes and value domains
 *  Operators (malloc, free, calloc, realloc)
 *
 *  11.b.2
 *  Address Sanitizer (ASan)
 *  detects memory corruption bugs such as buffer overflows or
 *  accesses to a dangling pointer (use-after-free).
 *
 *
 *  11.b.3
 *  Q:  for an input size of n, what is read_line()’s running time in big-O notation?
 *  To read n bytes of input, it need to allocate 2^k bytes of memory
 *  starting at 1 byte and repeatedly doubling the capacity of the buffer,
 * 1. O(n)
 *
 *  start by allocating 10 bytes and then repeatedly grew the capacity of the buffer by adding 10 bytes
 * 2. O(1)


char* read_line(void)
{
    int c = getchar();
    if (c == EOF) return NULL;
    size_t cap = CAPACITY0 , fill = 0;
    char* buffer = surely_malloc(cap);
    for (;;) {
        if (c == EOF || c == '\n') {
            buffer[fill] = 0;
            return buffer;
        }
        buffer[fill++] = (char) c;
        c = getchar();
        if (fill + 1 > cap) {
            cap *= 2;
            buffer = surely_realloc(buffer , cap);
        }
    }
}
  11.b.4
  Aliasing is when you have multiple pointers to the same object

void f(void)
{
     short* p1 = new_short (1);

    short* p2 = new_short (2);

    short* p3 = p1;

    *p3 = 10;

    printf("%d\n", *p1);
// 10⏎
    / But free p3 will cause p1 to dangle
    free(p3);
    
    printf("%d\n", *p1);
}
if we avoid aliasing, we can safely free it, however uniqueness is too restrictive
 we can't have functions like rest(), where the pointer of the first node and is also for the second node

 Owning pointers & borrowing pointers
 • Owning pointers must be unique as owners
• Borrowing pointers are allowed to alias anything

 The Ownership Protocol
 The owner is responsible for deallocating it.
• Borrowers of an object may use it.
• Passing a pointer may or may not transfer ownership: read the contract

 */


// Uncomment the following `#define` line to see what’s happening in the
// program. If `ENABLE_TRACEF` is #defined above the #include <libipd.h>
// for your program then it defines `tracef` to be like `printf` for
// debugging; if `ENABLE_TRACEF` isn’t #defined then `tracef` does
// nothing.

#define ENABLE_TRACEF

#include <libipd.h>

#include <assert.h>
//#include <printf.h>
#include <stdint.h>

// Computes the length of a list. Recursive, which means it will “blow
// the stack” on a sufficiently long list.
size_t list_len_bsl_style(list_t lst)
{
    return is_empty(lst)? 0 : 1 + list_len_bsl_style(rest(lst));
}

// Computes the length of a list. Iterative, so uses only one stack
// frame regardless of list length.
size_t list_len(list_t lst)
{
    size_t count = 0;

    while (is_cons(lst)) {
        ++count;
        lst = rest(lst);
    }

    return count;
}

// Helper for recursive list printing function.
void print_list_bsl_style_helper(list_t lst)
{
    if (is_cons(lst)) {
        printf("cons(%d, ", first(lst));
        print_list_bsl_style_helper(rest(lst));
        printf(")");
    } else {
        printf("empty");
    }
}

// Prints a list. Recursive == small lists only.
void print_list_bsl_style(list_t lst)
{
    print_list_bsl_style_helper(lst);
    printf("\n");
}

// Prints a list. Iterative == space safe.
void print_list(list_t lst)
{
    size_t count = 0;

    while (is_cons(lst)) {
        printf("cons(%d, ", first(lst));
        lst = rest(lst);
        ++count;
    }

    printf("empty");

    while (count--) printf(")");

    printf("\n");
}

// Generates the list 0, 1, ..., (length - 1).
list_t iota(size_t length)
{
    list_t result = empty;
    while (length) result = cons(--length, result);
    return result;
}

// Tries length function `len_fun` on a list of length `n`.
void try_len(size_t (*lenf)(list_t), size_t n)
{
    tracef("Trying n = %zu: iota()...", n);
    list_t lst = iota(n);
    tracef(" len()...");
#ifdef ENABLE_TRACEF
    size_t len = lenf(lst);
#endif
    tracef(" got %zu! uncons_all()... ", len);
    uncons_all(lst);
    tracef("done.\n");
}

// Little functions for passing to `map`:
static int add1(int z) { return z + 1; }
static int dbl(int z) { return z << 1; }

static void dblp(int* pz) { *pz <<= 1; }

int main(void)
{
    print_list(empty);

    list_t my_list = cons(2, cons(3, cons(4, cons(5, empty))));
    print_list(my_list);
    print_list(rest(my_list));

    list_t doubled = map(dbl, my_list);
    print_list(doubled);
    for_each(dblp, doubled);
    print_list(doubled);

    list_t incred = map(add1, rest(doubled));

    uncons_all(doubled);
    uncons_all(my_list);

    print_list(incred);
    uncons_all(incred);

    // Tries list_len_bsl_style on lists of exponetially increasing
    // length:
    for (size_t i = 1; i <= SIZE_MAX / 2; i *= 2)
        try_len(list_len_bsl_style, i);
}

