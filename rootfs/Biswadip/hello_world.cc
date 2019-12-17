#include <stdio.h>
#include <inttypes.h>

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define rdtime() read_csr(time)

// #define swap_csr(reg, val) ({ unsigned long __tmp; \
//   asm volatile ("csrrw %0, " #reg ", %1" : "=r"(__tmp) : "rK"(val)); \
//   __tmp; })

// #define set_csr(reg, bit) ({ unsigned long __tmp; \
//   asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
//   __tmp; })

// #define clear_csr(reg, bit) ({ unsigned long __tmp; \
//   asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
//   __tmp; })

// #define rdtime() read_csr(time)
// #define rdcycle() read_csr(cycle)
// #define rdinstret() read_csr(instret)

int main(int argc, char ** argv) {

  // for (unsigned long k = 0; k < 32; k++) {

    // printf("Hello world, I am HART %d! Counting (%d of 32)...\n", argv[0][0], k);
  
  //   write_csr(0x801, k);
  //   printf("CSR LCT: %" PRIu64 "\n", read_csr(0x801));

  //   write_csr(0x802, k+100);
  //   printf("CSR LCT: %" PRIu64 "\n", read_csr(0x802));

  //   write_csr(0x803, k+200);
  //   printf("CSR LCT: %" PRIu64 "\n", read_csr(0x803));
  // }

  int arr[50] = {};

  for (int k = 0; k < 50; k++) {
    arr[k]=k;
  }

  write_csr(0x801, &arr[2]);
  write_csr(0x803, 0xa);
  // printf("Faulty address = %p\n", (void *) read_csr(0x801) );
  for (int k = 0; k < 50; k++)
  {
    printf("%d ", arr[k]);
  }

  write_csr(0x801, 0);
  write_csr(0x803, 0);
  for (int k = 0; k < 50; k++)
  {
    printf("%d ", arr[k]);
  }

  printf("\n");

  // write_csr(DATA_A_REG, Starting_address);
  // write_csr(DATA_B_REG, Ending_address);
  // write_csr(CTRL_REG, Load_address);

  // write_csr(DATA_A_REG, BER_L1D);
  // write_csr(CTRL_REG, Load_BER_L1);
  
    
  printf("Done!\n");

  return 0;
}
