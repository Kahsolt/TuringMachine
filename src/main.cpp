#include <iostream>
#include <string>
#include <ctime>
#include "config.h"
#include "Parser.h"
#include "TuringMachine.h"

using namespace std;

struct CmdOpts {
  string base_path;
  LogLevel level;
  bool save;
  CmdOpts() : base_path(""), level(OPT_DEFAULT_LOGLEVEL), save(OPT_DEFAULT_SAVE) { }
};

CmdOpts parse_cmd(int argc, char* argv[]) {
  bool ok = true;
  CmdOpts opts;
  for (int i=1; ok && i<argc; i++) {
    string opt = argv[i];
    if (opt[0] == '-') {
      if (opt == "-b") opts.level = BRIEF;
      else if (opt == "-s") opts.level = SIMPLE;
      else if (opt == "-v") opts.level = VERBOSE;
      else if (opt == "-w") opts.save = true;
      else if (opt == "-W") opts.save = false;
      else ok = false;
    } else {
      if (!opts.base_path.length()) opts.base_path = string(argv[i]) + PATH_SEPERATOR;
      else ok = false;
    }
  }
  if (!opts.base_path.length()) ok = false;

  if (!ok) {
    string progname = argv[0];
    int i = progname.rfind(PATH_SEPERATOR);
    if (i != -1) progname = progname.substr(i + 1, progname.length());
    cout << "Usage: " << progname << " <case-directory> [-b|-s|-v] [-w|-W]" << endl;
    cout << endl;
    cout << "[log level]:" << endl;
    cout << "  -b        brief: tape results only" << endl;
    cout << "  -s        simple: runtime steps and tape results (default)" << endl;
    cout << "  -v        verbose: TM definition, runtime steps and tape results" << endl;
    cout << "[log save]:" << endl;
    cout << "  -w        save logs to disk (default)" << endl;
    cout << "  -W        no save logs" << endl;
    exit(-1);
  } else return opts;
}

int main(int argc, char* argv[]) {
  CmdOpts opts = parse_cmd(argc, argv);

  ios::sync_with_stdio(false);  // fixup
  try {
    clock_t t = clock();  // compile usually takes just 1 clock 
    TuringMachine* tm = Parser().parse(opts.base_path + DEFINITION_FILENAME);
    tm->opts(opts.level);
    tm->run((            opts.base_path + INPUT_FILENAME),
            (opts.save ? opts.base_path + OUTPUT_FILENAME : NULL_DEVICE),
            (opts.save ? opts.base_path + DEBUG_FILENAME : NULL_DEVICE));
    if (opts.level == VERBOSE) cout << "[Timer] finish in " << clock() - t << " clocks" << endl;
    delete tm;
  } catch (TMException* ex) {
    cerr << ex->what() << endl;
    exit(-1);
  }
}