#include <fstream>
#include <iostream>
#include <string>
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
  if (!fp)
  {
    cerr << "Error opening " << argv[1] << endl << flush;
    return 0;
  }

  fseek(fp,0,SEEK_SET);
  ATU_MDI_marketfeed_struct mdi_strct;
  size_t iReadSize = 0;
  do {
    iReadSize = fread(&mdi_strct, sizeof(ATU_MDI_marketfeed_struct), 1, fp);
    if (iReadSize == 0) break;
    cout << ATU_MDI_marketfeed_struct::ToString(mdi_strct) << endl;
  }
  while (iReadSize > 0);

  return 0;
}
