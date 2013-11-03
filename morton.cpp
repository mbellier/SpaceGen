#include "morton.hpp"

#ifdef MORTON_TEST
#include "time.h"
#include "stdio.h"
#endif


namespace Morton
{

unsigned int convert_to_unsigned(int x)
{
  if (x > 0)
    return ((unsigned int)x) << 1;
  else
    return ((unsigned int)(-x) << 1) + 1;
}


int convert_to_signed(unsigned int x)
{
  if (x & 1)
    return -(int)(x >> 1);
  else
    return (int)(x >> 1);
}

/**
 * @brief dilate_2 Computes dilation on 16 bits with a 1-bit gap.
 * @param x
 * @return
 */
unsigned int dilate_2(unsigned int x)
{
  x &= 0x0000ffff;                  // x = ---- ---- ---- ---- fedc ba98 7654 3210
  x = (x ^ (x <<  8)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
  x = (x ^ (x <<  4)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
  x = (x ^ (x <<  2)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
  x = (x ^ (x <<  1)) & 0x55555555; // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
  return x;
}


/**
 * @brief dilate_3 Computes dilation on 10 bits with a 2-bit gap.
 * @param x_
 * @return
 */
unsigned int dilate_3(unsigned int x)
{
  x &= 0x000003ff;                  // x = ---- ---- ---- ---- ---- --98 7654 3210
  x = (x ^ (x << 16)) & 0xff0000ff; // x = ---- --98 ---- ---- ---- ---- 7654 3210
  x = (x ^ (x <<  8)) & 0x0300f00f; // x = ---- --98 ---- ---- 7654 ---- ---- 3210
  x = (x ^ (x <<  4)) & 0x030c30c3; // x = ---- --98 ---- 76-- --54 ---- 32-- --10
  x = (x ^ (x <<  2)) & 0x09249249; // x = ---- 9--8 --7- -6-- 5--4 --3- -2-- 1--0
  return x;
}

/**
 * @brief contract_2 Contracts 32 bits with a 1-bit gap.
 * @param x
 * @return
 */
unsigned int contract_2(unsigned int x)
{
  x &= 0x55555555;                  // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
  x = (x ^ (x >>  1)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
  x = (x ^ (x >>  2)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
  x = (x ^ (x >>  4)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
  x = (x ^ (x >>  8)) & 0x0000ffff; // x = ---- ---- ---- ---- fedc ba98 7654 3210
  return x;
}

/**
 * @brief contract_3 Contracts 30 bits with a 2-bit gap.
 * @param x
 * @return
 */
unsigned int contract_3(unsigned int x)
{
  x &= 0x09249249;                  // x = ---- 9--8 --7- -6-- 5--4 --3- -2-- 1--0
  x = (x ^ (x >>  2)) & 0x030c30c3; // x = ---- --98 ---- 76-- --54 ---- 32-- --10
  x = (x ^ (x >>  4)) & 0x0300f00f; // x = ---- --98 ---- ---- 7654 ---- ---- 3210
  x = (x ^ (x >>  8)) & 0xff0000ff; // x = ---- --98 ---- ---- ---- ---- 7654 3210
  x = (x ^ (x >> 16)) & 0x000003ff; // x = ---- ---- ---- ---- ---- --98 7654 3210
  return x;
}

unsigned int encode_2(unsigned int x, unsigned int y)
{
  return dilate_2(x) | (dilate_2(y) << 1);
}

unsigned int encode_3(unsigned int x, unsigned int y, unsigned int z)
{
   return dilate_3(x) | (dilate_3(y) << 1) | (dilate_3(z) << 2);
}

unsigned int decode_2_x(unsigned int code)
{
  return contract_2(code);
}

unsigned int decode_2_y(unsigned int code)
{
  return contract_2(code >> 1);
}

unsigned int decode_3_x(unsigned int code)
{
  return contract_3(code);
}

unsigned int decode_3_y(unsigned int code)
{
  return contract_3(code >> 1);
}

unsigned int decode_3_z(unsigned int code)
{
  return contract_3(code >> 2);
}

unsigned int signed_encode_2(int x, int y)
{
  return encode_2(convert_to_unsigned(x), convert_to_unsigned(y));
}

unsigned int signed_encode_3(int x, int y, int z)
{
  return encode_3(convert_to_unsigned(x),
                  convert_to_unsigned(y),
                  convert_to_unsigned(z));
}

int signed_decode_2_x(unsigned int code)
{
  return convert_to_signed(decode_2_x(code));
}

int signed_decode_2_y(unsigned int code)
{
  return convert_to_signed(decode_2_y(code));
}

int signed_decode_3_x(unsigned int code)
{
  return convert_to_signed(decode_3_x(code));
}

int signed_decode_3_y(unsigned int code)
{
  return convert_to_signed(decode_3_y(code));
}

int signed_decode_3_z(unsigned int code)
{
  return convert_to_signed(decode_3_z(code));
}


#ifdef MORTON_TEST

int test_2(int a, int b, int enable_gui)
{
  int x, y;
  if (a < 0 || b < 0)
  {
    unsigned long code = Morton::signed_encode_2(a, b);
    x = Morton::signed_decode_2_x(code);
    y = Morton::signed_decode_2_y(code);
  }
  else
  {
    unsigned long code = Morton::encode_2(a, b);
    x = Morton::decode_2_x(code);
    y = Morton::decode_2_y(code);
  }

  int test = (a==x && b==y);
  if (enable_gui)
  {
    printf("[%s] (%d %d)", test ? "OK" : "KO", a, b);
    if (!test) printf (" != (%d %d)",x,y);
    printf("\n");
  }
  return test ? 1 : 0;
}

int range_test_2(int min, int max, int enable_gui){
  int nb_total = (max - min);

  nb_total = nb_total * nb_total;

  using namespace std;
  clock_t begin = clock();

  int nb_success = 0;
  for (int x = min; x < max; x++){
    for (int y = min; y < max; y++){
      nb_success += test_2(x, y, MORTON_TEST__DISABLE_GUI);
    }
  }

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

  if (enable_gui){
    printf("\n Succeded: %d, Failed: %d, Total: %d\n Success: %.1f %%\n",
           nb_success,
           nb_total - nb_success,
           nb_total,
           nb_success / (float)nb_total * 100);
    printf(" Elapsed Time: %.3f s (%.6f ms average)\n",
           elapsed_secs,
           elapsed_secs / (double)nb_total * 1000);
    printf("\n");
  }

  return nb_success == nb_total;
}

int test_3(int a, int b, int c, int enable_gui)
{
  int x, y, z;
  if (a < 0 || b < 0 || c < 0)
  {
    unsigned long code = Morton::signed_encode_3(a, b, c);
    x = Morton::signed_decode_3_x(code);
    y = Morton::signed_decode_3_y(code);
    z = Morton::signed_decode_3_z(code);
  }
  else
  {
    unsigned long code = Morton::encode_3(a, b, c);
    x = Morton::decode_3_x(code);
    y = Morton::decode_3_y(code);
    z = Morton::decode_3_z(code);
  }

  int test = (a==x && b==y && c==z);
  if (enable_gui)
  {
    printf("[%s] (%d %d %d)", test ? "OK" : "KO", a, b, c);
    if (!test) printf (" != (%d %d %d)",x,y,z);
    printf("\n");
  }
  return test ? 1 : 0;
}

int range_test_3(int min, int max, int enable_gui){
  int nb_total = (max - min);

  nb_total = nb_total * nb_total * nb_total;

  using namespace std;
  clock_t begin = clock();

  int nb_success = 0;
  for (int x = min; x < max; x++){
    for (int y = min; y < max; y++){
      for (int z = min; z < max; z++){
        nb_success += test_3(x, y, z, MORTON_TEST__DISABLE_GUI);
      }
    }
  }

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

  if (enable_gui){
    printf("\n Succeded: %d, Failed: %d, Total: %d\n Success: %.1f %%\n",
           nb_success,
           nb_total - nb_success,
           nb_total,
           nb_success / (float)nb_total * 100);
    printf(" Elapsed Time: %.3f s (%.6f ms average)\n",
           elapsed_secs,
           elapsed_secs / (double)nb_total * 1000);
    printf("\n");
  }

  return nb_success == nb_total;
}



#endif

} // namespace Morton
