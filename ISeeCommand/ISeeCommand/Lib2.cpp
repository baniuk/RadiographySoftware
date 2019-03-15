/*****************************************************************************************
Program: Wysy³anie przez interfejs telnetowy dowolnych komend do programu ISee! Lista
	dostêpnych komend do znalezienia pod adresem: 
	http://www.kb.bam.de/~alex/interfacing-with-ic/
Autor: Lech Napiera³a
Parametry wejœciowe:
	o - komenda w formacie ISee! do wys³ania
Parametry wyjœciowe:
	bufor2 - odpowiedŸ ISee! na dan¹ komendê. Je¿eli ISee! nie odpowiada nic wartoœciowego,
	podmieniane na "OK".
*****************************************************************************************/


#include "Cpp_Lib.h"
#include<stdio.h>
#include <windows.h>
//#include <process.h>


#define SOCKET_READY 0x08000 // special message indicating a socket is ready 

char bufor[200], bufor2[200];
char koniec[] = "exit\n";
//char polecenie[] = "savedatasetas c:\\set.txt\n";
int brejk = 0;	//do prymitywnego wykonywania dwóch czytañ i jednego zapisu
int port = 23, th;
//MSG msg;// A Win32 message structure. 
FILE * plik, * plik2;
C_Matrix_Container *ppp;

class telnetConnection
{
    public:
    char   myname[256];
    SOCKET sock;
    struct sockaddr_in sa;
    struct hostent *hp;
    
    int start(HWND* hwndMain)
    {
		//plik = fopen("c:/kalafior2.txt", "a");
		//fprintf(plik, "Linijka testowa\n");
		//fclose(plik);
		/*
		for(th=0;th<*(lastglobal);th++)
		{
           if((*globals)[th]!=NULL)        
		   {
			  if(strcmp("iseeport",(*globals)[th]->name)==0)  
			  {
				global_proc = (C_Matrix_Container*)(*globals)[th]->data;
                port = (char)global_proc->data[0];
              }
           }
		}
		*/

        WSADATA info;
        if (WSAStartup(MAKEWORD(2,0), &info) != 0) return -1;
        gethostname(myname, sizeof(myname));
        //cout << myname << endl;
        hp = gethostbyname(myname);
        //hp = gethostbyname("www.wp.pl");
        
        if (hp == NULL) return -1;
        //cout << hp << endl;
        memset(&sa,0,sizeof(sa));
        memcpy((char *)&sa.sin_addr, hp->h_addr, hp->h_length);   // set address 
        sa.sin_family = hp->h_addrtype;
        sa.sin_port = htons((u_short)port);
        //sa.sin_port = htons((u_short)80);
        
        sock = socket(hp->h_addrtype, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) return -1;
        if (connect(sock, (struct sockaddr *)&sa, sizeof sa) == SOCKET_ERROR) 
        {
            closesocket(sock);
            return -1;
        }
        if(WSAAsyncSelect(sock, *hwndMain, SOCKET_READY, FD_READ) != 0) return -1;
        return 0;
    }
    
    void slij(char *b, int size)
    {
         //*(b + size) = '\255';
         //cout << send(sock, b, size + 1, 0) << " bytes send" << endl;
         //cout << send(sock, b, size, 0) << " bytes send" << endl;
         //cout << write(sock, b, size) << " bytes send" << endl;
         int i = send(sock, b, size, 0);
         //int i = sendto(sock, b, size, 0);
         
         //char b[8];
         //sprintf(&b, "%d", i);
         //fwrite(&b, sizeof(b), 1, plik);
		 
		 plik = fopen("c:/kalafior2.txt", "a");
         fprintf(plik, "Wys³ano %d znaków:\n", i);
         fprintf(plik, b);
         fprintf(plik, "\n");
		 fclose(plik);
		 
         
         //send(sock, b, size, 0);
         
         /*
         for(i = 0; i < size; i++)
         {
               send(sock, b + i, 1, 0);
               //b++;
         }
         */
         
    }
    
    void odbierz(char* b)
    {
         //while(recv(sock, b, 200, 0) <= 0);
         //cout << recv(sock, b, 200, 0) << " bytes received" << endl;
         //cout << read(sock, b, 200) << " bytes received" << endl;
         
         int i = recv(sock, b, 200, 0);
		 
		 plik = fopen("c:/kalafior2.txt", "a");
         fprintf(plik, "Odebrano %d znaków:\n", i);
         fprintf(plik, b);
         fprintf(plik, "\n");
		 fclose(plik);
    }
    
    void stop()
    {
	    //fclose(plik);
        closesocket(sock);
        WSACleanup();
    }
};

class telnetConnection tC;


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"ISeeCommand");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"STRING = ISeeCommand(STRING)");
	definition->numofout = 1;
	definition->numofin = 1;
	definition->typesofin[0] = STRING;
	definition->typesofout[0] = STRING;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"LN");
	definition->ADDIP_version = 2.0;

}

