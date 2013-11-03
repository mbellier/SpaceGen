#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mainwindow.hpp"


#include "morton.hpp"


//void binary_print(int x, int nb_bits = 10)
//{
//    static char b[32];
//    b[0] = '\0';

//    int z;
//    for (z = 1 <<nb_bits; z > 0; z >>= 1)
//    {
//        strcat(b, ((x & z) == z) ? "1" : "0");
//    }

//    printf("%d (%s)\n", x, b);
//}



int main(int argc, char **argv){
//  Morton::range_test_2(-100, 200);
//  Morton::range_test_3(-10, 10);

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
