/* sca03.cpp
 *
 * Revision History
 * sca01.cpp, lower level discriminator nim
 * sca02.cpp, dead time skipping
 * sca03.cpp, option to skip transfer time, prints total counts
 * 
 * Usage
 * sca03 (level, decimal) (fractions of second, decimal) (deadt, int) (separator, C or N) (filename) (transfer time, char)
 * $ ./sca03 0.1 1.0 2 C U235_400ug_24s_2.5k_r02.lvm T
 *
 * Compile
 * $ g++ sca03.cpp -o sca03
 *
 * Author
 * rocampo 5/4/2015
 */
#include <stdlib.h>
#include <math.h>       // trunc
#include <string.h>     // strtok
#include <iostream>     // ifstream
#include <fstream>      // getline
using namespace std;

int main(int argc, char *argv[]) {
  // called correctly?
  if (argc != 7) {
    cout << "usage: ./sca03 0.1 1.0 2 C U235_400ug_24s_2.5k_r02.lvm T" << endl;
    return 1;
  }
    
  const int size = 40;
  char separator[3] = ", ";
  if (!strcmp(argv[4], "N"))
    strcpy(separator, "\n");
  ifstream infile(argv[5]);  // filename
  char data[size], *temp;
  double level = atof(argv[1]);  // level of discriminator
  double fractions = atof(argv[2]);  // bin size in decimal seconds
  int dtime = atoi(argv[3]);  // while in dead time
  int s = dtime;
  int count = 0;
  int total = 0;
  int line = 1;
    
  // skip over Labview header info
  for (int i = 0; i < 21; i++) {
    infile.getline(data, size);
  }
    
  // getting samples per second from Delta_X
  temp = strtok(data, "\t");
  int samples = (double)(fractions / atof(strtok(NULL, "\t"))) + 1;
    
  cout << "No. samples in bin " << samples << endl;

  // skip over end of header and column heading
  infile.getline(data, size);
  infile.getline(data, size);

  // skip over transfer time if T flag set
  if (strcmp(argv[6], "T") == 0) {
    cout << "Skipping " << trunc(0.6 * samples) << endl;
    for (int i = 0; i < trunc(0.6 * samples); i++) {
      infile.getline(data, size);
    }
  }

  while (infile) {
    infile.getline(data, size);
    if ((atof(data) > level) && (s == dtime)) {
      count++;
      total++;
      s--;
    }
    if (line % samples == 0) {
      cout << count << separator;
      count = 0;
    }
    line++;
    s--;
    if (s < 1) s = dtime;
  } 
  cout << endl;
  cout << "Counted " << total << endl;
  infile.close();
  return 0;
}
