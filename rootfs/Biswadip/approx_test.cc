// Approximation test

#include <stdio.h>

enum ber_type
{
  read,
  write
};

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ asm volatile("csrw " #reg ", %0" ::"rK"(val)); })

#define ARRAY_SIZE 1000
#define AVERAGE_SAMPLE 10

unsigned long getBerfromV(ber_type ty, float ber)
{
  unsigned long res;

  switch (ty)
  {
  case read:
    if (ber == 0.3f)
    {
      res = 2000;
    }
    else if (ber == 0.35f)
    {
      res = 16666;
    }
    else if (ber == 0.4f)
    {
      res = 200000;
    }
    else if (ber == 0.45f)
    {
      res = 5000000;
    }
    else if (ber == 0.5f)
    {
      res = 250000000;
    }
    else if (ber == 0.55f)
    {
      res = 25000000000;
    }
    break;

  case write:
    if (ber == 0.3f)
    {
      res = 91;
    }
    else if (ber == 0.35f)
    {
      res = 200;
    }
    else if (ber == 0.4f)
    {
      res = 1000;
    }
    else if (ber == 0.45f)
    {
      res = 6667;
    }
    else if (ber == 0.5f)
    {
      res = 66667;
    }
    else if (ber == 0.55f)
    {
      res = 909091;
    }
    break;

  default:
    break;
  }

  return res;
}

unsigned long setMaskBer(unsigned int ber)
{
  unsigned long max = 0xffffffffffffffff;
  // printf("Max:  %lu\n", max);
  // printf("BER:  %u\n", ber);
  // printf("Mask: %lu\n", max / ber);
  return max / ber;
}

unsigned int countSetBits(unsigned long n)
{
  unsigned int count = 0;
  while (n)
  {
    count += n & 1;
    n >>= 1;
  }
  return count;
}

// Returns the number of bit flips
int test_array()
{
  long unsigned arr[ARRAY_SIZE] = {0};
  int count_errors = 0;
  int errors = 0;
  long unsigned tmp;

  write_csr(0x801, &arr[0]);
  write_csr(0x802, &arr[ARRAY_SIZE - 1]);
  asm("nop");

  for (int i = 0; i < ARRAY_SIZE; i++)
    arr[i] = 1000 + i;
  //__atomic_store(&arr[i], (unsigned long *) &i, __ATOMIC_RELAXED);

  asm("nop");
  // asm("fence");
  // asm("nop");

  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    tmp = arr[i];
    errors = countSetBits((1000 + i) ^ tmp);
    // printf("%d\t%p : \t%d \t%016lx\n", i, &arr[i], errors, tmp);
    count_errors += errors;
  }

  write_csr(0x801, 0x0);
  write_csr(0x802, 0x0);

  // printf("Total bit flips : %d - %.5f%%\n", count_errors, ((float)count_errors * 100 / (64 * ARRAY_SIZE)));

  return count_errors;
}

float getVdd(int idx)
{
  if (idx==0)
    return 0.3f;
  
  if (idx==1)
    return 0.35f;

  if (idx==2)
    return 0.4f;

  if (idx==3)
    return 0.45f;

  if (idx==4)
    return 0.5f;

  if (idx==5)
    return 0.55f;
  
  return 0.0f;
}
int main(int argc, char **argv)
{
  write_csr(0x803, 0x0);
  write_csr(0x804, 0x0);
  write_csr(0x805, 0x0);
  write_csr(0x806, 0x0);

  for (int i = 0; i < 6; i++)
  {
    float vdd_l1 = getVdd(i);
    // printf("L1 : %0.2fV\n", vdd_l1);

    write_csr(0x803, setMaskBer(getBerfromV(read, vdd_l1)));
    write_csr(0x804, setMaskBer(getBerfromV(write, vdd_l1)));

    for (int j = 0; j < 6; j++)
    {
      float vdd_l2 = getVdd(j);
      printf("L1: %0.2fV, L2: %0.2fV\t", vdd_l1, vdd_l2);
      write_csr(0x805, setMaskBer(getBerfromV(read, vdd_l2)));
      write_csr(0x806, setMaskBer(getBerfromV(write, vdd_l2)));

      int flips = 0;
      for (int k = 0; k < AVERAGE_SAMPLE; k++)
      {
        flips += test_array();
      }
      flips/=AVERAGE_SAMPLE;

      printf("Avg flips : %d - %.5f%%\n", flips, ((float)flips * 100 / (64 * ARRAY_SIZE)));
    }
  }

  write_csr(0x803, 0x0);
  write_csr(0x804, 0x0);
  write_csr(0x805, 0x0);
  write_csr(0x806, 0x0);

  printf("Done!\n");

  return 0;
}
