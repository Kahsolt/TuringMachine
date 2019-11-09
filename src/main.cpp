#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include "config.h"
#include "Exception.h"
#include "Parser.h"
#include "TuringMachine.h"

using namespace std;


int main(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <case-directory>" << endl;
    exit(0);
  }
  ios::sync_with_stdio(false);

  string BASE_PATH = argv[1];
  try {
    ifstream fdef(BASE_PATH + PATH_SEPERATOR + DEFINITION_FILENAME);
    TuringMachine* tm = Parser().parse(fdef);
    fdef.close();
#ifdef DEBUG_TM
    tm->show_definition();
#endif
    ifstream fin(BASE_PATH + PATH_SEPERATOR + INPUT_FILENAME);
    ofstream fout(BASE_PATH + PATH_SEPERATOR + OUTPUT_FILENAME);
    ofstream ferr(BASE_PATH + PATH_SEPERATOR + DEBUG_FILENAME);
    tm->run(fin, fout, ferr);
    fin.close();
    fout.close();
    ferr.close();
    delete tm;
  } catch (TMException* ex) {
    cerr << ex->what() << endl;
    exit(-1);
  }
}