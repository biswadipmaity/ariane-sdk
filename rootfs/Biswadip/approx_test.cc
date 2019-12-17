// Approximation test

#include <stdio.h>

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ asm volatile("csrw " #reg ", %0" ::"rK"(val)); })


void test_array()
{
  long unsigned arr[10] = {0};
  write_csr(0x803, 0xffffffffffffffff);
  write_csr(0x801, &arr[2]);
  write_csr(0x802, &arr[8]);
  asm("nop");

  for (int i=0;i<10;i++)
    arr[i] = i;

  asm("nop");

  for (int i=0;i<10;i++)
    printf("%p : %d: %lx\n", &arr[i], i, arr[i]);

  // tmp = x;
  write_csr(0x803, 0);
}

int main(int argc, char **argv)
{
    test_array();
//   long unsigned x = 0xabcd;
//   long unsigned arr[10] = {0};
//   long unsigned tmp=0;
  
//   // printf("Virtual  address = %p \n", &x);

//   printf("Sizeof(int) = %d \n", sizeof(int));

//   for (int i=0;i<10;i++)
//     arr[i] = i;

//   write_csr(0x803, 0xa);
//   write_csr(0x801, &arr[5]);
//   asm("nop");

//   for (int i=0;i<10;i++)
//     printf("%d: %lx\n", i, arr[i]);

//   // tmp = x;
//   write_csr(0x803, 0);

//   // printf("CSR A: %lx\n", read_csr(0x801));

//   // printf("Read value: %lx\n", tmp);

  printf("Done!\n");

  return 0;
}
