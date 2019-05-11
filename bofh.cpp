/* Outputs a random bofh excuse using elements from the list at
 * http://bofh.ntk.net/ExcuseBoard.html
 * Optionally, the user may edit lists 1 through 4 (bofh1.dat - bofh4.dat) to
 * customize them.
 *
 * This program is designed to be run from the *nix shell with no arguments.
 * However, the -dumb flag must be used to include the fourth word.
 * The code should be fully portable to Windows, DOS, or mac.
 *
 * The original howto is included in the file 'readme.txt'
 *
 * If this file is compiled with bofhstatic defined, the lists in the code will
 * be used instead of external files.
 * If dumb is defined, the program will output the fourth word by default.
 * The compiled preference can be overridden with the -dumb or -ndumb arguments.
 * If the program is compiled to use the datafiles, but they cannot be found or
 * there is an error opening one of them, the builtin list for that file will
 * be used.
 */

/*
 * BUGS:
 *
 * Currently reads the entire wordlist file into memory, which is not good for
 * very long lists.
 */

/* TO DO:
 * Make commandline flags/arguments more gnu-like
 * Redo argument parsing to be more dynamic
 * Organise return values
 */


#define bofhstatic	// default to builtin lists
//#define dumb

#include <cstdio>
#ifdef WIN32
#include <conio.h>
#endif
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <initializer_list>
#include <string>
#include <random>
using namespace std;

bool dump=false;	// default to not output data files

#ifdef bofhstatic	// default to builtin lists
bool bstat=true;	
#else				// defualt to external lists
bool bstat=false;
#endif				// disable builtin lists
bool nstat=false;


typedef vector<string> wordlist;

int bofhstat(string nothing, wordlist &st)
{
	if(nothing == "bofh1.dat") // list 1
	{
		st = {
			"Temporary",
			"Intermittant",
			"Partial",
			"Redundant",
			"Total",
			"Multiplexed",
			"Inherent",
			"Duplicated",
			"Dual-Homed",
			"Synchronous",
			"Bidirectional",
			"Serial",
			"Asynchronous",
			"Multiple",
			"Replicated",
			"Non-Replicated",
			"Unregistered",
			"Non-Specific",
			"Generic",
			"Migrated",
			"Localised",
			"Resignalled",
			"Dereferenced",
			"Nullified",
			"Aborted",
			"Serious",
			"Minor",
			"Major",
			"Extraneous",
			"Illegal",
			"Insufficient",
			"Viral",
			"Unsupported",
			"Outmoded",
			"Legacy",
			"Permanent",
			"Invalid",
			"Deprecated",
			"Virtual",
			"Unreportable",
			"Undetermined",
			"Undiagnosable",
			"Unfiltered",
			"Static",
			"Dynamic",
			"Delayed",
			"Immediate",
			"Nonfatal",
			"Fatal",
			"Non-Valid",
			"Unvalidated",
			"Non-Static",
			"Unreplicatable",
			"Non-Serious"
		};
	}
	else if(nothing == "bofh2.dat") // list 2
	{
		st = {
			"Array",
			"Systems",
			"Hardware",
			"Software",
			"Firmware",
			"Backplane",
			"Logic-Subsystem",
			"Integrity",
			"Subsystem",
			"Memory",
			"Comms",
			"Integrity",
			"Checksum",
			"Protocol",
			"Parity",
			"Bus",
			"Timing",
			"Synchronisation",
			"Topology",
			"Transmission",
			"Reception",
			"Stack",
			"Framing",
			"Code",
			"Programming",
			"Peripheral",
			"Environmental",
			"Loading",
			"Operation",
			"Parameter",
			"Syntax",
			"Initialisation",
			"Execution",
			"Resource",
			"Encryption",
			"Decryption",
			"File",
			"Precondition",
			"Authentication",
			"Paging",
			"Swapfile",
			"Service",
			"Gateway",
			"Request",
			"Proxy",
			"Media",
			"Registry",
			"Configuration",
			"Metadata",
			"Streaming",
			"Retrieval",
			"Installation",
			"Library",
			"Handler"
		};
	}
	else if(nothing == "bofh3.dat") // list 3
	{
		st = {
			"Interruption",
			"Destabilisation",
			"Destruction",
			"Desynchronisation",
			"Failure",
			"Dereferencing",
			"Overflow",
			"Underflow",
			"NMI",
			"Interrupt",
			"Corruption",
			"Anomoly",
			"Seizure",
			"Override",
			"Reclock",
			"Rejection",
			"Invalidation",
			"Halt",
			"Exhaustion",
			"Infection",
			"Incompatibility",
			"Timeout",
			"Expiry",
			"Unavailability",
			"Bug",
			"Condition",
			"Crash",
			"Dump",
			"Crashdump",
			"Stackdump",
			"Problem",
			"Lockout"
		};
	}
	else if(nothing == "bofh4.dat") // list 4
	{
		st = {
			"Error",
			"Problem",
			"Warning",
			"Signal",
			"Flag"
		};
	}
	else
	{
		cerr << "Error: '" << nothing << "' is not a valid internal list." << endl;
		return 1;
	}

	if(dump)
	{
		ofstream outf;
		outf.open(nothing.c_str());
		if(outf.is_open())
		{
			for(int i=0; i < st.word.size(); i++)
			{
				outf << st[i] << '\n';
			}
			outf.close();
		}
		else{
			outf.close();
			cerr << "Error: Could not open file '" << nothing << "' for writing" << endl;
			return 1;
		}
	}

	return 0;
}