void AssignGlobals(C_Variables *(*p_globals)[MAX_GLOBALS],int* p_lastglobal)
{
	int i;
	//FILE * plik;
	//plik = fopen("c:/bizon.txt", "w");
    globals = p_globals;
    lastglobal = p_lastglobal;
    for(i = 0; i < *(lastglobal); i++)
	{
        if((*globals)[i]!=NULL)
	    {
            if(strcmp("iseeport", (*globals)[i]->name) == 0) 
			{
                ppp = (C_Matrix_Container*)(*globals)[i]->data;
                port = (int)ppp->data[0];
            }
	    }
	}
	//fprintf(plik, "%d", n);
	//fclose(plik);
}

/*
unsigned int WINAPI Czekanie(void *param)
{
	while (GetMessage (&msg, NULL, 0, 0))
	{
		/*
		plik2 = fopen("c:/kalafior.txt", "a");
		fprintf(plik2, "Dosta³em wiadomoœæ %d\n", brejk);
		fwrite(&msg, sizeof(msg), 1, plik2);
		fclose(plik2);
		*
		
 		TranslateMessage (&msg);
		DispatchMessage (&msg);
		
		//fwrite(&bufor, sizeof(bufor), 1, plik);
		//fputc('\n', plik);
		//fwrite(&msg, sizeof(msg), 1, plik);
		//fputc('\n', plik);
		
		/*
		if(brejk == 0) 
		{
			
		}
		if(brejk == 2) PostQuitMessage(0);
		brejk++;
		*
		
		//if(brejk == 2) PostQuitMessage(0);
	}
	return(0);
}
*/

