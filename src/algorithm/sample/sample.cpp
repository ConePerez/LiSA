/**
 * @author  LiSA
 * @version 3.0
 */

// Include the header files for the used objects! Standard:
// LisaProblemtype requires global.hpp.
// The file global.hpp is needed by LisaProblemtype.
// The files ctrlpara.hpp, ptype.hpp and lvalues.hpp
// are used for parsing the input file.
// The object Lisa_Schedule (header: schedule.hpp) 
// is necessary for writing the result into the output file.
// The file except.hpp is used for the exception handling.  

#include <unistd.h>
#include <iostream>
#include <fstream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]) 
{

    // print a message that the program has started:
    cout << "This is the LiSA Sample Module" << endl;

    // The Lisa_Exceptionlist is forced for writing
    // error messages to cout. Then LiSA is able
    // to show the error messages of an external module.
    G_ExceptionList.set_output_to_cout();   

    // open files and assure existence:
    if (argc != 3) 
      {
        cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
        exit(1);
      }

    cout << "PID= " << getpid() << endl;

    // The both parameter of the module call are the name of the
    // input file and the name of the output file:
    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);
    if (!i_strm){
      cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
      exit(1);
    }
    if (!o_strm){
      cout << "ERROR: cannot find output file " << argv[1] << "." << endl;
      exit(1);
    }
    i_strm.close();
    o_strm.close();
    
    //initialize class
    LisaXmlFile::initialize();
    //create Input handler
    LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
    //communication objects
    Lisa_ProblemType Problem;
    Lisa_ControlParameters Parameter;   
    Lisa_Values Values;
    
    //parse xml file
    xmlInput.read(argv[1]);
    //determine document type
    LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
    
    //check for successful parsing and valid document type
    if (!xmlInput || !(type == LisaXmlFile::INSTANCE || type == LisaXmlFile::SOLUTION))
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
    //get Problem
    if( !(xmlInput >> Problem))
    {
      cout << "ERROR: cannot read ProblemType , aborting program." << endl;
      exit(1);
    }
    //get ControlParameters
    if( !(xmlInput >> Parameter))
    {
      cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
      exit(1);
    }
    //get Values
    if( !(xmlInput >> Values))
    {
      cout << "ERROR: cannot read Values , aborting program." << endl;
      exit(1);
    }
    // if something else went wrong
    if (!G_ExceptionList.empty())
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
    

    // Define the LiSA  schedule object for storing results
    Lisa_Schedule out_schedule(Values.get_n(),Values.get_m());
    out_schedule.make_LR();
    
    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************
    
    int stages=MAX(Values.get_n(),Values.get_m());
    int i,j;
    
    for (i=0; i <Values.get_n(); i++)
      for (j=0; j <Values.get_m(); j++)
	if ((*Values.SIJ)[i][j])
	  (*out_schedule.LR)[i][j]= ((i+j) % stages)+1;

    // The following lines demonstrate how to write into an extra LiSA window:
     cout << "WARNING: The Problemtype is:" << Problem.output_problem()<< endl;
     cout << "WARNING: The upper bound is:" << Parameter.get_double("UPPER_BOUND")<< endl;
     cout << "WARNING: P(1,2)=" << (*Values.PT)[1][2]<< endl;
     cout << "WARNING: Name:" << Parameter.get_string("NAME")<< endl;

    // ***************************************************************
    // ********************* End of Algorithm ************************ 
    // ***************************************************************
    
    // The object out_schedule contain the result of this algorithm,
    // which is written into the output file
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    //pipe objects to this
    xmlOutput << Problem << Values << Parameter << out_schedule;
    //write content to a file
    xmlOutput.write(argv[2]);
}

//**************************************************************************

