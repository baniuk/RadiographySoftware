#define IMAGE  0x00000010
#define STRING 0x00000100
#define MATRIX 0x00001000
#define NUMBER MATRIX
#define DT UINT
#define MAX_VER 6
#define MAX_DATE 16
#define MAX_TIME 16
#define MAX_NAME 256
#define MAX_DESC 256
#define MAX_SYNT 256
#define MAX_NUMBER_FUNCTION_PARAM 50
#define MAX_ERROR 512
#define MAX_STRING 1024

struct FUNC_DEF {
	char version[MAX_VER];		// function version
	char name[MAX_NAME];		// function name
	char syntax[MAX_SYNT];		// example of use
	char description[MAX_DESC];	// whatever you want
	int numofout;				// number of input parameters
	int numofin;				// number of output parameters
	UINT typesofin[MAX_NUMBER_FUNCTION_PARAM];			// table of parameters type 
	UINT typesofout[MAX_NUMBER_FUNCTION_PARAM];
	unsigned int interface_version;		// version of interface
	double ADDIP_version;		// version of the ADDIP that uses this command
	char fdate[MAX_DATE];		// compilation date
	char ftime[MAX_TIME];		// compilation time Wszystko dodane do tej struktury musi byc na koncu bo inaczej trzeba by przekompilowywac wszystkie dll
};