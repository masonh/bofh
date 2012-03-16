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
 * If this file is compiled the bofhstatic defined, the lists in the code will
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
 * It can only return one unique bofh excuse per second because rand() is
 * seeded with time(), which returns seconds. I don't think this will be an
 * issue for most people, though.
 */

/* TO DO:
 * Make commandline flags/arguments more gnu-like
 * Redo argument parsing to be more dynamic
 * Now that it's fixed for Windows, recheck in Linux
 * Use C++ features
 */



#define bofhstatic
//#define dumb
#include <stdio.h>
#ifdef WIN32
#include <conio.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

bool dump=false;	// default to not output data files

#ifdef bofhstatic	// default to builtin lists
bool bstat=true;	
#else				// defualt to external lists
bool bstat=false;
#endif				// disable builtin lists
bool nstat=false;

// fix for windows functions
#ifndef WIN32
#define strcpy_s strcpy
#endif


struct wordlist
{
	int pos;
	char word[256][64];
};

int bofhstat(char nothing[16], wordlist &st)
{
	char datas[2048];
	bool exitloop=false;
	st.pos=0;
	int pos1=0;
	int pos2=0;
	if(strcmp(nothing,"bofh1.dat")==0) // list 1
	{
		strcpy_s(datas,"\
Temporary\n\
Intermittant\n\
Partial\n\
Redundant\n\
Total\n\
Multiplexed\n\
Inherent\n\
Duplicated\n\
Dual-Homed\n\
Synchronous\n\
Bidirectional\n\
Serial\n\
Asynchronous\n\
Multiple\n\
Replicated\n\
Non-Replicated\n\
Unregistered\n\
Non-Specific\n\
Generic\n\
Migrated\n\
Localised\n\
Resignalled\n\
Dereferenced\n\
Nullified\n\
Aborted\n\
Serious\n\
Minor\n\
Major\n\
Extraneous\n\
Illegal\n\
Insufficient\n\
Viral\n\
Unsupported\n\
Outmoded\n\
Legacy\n\
Permanent\n\
Invalid\n\
Deprecated\n\
Virtual\n\
Unreportable\n\
Undetermined\n\
Undiagnosable\n\
Unfiltered\n\
Static\n\
Dynamic\n\
Delayed\n\
Immediate\n\
Nonfatal\n\
Fatal\n\
Non-Valid\n\
Unvalidated\n\
Non-Static\n\
Unreplicatable\n\
Non-Serious\n");
	}
	else if(strcmp(nothing,"bofh2.dat")==0) // list 2
	{
		strcpy_s(datas,"\
Array\n\
Systems\n\
Hardware\n\
Software\n\
Firmware\n\
Backplane\n\
Logic-Subsystem\n\
Integrity\n\
Subsystem\n\
Memory\n\
Comms\n\
Integrity\n\
Checksum\n\
Protocol\n\
Parity\n\
Bus\n\
Timing\n\
Synchronisation\n\
Topology\n\
Transmission\n\
Reception\n\
Stack\n\
Framing\n\
Code\n\
Programming\n\
Peripheral\n\
Environmental\n\
Loading\n\
Operation\n\
Parameter\n\
Syntax\n\
Initialisation\n\
Execution\n\
Resource\n\
Encryption\n\
Decryption\n\
File\n\
Precondition\n\
Authentication\n\
Paging\n\
Swapfile\n\
Service\n\
Gateway\n\
Request\n\
Proxy\n\
Media\n\
Registry\n\
Configuration\n\
Metadata\n\
Streaming\n\
Retrieval\n\
Installation\n\
Library\n\
Handler\n");
	}
	else if(strcmp(nothing,"bofh3.dat")==0) // list 3
	{
		strcpy_s(datas,"\
Interruption\n\
Destabilisation\n\
Destruction\n\
Desynchronisation\n\
Failure\n\
Dereferencing\n\
Overflow\n\
Underflow\n\
NMI\n\
Interrupt\n\
Corruption\n\
Anomoly\n\
Seizure\n\
Override\n\
Reclock\n\
Rejection\n\
Invalidation\n\
Halt\n\
Exhaustion\n\
Infection\n\
Incompatibility\n\
Timeout\n\
Expiry\n\
Unavailability\n\
Bug\n\
Condition\n\
Crash\n\
Dump\n\
Crashdump\n\
Stackdump\n\
Problem\n\
Lockout\n");
	}
	else if(strcmp(nothing,"bofh4.dat")==0) // list 4
	{
		strcpy_s(datas,"\
Error\n\
Problem\n\
Warning\n\
Signal\n\
Flag\n");
	}
	else
	{
		fprintf(stderr,"Error: '%s' is not a valid internal list.\n",nothing);
		return 1;
	}

	if(dump)
	{
		FILE* outf;
		#ifdef WIN32
		if((fopen_s(&outf,nothing,"w"))!=NULL)
		#else
		if((outf = fopen(nothing,"w"))!=NULL)
		#endif
		{
			fprintf(outf,"%s",datas);
		}
		else{
			fprintf(stderr,"Error: Could not open file '%s' for writing\n",nothing);
			_fcloseall();
			return 1;
		}
		fclose(outf);
		return 0;
	}

	// start reading data into variables
	while (exitloop==false)
	{
		while(datas[pos2]!='\n')
		{
			st.word[st.pos][pos1]=datas[pos2];
			pos2++;
			pos1++;
		}
		st.pos++;
		pos1=0;
		pos2++;
		if(datas[pos2]=='\0')
			exitloop=true;
		if(st.pos>255)
			fprintf(stderr,"Error: %s internal list max length exceeded. Truncated.\n",nothing);
	}

	return 0;
}

