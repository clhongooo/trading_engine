#include <boost/cstdint.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "SDateTime.h"
#include "ATU_Abstract_MDI.h"

using namespace std;
using namespace boost;

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " [binary file]" << endl << flush;
    return 0;
  }

  fpos_t pos;

  FILE *fp = fopen(argv[1], "rb");
  if (!fp) return 1;

  ATU_MDI_binary_marketfeed_struct bin_mdi_struct;
  while (ferror( fp ) != 0) {
    fread(&bin_mdi_struct, sizeof(ATU_MDI_binary_marketfeed_struct), 1, fp);
    cout << bin_mdi_struct.m_traded_price << endl;
  }

  return 0;
}
