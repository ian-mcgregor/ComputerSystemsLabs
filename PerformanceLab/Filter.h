//-*-c++-*-
#ifndef _Filter_h_
#define _Filter_h_

using namespace std;

class Filter {
  short int divisor;
  short int dim;
  int *data;

public:
  Filter(int _dim);
  int get(int r, int c);
  void set(int r, int c, int value);

  short int getDivisor();
  void setDivisor(int value);

  short int getSize();
  void info();
};

#endif
