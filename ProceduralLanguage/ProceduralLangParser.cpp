#include<iostream>
#include<string.h>
using namespace std;

/*****************************************************************
 *                     DECLARATIONS                              *
 *****************************************************************/
typedef int NUMBER;
typedef int NAME;
const int  NAMELENG = 20;      /* Maximum length of a name */
const int  MAXNAMES = 100;     /* Maximum number of different names */
const int  MAXINPUT = 500;     /* Maximum length of an input */
const char*   PROMPT = "-> ";
const char*   PROMPT2 = "> ";
const char  COMMENTCHAR = ';';
const int   TABCODE = 9;        /* in ASCII */

struct EXPLISTREC;
typedef EXPLISTREC* EXPLIST;
enum EXPTYPE {VALEXP,VAREXP,APEXP};
struct EXPREC
{
	  EXPTYPE etype; //what type of expression
	  NUMBER num;
	  NAME varble;
	  NAME optr;
	  EXPLIST args;
};

typedef EXPREC* EXP;  //forward declaration
struct EXPLISTREC
{
	  EXP head;
	  EXPLIST tail;
};


struct VALUELISTREC
{
	  NUMBER  head;
	  VALUELISTREC*  tail;
};

typedef VALUELISTREC* VALUELIST;  //forward declaration
struct NAMELISTREC
{
	  NAME   head;
	  NAMELISTREC* tail;
};

typedef NAMELISTREC* NAMELIST;  //forward declaration
struct  ENVREC
{
	   NAMELIST vars;
	   VALUELIST values;
};

typedef ENVREC* ENV;   //forward declaration
struct  FUNDEFREC
{
	   NAME  funname;
	   NAMELIST  formals;
	   EXP  body;
	   FUNDEFREC*  nextfundef;
};

typedef FUNDEFREC* FUNDEF;  //forward declaration
FUNDEF  fundefs;   //points to the head of the linked list of all the funcions
ENV globalEnv;     // keeps track of all the global variables
EXP currentExp;    //pointer to the current expression
char userinput[MAXINPUT];   //all the cleaned up userinput terminated with #
int   inputleng, pos;       // pos is at the current character of userinput

char*   printNames[MAXNAMES];   // symbol table
int   numNames, numBuiltins;    

int   quittingtime;  //HALT when true



/*****************************************************************
 *                     DATA STRUCTURE OP'S                       *
 *****************************************************************/

/* mkVALEXP - return an EXP of type VALEXP with num n            */

EXP mkVALEXP ( NUMBER n)
{
   EXP e;
   e = new EXPREC;
   e->etype = VALEXP;
   e->num = n;
   return e;
}/* mkVALEXP */


/* mkVAREXP - return an EXP of type VAREXP with varble nm  */

EXP mkVAREXP ( NAME nm)
{
    EXP e;
    e = new EXPREC;
    e->etype = VAREXP;
    e->varble = nm;
    return e;

}/* mkVAREXP */


/* mkAPEXP - return EXP of type APEXP w/ optr op and args el     */



//you write the code
EXP mkAPEXP (NAME op, EXPLIST el)
{

   EXP e;
   e = new EXPREC;
   e->etype = APEXP;
   e->optr = op;
   e->args = el;
   return e;

}/* mkAPEXP */

/* mkExplist - return an EXPLIST with head e and tail el         */

EXPLIST mkExplist (EXP e, EXPLIST el)
{
    EXPLIST el2 = new EXPLISTREC;
    el2->tail = el;
    el2->head = e;
    return el2;
 //you write the code

}/* mkExplist */

/* mkNamelist - return a NAMELIST with head n and tail nl        */

NAMELIST mkNamelist ( NAME nm, NAMELIST nl)
{

    NAMELIST n = new NAMELISTREC;
    n->head = nm;
    n->tail = nl;
    return n;

}/* mkNamelist */

/* mkValuelist - return an VALUELIST with head n and tail vl     */



   //you write the code
VALUELIST mkValuelist (NUMBER n,  VALUELIST vl)
{

   VALUELIST v = new VALUELISTREC;
   v->head = n;
   v->tail = vl;
   return v;

}/* mkValuelist */



  //you write the code
/* newFunDef - add new function fname w/ parameters nl, body e   */

void  newFunDef (NAME fname,  NAMELIST nl, EXP e)
{

   FUNDEF f;
   f = new FUNDEFREC;
   f->funname = fname;
   f->formals = nl;
   f->body = e;
   f->nextfundef = fundefs;
   fundefs = f;

}// newFunDef


/* initNames - place all pre-defined names into printNames */
/* this is our symbol table */

void initNames()
{
   int i =0;
   fundefs = 0;
   printNames[i] = (char* ) "if"; i++;
   printNames[i] = (char* ) "while"; i++;
   printNames[i] = (char* ) "set"; i++;
   printNames[i] = (char* ) "begin"; i++;
   printNames[i] = (char* ) "+"; i++;
   printNames[i] = (char* ) "-"; i++;
   printNames[i] = (char* ) "*"; i++;
   printNames[i] = (char* ) "/"; i++;
   printNames[i] = (char* ) "="; i++;
   printNames[i] = (char* ) "<"; i++;
   printNames[i] = (char* ) ">"; i++;
   printNames[i] = (char* ) "print";
   numNames = i;
   numBuiltins = i;
}//initNames

/* install - insert new name into printNames  */

NAME install ( char* nm)
{
   int i = 0;
   while (i <= numNames)
	 if (strcmp( nm,printNames[i] ) == 0)
	    break;
	 else
	   i++;
   if (i > numNames)
   {
	  numNames = i;
	  printNames[i] = new char[strlen(nm) + 1];
	  strcpy(printNames[i], nm);
   }
   return i;
}// install