LRESULT CALLBACK MainWndProc2 (HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    // The window handle for the "Click Me" button. 
    //static HWND hwndButton = 0;
    //static int cx, cy;/* Height and width of our button. 
    
	/*
    HDC hdc;/* A device context used for drawing 
    PAINTSTRUCT ps;/* Also used during window drawing 
    RECT rc;/* A rectangle used during drawing 
    
    // Perform processing based on what kind of message we got.
	*/

	/*
	plik2 = fopen("c:/kalafiorr.txt", "a");
	fprintf(plik2, "Jestem w callback-u\n");
	fprintf(plik2, "brejk = %d\n", brejk);
	fwrite(&nMsg, sizeof(nMsg), 1, plik2);
	fclose(plik2);
	*/
     
    switch (nMsg)
    {
		
	    case WM_CREATE:
    		 //{
				/*
  		        /*
			  	HMENU hMenu, hSubMenu;
				HICON hIcon, hIconSm;

				hMenu = CreateMenu();
				hSubMenu = CreatePopupMenu();
				AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
				AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
				hSubMenu = CreatePopupMenu();
				AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
				AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");
				SetMenu(hwnd, hMenu);	   
				/*
			  		   
			  		   
			    /* The window is being created. Create our button
			     * window now. */
				 /*
			    TEXTMETRIC tm;
			    
			    /* First we use the system fixed font size to choose
			     * a nice button size. */
				 /*
			    hdc = GetDC (hwnd);
			    SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT));
			    GetTextMetrics (hdc, &tm);
			    cx = tm.tmAveCharWidth * 30;
			    cy = (tm.tmHeight + tm.tmExternalLeading) * 2;
			    ReleaseDC (hwnd, hdc);
			    
			    /* Now create the button */
				 /*
			    hwndButton = CreateWindow (
			    "button",/* Builtin button class */
				 /*
			    "Send",
			    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			    0, 0, cx, cy,
			    hwnd,/* Parent is this window. */
				 /*
			    (HMENU) 1,/* Control ID: 1 */
				 /*
			    ((LPCREATESTRUCT) lParam)->hInstance,
			    NULL
			    );
	    		//return 0;
				*/
				break;
				
        	//}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			 // The window is being destroyed, close the application
		 	 // (the child button gets destroyed automatically). 
		 	 //tC.stop();
			 PostQuitMessage(0);
			 //return 0;
  			 break;
		/*
        case WM_PAINT:
			 // The window needs to be painted (redrawn). 
			 hdc = BeginPaint (hwnd, &ps);
			 GetClientRect (hwnd, &rc);
		
			 // Draw "Hello, World" in the middle of the upper
		 	 // half of the window. 
			 rc.bottom = rc.bottom / 2;
			 DrawText (hdc, bufor, -1, &rc,
			 DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
			 EndPaint (hwnd, &ps);
			 //return 0;
			 break;
			 
        case WM_SIZE:
			// The window size is changing. If the button exists
			// then place it in the center of the bottom half of
			// the window. 
			if (hwndButton && (wParam == SIZEFULLSCREEN || wParam == SIZENORMAL))
			{
				rc.left = (LOWORD(lParam) - cx) / 2;
				rc.top = HIWORD(lParam) * 3 / 4 - cy / 2;
				MoveWindow(hwndButton, rc.left, rc.top, cx, cy, TRUE);
			}
			break;
		*/
			break;
		case SOCKET_READY:
	        //MessageBox(NULL, (LPCWSTR)"Odbieram", (LPCWSTR)"Note", MB_OK);
	 	    if(brejk == 0) tC.odbierz(&bufor[0]);
			if(brejk == 2) tC.odbierz(&bufor[0]);
		    brejk++;
			
			
			
				//PostMessage(hwnd, WM_PAINT, 0, 0);

			break; 
 		default:
			 // If we don't handle a message completely we hand it to the system
	 		 // provided default window function. 
	 		 return DefWindowProc (hwnd, nMsg, wParam, lParam);
    }
    return 0;
}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	int i;
	char *o, oo[200];
	char pom[] = "\n";
	brejk = 0;
		
	HWND hwndMain;// Handle for the main window. 
	MSG msg;// A Win32 message structure. 
	WNDCLASSEX wndclass2;
	HINSTANCE hInst = NULL;
	char*szMainWndClass = "WinTestWin";
	
	memset (&wndclass2, 0, sizeof(WNDCLASSEX));
	
	
	wndclass2.lpszClassName = (LPCWSTR)szMainWndClass;
	
	
	wndclass2.cbSize = sizeof(WNDCLASSEX);
	
	
	wndclass2.style = CS_HREDRAW | CS_VREDRAW;
	
	
	wndclass2.lpfnWndProc = MainWndProc2;
	
	
	wndclass2.hInstance = hInst;
	
	
	wndclass2.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wndclass2.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	wndclass2.hCursor = LoadCursor (NULL, IDC_ARROW);
	
	
	wndclass2.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	
	RegisterClassEx (&wndclass2);
	
	hwndMain = CreateWindow (
	(LPCWSTR)szMainWndClass,
	(LPCWSTR)szMainWndClass,
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	NULL,
	NULL,
	hInst,
	NULL
	);

	//Przetwarzanie komendy wejœciowej
	//char *o, oo[200];
	o = (char*)intab[0]->data;
	i = 0;
	while(*(o + i) != '\0')
	{
		oo[i] = *(o + i);
		i++;
	}
	oo[i] = '\n';

	
	// outtab[0] = "Error!";
	//tC.odbierz(&bufor[0]);
	//tC.slij(oo, i + 1);
	//hThread = (HANDLE)_beginthreadex(NULL, 0, Czekanie, NULL, 0, NULL);
	//WaitForSingleObject(hThread, 2000);
	//CloseHandle(hThread);
	//outtab[0]->AssignValue((void*)bufor,err);
	//for(i = 0; i < 200; i++) bufor[i] = '\0';
	//tC.slij(oo, i + 1);
	//hThread = (HANDLE)_beginthreadex(NULL, 0, Czekanie, NULL, 0, NULL);
	//WaitForSingleObject(hThread, 2000);
	//CloseHandle(hThread);

	//hThread = (HANDLE)_beginthreadex(NULL, 0, Czekanie, NULL, 0, NULL);
	//PostQuitMessage(0);
	//WaitForSingleObject(hThread, 3000);
	//CloseHandle(hThread);
	
	tC.start(&hwndMain); 
	tC.odbierz(&bufor[0]);
	tC.slij(oo, i + 1);
	for(i = 0; i < 200; i++) bufor[i] = '\0';

	/*
	plik2 = fopen("c:/kalafiorr.txt", "a");
	fprintf(plik2, "Lynijka testova\n", brejk);
	fclose(plik2);
	*/

	//tC.odbierz(&bufor[0]);
	
	while (GetMessage (&msg, NULL, 0, 0) > 0)
	{
		/*		
		plik2 = fopen("c:/getCommand.txt", "a");
		fprintf(plik2, "Dosta³em wiadomoœæ, brejk = %d\n", brejk);
		fwrite(&msg, sizeof(msg), 1, plik2);
		fprintf(plik2, "\n\n");
		fclose(plik2);
		*/
		
		
	 	TranslateMessage (&msg);
		DispatchMessage (&msg);
		
		//fwrite(&bufor, sizeof(bufor), 1, plik);
		//fputc('\n', plik);
		//fwrite(&msg, sizeof(msg), 1, plik);
		//fputc('\n', plik);
		
		
		//if(brejk == 0) 
		//{
			
		//}
		//if(brejk == 2) PostQuitMessage(0);
		//brejk++;
		//
		
		if(brejk == 1) 
		{
			for(i = 0; i < 200; i++) bufor2[i] = bufor[i];
			tC.slij(&koniec[0], sizeof(koniec));
			brejk++;
		}
		if(brejk == 2) SendMessage(hwndMain, WM_CLOSE, 0, 0);//PostQuitMessage(0);
	}
	DestroyWindow(hwndMain);
		
	tC.stop();
	
	if(bufor2[0] == '\0' || bufor2[0] == '\n') 
	{
		bufor[0] = 'O';
		bufor[1] = 'K';
	}

	//time(&t1);
	
	outtab[0]->AssignValue((void*)bufor2,err);
}