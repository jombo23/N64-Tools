#include "AN8X.h"



AN8XSTRINGPARSER::AN8XSTRINGPARSER(void)
{
	_CurrPos = 0;
}

AN8XSTRINGPARSER::~AN8XSTRINGPARSER(void)
{

}

void AN8XSTRINGPARSER::SetString( char* i_FileString )
{
	_FileString = i_FileString;
	_CurrPos = 0;
}

int AN8XSTRINGPARSER::ReadLine( void )
{
	if(  _FileString[_CurrPos] == '\0' )
	{
		Line = "";
		return 0;
	}

	if(  _FileString[_CurrPos] == '\n' )
	{
		Line = "\n";
		++_CurrPos;
		return 2;
	}

	Line = "";
	while( _FileString[_CurrPos] != '\n' && _FileString[_CurrPos] != '\0' )
	{
		Line += _FileString[_CurrPos];
		++_CurrPos;
	}

	if(  _FileString[_CurrPos] == '\n' )
	{
		Line += _FileString[_CurrPos];
		++_CurrPos;
	}

	return 1;	
};


AN8XPARSER::AN8XPARSER(void)
{
	LineNbr = 0 ;  // num of the current line
	Line = "NO LINE AS BEEN READED YET";
	IsOpen = false;
}

AN8XPARSER::~AN8XPARSER(void)
{
}

const char* AN8XPARSER::GetLine(void)
{
	return Line.c_str();
}

bool AN8XPARSER::Open(std::string filename)
{
	_FileName = filename;

	_file.open ( filename.c_str() , std::ifstream::in | std::ifstream::binary);
	if ( !_file.good() )
	{
		IsOpen = false;
		return false;
	}
	IsOpen = true;
	return true;
};

void AN8XPARSER::Close(void)
{
	if( _file.is_open() == true )
		_file.close();
	IsOpen = false;
};

// -------------------------------------------
// Read one line
// -------------------------------------------
int AN8XPARSER::ReadLine(void)
{
	getline( _file,Line , '\n' );

	if( Line.length() == 1 )
		return 2;

	if ( !_file.good() )
		return 0;

	return 1;	
};

// -------------------------------------------
// Find a word in the current line
// -------------------------------------------
int AN8XPARSER::FindWord(std::string theWord)
{
	if( Line.find(theWord,0) != std::string::npos )
		return 1;
	return 0;
};

// -------------------------------------------
// Get a string between "" in the line
// -------------------------------------------
void AN8XPARSER::GetName(std::string* stringName)
{
	int start = (int)Line.find("\"",0);
	int end   = (int)Line.find("\"",start+1);

	*stringName = Line.substr(start+1,end - start-1);
}

// -------------------------------------------
// Get 2 strings between "" in the line
// -------------------------------------------
void AN8XPARSER::Get2Names(std::string *stringName,std::string *stringName2)
{
	//std::string str = CutStr("\"",1);
	int start = 0;
	start = (int)Line.find("\"",0);
	std::string str = Line.substr(start + 1 ,Line.length() - start - 1);


	int end = (int)str.find("\"",0);

	*stringName = str.substr(0,end);

	//str = CutStr("\" \"",3);
	start = (int)Line.find("\" \"",0);
	str = Line.substr(start + 3 ,Line.length() - start - 3);



	end = (int)str.find("\"",0);
	*stringName2 = str.substr(0,end);
}

// -------------------------------------------
// Get position of a string in the line
// -------------------------------------------

int AN8XPARSER::IndexOf(int occuenceNbr, std::string theWord)
{
	int StringLenght,WordLenght;
	int iw,is;
	int found;
	int rtnValue = -1;

	iw=is=0;
	found=0;
	int start = -1;

	StringLenght=WordLenght=0;

	StringLenght = (int)Line.length();
	WordLenght   = (int)theWord.length();

	if(StringLenght < 1 || WordLenght < 1 || WordLenght > StringLenght)
		return 0;

	while( iw<WordLenght && is<StringLenght)
	{
		if(theWord[iw]==Line[is])
		{
			if(start == -1 )
				start = is;

			found+=1;
			if(found==WordLenght)
			{
				if(occuenceNbr == 1)
				{
					rtnValue = start;
					break;
				}
				else
				{
					--occuenceNbr;
					start = -1;
					found=0;
					iw=-1;
				}
		    }
			++iw;
		}
		else
		{
			start = -1;
		    found=0;
		    iw=0;
		}
		++is;
	};

	return rtnValue;
};

// -------------------------------------------
// Cut the line
// -------------------------------------------
void AN8XPARSER::CutLine(std::string strStart, int offset)
{
	int start = 0;
	if(strStart.length() > 0 )
		start = (int)Line.find(strStart,0);
	else
		start = 0;
	Line = Line.substr(start + offset ,Line.length() - start - offset);
	//return true;
}