int structurise(char filename[16], wordlist &st)
{
	if(bstat){
		//printf("%s\n",filename);	//for debug
		return bofhstat(filename,st);
	}

	FILE* datfile;
	char templine[64];
	st.pos=0;

	// start reading datafiles into variables
	#ifdef WIN32
	if((fopen_s(&datfile,filename,"r"))==NULL)
	#else
	if((datfile = fopen(filename,"r"))==NULL)
	#endif
	{
		fprintf(stderr,"Error: %s could not be opened for read.",filename);
		if(!nstat){
			fprintf(stderr," Using built-in list\n");
			return bofhstat(filename,st);}
		else{
			fprintf(stderr," Try using '-static'.\n");
			return 2;}
	}
	while ((fscanf_s(datfile,"%63[^\n]\n",templine))!=EOF)
	{
		if(templine[0]!='/' && templine[0]!='\n'){
	    strcpy_s(st.word[st.pos],templine);
	    st.pos++;
	    if (st.pos > 255)
	    {
	    	fprintf(stderr,"Error: %s max file length exceeded. Input truncated.\n",filename);
			break;
	    }}
	}
	fclose(datfile);

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
	char list1[15]="bofh1.dat",list2[15]="bofh2.dat",list3[15]="bofh3.dat",list4[15]="bofh4.dat";
	four.pos=0;
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
			printf("BOFH EXCUSE GENERATOR\nCreated by Mason Heller\n\n");
			printf("The idea and all included words come from the official BOFH website at");
			printf("\nhttp://bofh.ntk.net/\n");
			printf("This program is freeware. Use and distribute as you please.\n");
			printf("For the full source, email me at masonph@gmail.com\n\n");
			printf("USAGE:\n\nRunning bofh with no arguments returns a random BOFH excuse");
			printf("\n\nbofh -dumb | bofh -ndumb\n");
			printf("Activates or deactivates optional fourth word, respectively\n\n");
			printf("bofh -static       | bofh -nstatic         | bofh -nostatic\n");
			printf("Use built-in lists | Default to data files | Disable built-in lists.\n\n");
			printf("bofh --dump\nDumps the builtin lists to data files\n\n");
			printf("bofh -h | bofh --help | bofh /?\nShow this help text and quit");
			printf("\n");
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
			printf("Unrecognised argument: '%s'\n",argv[1]);
			printf("Try 'bofh -h' for a list of valid arguments\n");
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
			fprintf(stderr,"Unrecognised argument: '%s'\n",argv[2]);
		}
	}
	//process lists
	if(structurise(list1,one)==2){
		fprintf(stderr,"Fatal Error. Exiting...\n");
		return 1;}
	if(structurise(list2,two)==2){
		fprintf(stderr,"Fatal Error. Exiting...\n");
		return 1;}
	if(structurise(list3,three)==2){
		fprintf(stderr,"Fatal Error. Exiting...\n");
		return 1;}


	if(dumb)	// check if we want the fourth word
		if(structurise(list4,four)==2){
			fprintf(stderr,"Fatal Error. Exiting...\n");
			return 1;}
	if(!dumb) // set it to output nothing for the fourth word (and give back the space)
	{
		four.word[0][0]='\b';	// the last word is a backspace
		four.pos=1;
	}

	srand(time(NULL));	//seed random so we get unique output each second. this means the program won't work for mass-generating but for normal use it is fine. linux can seed via /dev/urandom but windows has no such device.

	printf("%s %s %s %s\n",
		one.word[rand()%one.pos],
		two.word[rand()%two.pos],
		three.word[rand()%three.pos],
		four.word[rand()%four.pos]);	// pick a random word from each list and print them out.
#ifdef _INC_CONIO
	if(pauseAtEnd){
		printf("\nPress any key to continue...\n");
		_getch();
	}
#endif		

	return 0;
}
