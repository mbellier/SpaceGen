#ifndef MORTON_H
#define MORTON_H

#define MORTON_TEST

namespace Morton
{

const unsigned int MAX_BITS_2 = 16;
const unsigned int MAX_BITS_3 = 10;

unsigned int convert_to_unsigned (int x);
int          convert_to_signed   (unsigned int x);

unsigned int dilate_2_16bits     (unsigned int x);
unsigned int dilate_3_10bits     (unsigned int x);

unsigned int dilate_2            (unsigned int x);
unsigned int dilate_3            (unsigned int x);

unsigned int contract_2          (unsigned int x);
unsigned int contract_3          (unsigned int x);

unsigned int encode_2            (unsigned int x, unsigned int y);
unsigned int encode_3            (unsigned int x, unsigned int y, unsigned int z);

unsigned int decode_2_x          (unsigned int code);
unsigned int decode_2_y          (unsigned int code);

unsigned int decode_3_x          (unsigned int code);
unsigned int decode_3_y          (unsigned int code);
unsigned int decode_3_z          (unsigned int code);

unsigned int signed_encode_2     (int x, int y);
unsigned int signed_encode_3     (int x, int y, int z);

int          signed_decode_2_x   (unsigned int code);
int          signed_decode_2_y   (unsigned int code);

int          signed_decode_3_x   (unsigned int code);
int          signed_decode_3_y   (unsigned int code);
int          signed_decode_3_z   (unsigned int code);


#ifdef MORTON_TEST
#define MORTON_TEST__ENABLE_GUI 1
#define MORTON_TEST__DISABLE_GUI 0
int  test_2      (int a, int b, int enable_gui = MORTON_TEST__ENABLE_GUI);
int  test_3      (int a, int b, int c, int enable_gui = MORTON_TEST__ENABLE_GUI);
int  range_test_2(int min, int max, int enable_gui = MORTON_TEST__ENABLE_GUI);
int  range_test_3(int min, int max, int enable_gui = MORTON_TEST__ENABLE_GUI);
#endif

}


#endif // MORTON_H
