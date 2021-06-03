#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include "Filter.h"

using namespace std;

#include "rdtsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;

  for (short int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

    if ( ok ) {
      double sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
    delete input;
    delete output;
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

struct Filter *readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    
    short int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    
    short int div;
    input >> div;
    filter -> setDivisor(div);
    
    for (short int i=0; i < size; i++) {
      for (short int j=0; j < size; j++) {
        short int value;
        input >> value;
        filter -> set(i,j,value);
      }
    }
    return filter;
  } else {
      cerr << "Bad input in readFilter:" << filename << endl;
      exit(-1);
  }
}


double applyFilter(struct Filter *filter, cs1300bmp *input, cs1300bmp *output)
{
  //DESIRED RUNTIME == ~30 (FACTOR OF 50 IMPROVEMENT)
  long cycStart, cycStop;

  cycStart = rdtscll();

  output -> width = input -> width;
  output -> height = input -> height;

  short int plane = 0;
  output -> color[0][0][0] = 0;
  output -> color[0][0][1] = 0;
  output -> color[0][0][2] = 0;
  output -> color[0][1][0] = 0;
  output -> color[0][1][1] = 0;
  output -> color[0][1][2] = 0;
  output -> color[0][2][0] = 0;
  output -> color[0][2][1] = 0;
  output -> color[0][2][2] = 0;

  output -> color[1][0][0] = 0;
  output -> color[1][0][1] = 0;
  output -> color[1][0][2] = 0;
  output -> color[1][1][0] = 0;
  output -> color[1][1][1] = 0;
  output -> color[1][1][2] = 0;
  output -> color[1][2][0] = 0;
  output -> color[1][2][1] = 0;
  output -> color[1][2][2] = 0;

  output -> color[2][0][0] = 0;
  output -> color[2][0][1] = 0;
  output -> color[2][0][2] = 0;
  output -> color[2][1][0] = 0;
  output -> color[2][1][1] = 0;
  output -> color[2][1][2] = 0;
  output -> color[2][2][0] = 0;
  output -> color[2][2][1] = 0;
  output -> color[2][2][2] = 0;
  
  for(short int col = 1; col < (input -> width) - 1; col++) {
    
    for(short int row = 1; row < (input -> height) - 1 ; row++) {
      short int x = output -> color[plane][row][col];
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row - 1][col - 1] * filter -> get(0, 0) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row][col - 1] * filter -> get(1, 0) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row + 1][col - 1] * filter -> get(2, 0) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row - 1][col] * filter -> get(0, 1) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row][col] * filter -> get(1, 1) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row + 1][col] * filter -> get(2, 1) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row - 1][col + 1] * filter -> get(0, 2) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row][col + 1] * filter -> get(1, 2) );
      output -> color[plane][row][col] = output -> color[plane][row][col] + (input -> color[plane][row + 1][col + 1] * filter -> get(2, 2) );
       
      output -> color[plane][row][col] = output -> color[plane][row][col] / filter -> getDivisor();

      (x < 0) ? (x = 0) : x;

      (x  > 255) ? (x = 255) : x;
      
        
    }
  }

  cycStop = rdtscll();
  double diff = cycStop - cycStart;
  
  double diffPerPixel = diff / (output -> width * output -> height);
  
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n", diff, diff / (output -> width * output -> height));
  
  return diffPerPixel;
}