int structurise(string filename, wordlist &st)
{
	if(bstat)
		return bofhstat(filename,st);

	ifstream datfile;
	string templine;

	// start reading datafiles into variables
	datfile.open(filename.c_str());
	if(!datfile.is_open())
	{
		cerr << "Error: " << filename << " could not be opened for read.";
		if(!nstat)
		{
			cerr << " Using built-in list" << endl;
			return bofhstat(filename,st);
		}
		else
		{
			cerr << " Try using '-static'." << endl;
			return 2;
		}
	}
	while (!datfile.eof())
	{
		getline(datfile,templine);
		// allow comments in the data files
		if(templine[0]!='/' && templine[0]!='\n' && templine[0]!='#' && templine[0]!=';')
		{
			st.push_back(templine);
		}
	}
	datfile.close();

	return 0;
}

int main (int argc, char *argv[])
{
#ifdef WIN32
	bool pauseAtEnd = true;	//most Windows users don't run from terminal, so pause at end.
#else
	bool pauseAtEnd = false;
#endif

	wordlist one;
	wordlist two;
	wordlist three;
	wordlist four;
	string list1 = "bofh1.dat";
	string list2 = "bofh2.dat";
	string list3 = "bofh3.dat";
	string list4 = "bofh4.dat";
	#ifdef dumb
	bool dumb=true;
	#else
	bool dumb=false;
	#endif

	// parse arguments
	if(argc>1){
		if(strcmp(argv[1],"-p")==0)
			pauseAtEnd = !pauseAtEnd; // toggle for windows users
		else if(strcmp(argv[1],"-dumb")==0 || strcmp(argv[1],"-d")==0)
			dumb=true;
		else if(strcmp(argv[1],"-ndumb")==0 || strcmp(argv[1],"-nd")==0)
			dumb=false;
		else if(strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0 || strcmp(argv[1],"/?")==0)
		{
			cout << "BOFH EXCUSE GENERATOR\nCreated by Mason Heller\n\n"
			<< "The idea and all included words come from the official BOFH website at"
			<< "\nhttp://bofh.ntk.net/\n"
			<< "This program is freeware. Use and distribute as you please.\n"
			<< "Full source is available at http://github.com/masonh/bofh\n\n"
			<< "USAGE:\n\nRunning bofh with no arguments returns a random BOFH excuse"
			<< "\n\nbofh -dumb | bofh -ndumb\n"
			<< "Activates or deactivates optional fourth word, respectively\n\n"
			<< "bofh -static       | bofh -nstatic         | bofh -nostatic\n"
			<< "Use built-in lists | Default to data files | Disable built-in lists.\n\n"
			<< "bofh --dump\nDumps the builtin lists to data files\n\n"
			<< "bofh -h | bofh --help | bofh /?\nShow this help text and quit"
			<< endl;
			return 0;
		}
		else if(strcmp(argv[1],"--dump")==0)
		{
			dump=true;
			bofhstat(list1,one);
			bofhstat(list2,two);
			bofhstat(list3,three);
			bofhstat(list4,four);
			return 0;
		}
		else if(strcmp(argv[1],"-static")==0 || strcmp(argv[1],"-s")==0)
		{
			bstat=true;
		}
		else if(strcmp(argv[1],"-nstatic")==0 || strcmp(argv[1],"-ns")==0)
		{
			bstat=false;
		}
		else if(strcmp(argv[1],"-nostatic")==0 || strcmp(argv[1],"-nos")==0)
		{
			nstat=true;
			bstat=false;
		}
		else if(strcmp(argv[1],"-p")==0)
			pauseAtEnd = true;
		else if(strcmp(argv[1],"-np")==0)
			pauseAtEnd = false;
		else{
			cerr << "Unrecognised argument: '" << argv[1] << "'\n"
			<< "Try 'bofh -h' for a list of valid arguments" << endl;
			return 0;
		}
	}
	if(argc>2)
	{
		if(strcmp(argv[2],"-dumb")==0 || strcmp(argv[2],"-d")==0)
			dumb=true;
		else if(strcmp(argv[2],"-ndumb")==0 || strcmp(argv[2],"-nd")==0)
			dumb=false;
		else if(strcmp(argv[2],"-static")==0 || strcmp(argv[2],"-s")==0)
		{
			bstat=true;
		}
		else if(strcmp(argv[2],"-nstatic")==0 || strcmp(argv[2],"-ns")==0)
		{
			bstat=false;
		}
		else if(strcmp(argv[2],"-nostatic")==0 || strcmp(argv[2],"-nos")==0)
		{
			nstat=true;
			bstat=false;
		}
		else if(strcmp(argv[2],"-p")==0)
			pauseAtEnd = true;
		else if(strcmp(argv[2],"-np")==0)
			pauseAtEnd = false;
		else{
			cerr << "Unrecognised argument: '" << argv[2] << "'" << endl;
		}
	}
	//process lists
	if(structurise(list1,one)==2)
	{
		cerr << "Fatal Error. Exiting..." << endl;
		return 1;
	}
	if(structurise(list2,two)==2)
	{
		cerr << "Fatal Error. Exiting..." << endl;
		return 1;
	}
	if(structurise(list3,three)==2)
	{
		cerr << "Fatal Error. Exiting..." << endl;
		return 1;
	}


	if(dumb)	// check if we want the fourth word
		if(structurise(list4,four)==2)
		{
			cerr << "Fatal Error. Exiting..." << endl;
			return 1;
		}

	std::random_device randdev;   // a random device used to seed the Mersenne Twister
	std::mt19937 rng(randdev());  // the Mersenne Twister with a popular choice of parameters

	// create random distributions for the words
	std::uniform_int_distribution<uint32_t> uint_dist1(0,one.size()-1);
	std::uniform_int_distribution<uint32_t> uint_dist2(0,two.size()-1);
	std::uniform_int_distribution<uint32_t> uint_dist3(0,three.size()-1);
	std::uniform_int_distribution<uint32_t> uint_dist4(0,four.size()-1);

	if(dumb)
	{
		cout << one[uint_dist1(rng)] << " "
			<< two[uint_dist2(rng)] << " "
			<< three[uint_dist3(rng)] << " "
			<< four[uint_dist4(rng)] << endl;
	}
	else
	{
		cout << one[uint_dist1(rng)] << " "
			<< two[uint_dist2(rng)] << " "
			<< three[uint_dist3(rng)] << endl;
	}

	if(pauseAtEnd)
	{
		cout << "\nPress return to continue..." << endl;
#ifdef _INC_CONIO
		_getch();
#else
		cin.get();
#endif
	}

	return 0;
}