/* prName - print name nm              */

void prName ( NAME nm)
{
	 cout<< printNames[nm];
} //prName

/*****************************************************************
 *                        INPUT                                  *
 *****************************************************************/

/* isDelim - check if c is a delimiter   */

int isDelim (char c)
{
   return ( ( c == '(') || ( c == ')') ||( c == ' ')||( c== COMMENTCHAR) );
}

/* skipblanks - return next non-blank position in userinput */

int skipblanks (int p)
{
   while (userinput[p] == ' ')
	++p;
   return p;
}


/* parseName - return (installed) NAME starting at userinput[pos]*/

NAME parseName()
{
   char nm[20]; // array to accumulate characters
   int leng; // length of name
   leng = 0;
   pos = skipblanks(pos);
   while ( (pos <= inputleng) && !isDelim(userinput[pos]) )
   {
	    ++leng;
	    nm[leng-1] = userinput[pos];
	   ++pos;
   }
   if (leng == 0)
   {
	   cout<<"Error: expected name, instead read: "<< userinput[pos]<<endl;
	   exit(1);
   }
   nm[leng] = '\0';
   pos = skipblanks(pos); // skip blanks after name
   return ( install(nm) );
}// parseName


/* isDigits - check if sequence of digits begins at pos   */

int isDigits (int pos)
{
   if ( ( userinput[pos] < '0' ) ||  ( userinput[pos] > '9' ) )
	  return 0;
   while ( ( userinput[pos] >='0') && ( userinput[pos] <= '9') )
	 ++pos;
   if (!isDelim(userinput[pos] ))
	 return 0;
   return 1;
}// isDigits


/* isNumber - check if a number begins at pos  */

int isNumber (int pos)
{
   return ( isDigits(pos) || 
             ( (userinput[pos] == '-') && isDigits(pos+1))
            ||( (userinput[pos] == '+') && isDigits(pos+1)));
}// isNumber

/* parseVal - return number starting at userinput[pos]   */

NUMBER parseVal()
{
   char val[11]; // array to store val
   int leng = 0; // length of val
   pos = skipblanks(pos);

   if(userinput[pos] == '-' || userinput[pos] == '+' || isDigits(pos))
      val[leng++] = userinput[pos++];

   while ( (pos <= inputleng) && isdigit(pos))
   {
	   val[leng++] = userinput[pos++];
   }
   if (leng == 0 || (leng==1 && (userinput[leng] == '-' || userinput[leng] == '+') ))
   {
	   cout<<"Error: expected number, instead read: "<< userinput[pos]<<endl;
	   exit(1);
   }
   
   return ( (NUMBER) atoi(val));
}// parseVal

EXPLIST parseEL();

/* parseExp - return EXP starting at userinput[pos]  */

EXP parseExp()
{
    pos = skipblanks(pos);
    if (userinput[pos] ==  '(' ) {
        pos = skipblanks(pos+1);
        NAME op = parseName();
        EXPLIST el = parseEL();
        return mkAPEXP(op, el);
    }
    
    if (isNumber(pos))
        return mkVALEXP((NUMBER) parseVal());
    return mkVAREXP ((NAME) parseName());
}// parseExp

/* parseEL - return EXPLIST starting at userinput[pos]  */
EXPLIST parseEL()
{
    pos = skipblanks(pos);
    if (userinput[pos] ==  ')' ) {
        pos = skipblanks(pos+1);
        return 0;
    }
    EXP e = parseExp();
    EXPLIST el = parseEL();
    return mkExplist(e, el);

}// parseEL

/* parseNL - return NAMELIST starting at userinput[pos]  */

//  mkNamelist ( NAME nm, NAMELIST nl)
/* parseName - return (installed) NAME starting at userinput[pos]  NAME parseName()*/


// you write the code
NAMELIST parseNL()
{

   pos = skipblanks(pos);
   NAMELIST nl;
   while(userinput[pos] !=  ')'){
      nl = mkNamelist(parseName(),nl);  
      pos = skipblanks(pos);
   }
   pos = skipblanks(pos+1); // skip ') ..'


   return nl;
  
}// parseNL


// what if user enters define add   (x , y)  i.e. line 388 does not successfully skip (

/* parseDef - parse function definition at userinput[pos]   */

NAME parseDef()
{
    NAME fname;        // function name
    NAMELIST nl;       // formal parameters
    EXP e;             // body
    pos = skipblanks(pos+1); // skip '( ..'
    pos = skipblanks(pos+6); // skip 'define ..'
    fname = parseName();
    pos = skipblanks(pos+1); // skip '( ..'
    nl = parseNL();
    e = parseExp();
    pos = skipblanks(pos+1); // skip ') ..'
    newFunDef(fname, nl, e);
   return ( fname);
}// parseDef





/*****************************************************************
 *                     READ-EVAL-PRINT LOOP                      *
 *****************************************************************/


//ignore this for the moment

void main()
{
   
   initNames();
   globalEnv = emptyEnv();

   quittingtime = 0;
   while (!quittingtime)
   {
	 reader();
	 if ( matches(pos, 4, (char* ) "quit"))
	    quittingtime = 1;
	 else if( (userinput[pos] == '(') &&
		    matches(skipblanks(pos+1), 6, (char* )"define")  )
	 {
		    prName(parseDef());
		    cout <<endl;
	 }
	 else {
			currentExp = parseExp();
			prValue(eval(currentExp, emptyEnv() ));
			cout <<endl<<endl;
		 }
	}// while
}