std::string* AN8XPARSER::CutStr(std::string* io_pStrLine,std::string strStart, int offset)
{
	int start = 0;
	if(strStart.length() > 0 )
		start = (int)io_pStrLine->find(strStart,0);
	else
		start = 0;
	*io_pStrLine = io_pStrLine->substr(start + offset ,io_pStrLine->length() - start - offset);
	return io_pStrLine;
}

// -------------------------------------------
// Get a part of the line
// -------------------------------------------
/*
std::string AN8XPARSER::CutStr(std::string strStart, int offset)
{
	int start = 0;
	if(strStart.length() > 0 )
		start = (int)Line.find(strStart,0);
	else
		start = 0;
	return Line.substr(start + offset ,Line.length() - start - offset);
}
*/

// -------------------------------------------
// Get a part of a line between 2 characters
// -------------------------------------------
std::string AN8XPARSER::SubStr(std::string strStart, std::string strEnd)
{
	int start = (int)Line.find(strStart,0);
	int end   = (int)Line.find(strEnd,start+1);

	return Line.substr(start+strStart.length(),end - (start + strStart.length()) );
}

std::string AN8XPARSER::SubStr(std::string* i_pStrStart,std::string strStart, std::string strEnd)
{
	int start = (int)i_pStrStart->find(strStart,0);
	int end   = (int)i_pStrStart->find(strEnd,start+1);

	return i_pStrStart->substr(start+strStart.length(),end - (start + strStart.length()) );
}

bool AN8XPARSER::SubLine(int start, int end)
{
	Line = Line.substr(start,end);

	return true;
}

/*
bool AN8XPARSER::SubStr(std::string* dest, int start, int end)
{
	*dest = Line.substr(start,end);

	return true;
};
*/
/*
int AN8XPARSER::CountChar(std::string* source, std::string theString)
{
	std::string str = *source;
	int start = 0;
	int nbrString = 0;
	std::string line = str.substr(0,str.length());
	while( (start = (int)line.find(theString,0)) != (int) std::string::npos )
	{
		line = line.substr(start+1,str.length());
		++nbrString;
	}
	return nbrString;
}
*/

int AN8XPARSER::CountChar(std::string theString)
{
	/*
	int start = 0;
	int nbrString = 0;
	std::string line = Line.substr(0,Line.length());
	while( (start = (int)line.find(theString,0)) != (int)std::string::npos )
	{
		line = line.substr(start+1,Line.length());
		++nbrString;
	}
	return nbrString;
	*/

	std::string::size_type theString_pos = 0;
	int nbrString = 0;

	while ( theString_pos!=std::string::npos )
	{
		theString_pos = Line.find(theString, theString_pos );
		if ( theString_pos != std::string::npos )
		{
			++nbrString;
			theString_pos += theString.length(); // start next search after this "and"
		}
	}
	return nbrString;


};
int AN8XPARSER::Length(void)
{
	return (int)Line.length();
}

int AN8XPARSER::CaptureNumber(std::string* outString)
{
	int startPos = -1;
	for( int i = 0 ; i < (int)Line.length() ; ++i )
	{
		int character = (int) Line[i];
		if( (character >= '0' && character <= '9') || character=='-' || character=='.' )
		{
			startPos = i;
			break;
		}
	}
	if( startPos == -1 )
		return -1;

	int endPos = startPos;
	for( endPos = startPos ; endPos < (int)Line.length() ; ++endPos )
	{
		int character = (int) Line[endPos];
		if( (character >= '0' && character <= '9') || character=='-' || character=='.' )
		{}
		else
			break;
	}

	*outString = Line.substr(startPos,endPos-startPos);
	return startPos;
}

int AN8XPARSER::StrCaptureNumber(std::string* o_String,std::string* i_String )
{
	int startPos = -1;
	for( int i = 0 ; i < (int)i_String->length() ; ++i )
	{
		int character = (int) i_String->c_str()[i];
		if( (character >= '0' && character <= '9') || character=='-' || character=='.' )
		{
			startPos = i;
			break;
		}
	}
	if( startPos == -1 )
		return -1;

	int endPos = startPos;
	for( endPos = startPos ; endPos < (int)i_String->length() ; ++endPos )
	{
		int character = (int) i_String->c_str()[endPos];
		if( (character >= '0' && character <= '9') || character=='-' || character=='.' )
		{}
		else
			break;
	}

	*o_String = i_String->substr(startPos,endPos-startPos);
	return startPos;
}


void AN8XPARSER::SetLine(std::string inString)
{
	Line = inString;
}

std::string* AN8XFileNameFromPath( std::string* o_Result, std::string* i_String)
{
	if( i_String->find("\\",0) != std::string::npos )
	{
		int last = (int)i_String->find_last_of("\\");
		*o_Result = i_String->substr(last+1,i_String->length()-last-1);
	}
	else
	{
		*o_Result = *i_String;
	}
	return o_Result;
}
