#include <iostream>
#include <string>
#include <sstream>


using std::cin;
using std::cout;
using std::endl;



char* reader();

int main(){
    reader();
}

char* reader(){


    const int INPUTSIZE = 5000;
    const int QUITLEN = 4;

    char* usrInput = new char[INPUTSIZE];
    unsigned endCharInd=0;
    char nextChar;
    unsigned parenthesisCount = 0;
    std::string line;
    bool quit(false);

    while(!quit){


        // format prompt and add '#' to usrInput
        if(parenthesisCount == 0) cout<<"--> ";
        else cout<<"> ";
        
        // read input line
        std::getline(cin,line);
        
        // remove leading space
        line.erase(0, line.find_first_not_of(' '));
        std::istringstream ss(line);
        
        // parse input line
        while(ss >> std::noskipws >> nextChar){
            
            
            // parse every character
            switch(nextChar)
            {
                case '(':
                    usrInput[endCharInd++] = nextChar;
                    ++parenthesisCount;
                    break;

                case ')':

                    if(parenthesisCount > 0){
                        usrInput[endCharInd++] = nextChar;
                        --parenthesisCount;
                    } else cout << "enter '(' first"<<endl;
                    break;

                case ';':
                    if(parenthesisCount==0) ss.str("");
                    else ss.str("");
                    break;

                default:
                    usrInput[endCharInd++] = nextChar;
                    break;
            }
        
            
        }  // END inner while


        // check if user enters quit
        if(QUITLEN==endCharInd  && usrInput[0] == 'q' && usrInput[1] == 'u'
            && usrInput[2] == 'i' && usrInput[3] == 't')  quit=true;
        
        // print input & clear array
        if(endCharInd != 0 && parenthesisCount == 0){

            // print input
            for (unsigned i = 0; i < endCharInd; i++) cout<<usrInput[i];

            // add end of input character
            usrInput[endCharInd++] = '#';


            cout << endl;
        }

        // clear array, no other reset needed to implement reader
        if(parenthesisCount==0)endCharInd = 0; // fastest way
        // alternative std::fill_n(usrInput.begin(), endCharInd, '')

        // following reset is unnecessary
        // nextChar ='';

    } // END outer while



    cout<<endl;



    return usrInput;
}
