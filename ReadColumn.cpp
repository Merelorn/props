//by wondrash
//int ReadColumn (string * output, string line, int column){}			//vybere column-ty stlpec do output a vrati jeho dlzku, delimiter je medzera
//int ReadColumn (string * output, string line, int column, char delimiter){}	//vybere column-ty stlpec do output a vrati jeho dlzku, delimiter je zvoleny
//int ReadColumn (char * output, char line, int column){}			//
//int ReadColumn (char * output, char line, int column, char delimiter){}	//
#include<string>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<vector>
#include<stdlib.h>


//int ReadColumn (string * output, string line, int column, char delimiter){}

namespace props{

std::string ReadColumn (const std::string & line, int column, std::string delim){
	std::vector<int> ColumnStart, ColumnEnd;
	bool reading = false;
	
	for ( int j = 0 ; j < line.size() ; j++){
			if      ( (delim.compare(line.substr(j,1)) != 0)    && ( !reading   ))
	                {
	                    reading = true;
	                    ColumnStart.push_back(j);
	                }
	                else if ( (delim.compare(line.substr(j,1)) == 0)     && ( reading    ))
	                {
	                    reading = false;
	                    ColumnEnd.push_back(j);
	                }
	}		
	if ( ColumnEnd.size() != ColumnStart.size() ){ ColumnEnd.push_back(line.size() + 1); } //effectively terminates original string with delim
	if ( column == 0 ){
		std::string answer = "";
		for (int i = 0; i < ColumnStart.size(); i++){ answer = answer + delim;}
		return answer;
	} else {
		column--;
		return line.substr(ColumnStart[column], ColumnEnd[column] - ColumnStart[column]);
	}
}

float ReadColumnP (const std::string & line, int column, std::string delim){
  std::string mybuf = ReadColumn(line, column, delim);
  return strtof(mybuf.c_str(), NULL); 
}

/*
int ReadColumn (char * output, char * line, int column){
int CurrentColumn=0,EndOfColumn=0,j;
int positions[100]={};
bool reading=false;

for (j=0;j<strlen(line);j++){	
		if ((int(*(line+j))!=32)&&(reading==false)){reading=true;CurrentColumn++;positions[CurrentColumn]=j;}
		if ((int(*(line+j))!=32)&&(reading==true)){continue;}
		if ((int(*(line+j))==32)&&(reading==true)){reading=false;if(CurrentColumn==column){EndOfColumn=j;}}
		if ((int(*(line+j))==32)&&(reading==false)){continue;}
}		
strncpy(output,line+positions[column],EndOfColumn-positions[column]);
output[(EndOfColumn-positions[column])]='\0';
return EndOfColumn-positions[column];
}

int ReadColumn (char * output, char * line, int column, char delimiter){
int CurrentColumn=0,EndOfColumn=0,j;
int positions[100]={};
bool reading=false;
int delim=int(delimiter);

for (j=0;j<strlen(line);j++){	
		if ((int(*(line+j))!=delim)&&(reading==false)){reading=true;CurrentColumn++;positions[CurrentColumn]=j;}
		if ((int(*(line+j))!=delim)&&(reading==true)){continue;}
		if ((int(*(line+j))==delim)&&(reading==true)){reading=false;if(CurrentColumn==column){EndOfColumn=j;}}
		if ((int(*(line+j))==delim)&&(reading==false)){continue;}
}		
strncpy(output,line+positions[column],EndOfColumn-positions[column]);
output[(EndOfColumn-positions[column])]='\0';
return EndOfColumn-positions[column];
}
*/

}
