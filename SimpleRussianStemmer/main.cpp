#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <windows.h>
#include "d:\support\stemlib\stemming\russian_stem.h" 
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

    char c = 'a';
    size_t len = 0;
	std::string filename, strIn, strTemp;
	std::vector<std::string> vSplit;

	stemming::russian_stem<> stemRussian;

	boost::regex WRONG_NAMES("[^a-z�-��0-9._-]", boost::regex::icase);
	boost::regex WRONG_ENDINGS("(���(�)?)", boost::regex::icase);

void stemRussianWord(std::string& word){

	std::wstring wStem;

	wchar_t* UnicodeTextBuffer = new wchar_t[word.length()+1];             //�������� ����� �������
    std::wmemset(UnicodeTextBuffer, 0, word.length()+1);
    std::mbstowcs(UnicodeTextBuffer, word.c_str(), word.length());
    wStem = UnicodeTextBuffer;		
    stemRussian(wStem);

	word = word.substr(0, wStem.length());

	delete[] UnicodeTextBuffer; 
}

const char* wordsExceptions[] = {"�����", "�����"};



int main(int argc, char **argv) {
//----------------------Definitions------------------------------------------------------------
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	if(argc > 1) filename = argv[1];
	else { 
	//------------------------------------------------MENU MODULE---------------------------------------------------
        std::cout << "\n������� ��� ����� �������� ������ (�������� \"name.txt\"),\n's' - ���������� ��� 'q' - �����:\n";	
	 
	    while(1){		
			std::cin >> filename;		
			if(!boost::regex_search(filename, WRONG_NAMES)) break;		
			else { system("cls"); std::cout << "\n��� ����� ������ �������� ������ �� �������� ��� ��������� ���� ��� ����.\n ���������� ������� ������.\n"; }
		}

		if(filename == "s" || filename == "S") strIn = "1.txt";
	    if(filename == "q" || filename == "Q") return 0;	
}

	std::cout << "\nProcessing file: " << filename << "...\n\n"; system("pause");

	std::ifstream fin(filename); if(!fin) { std::cerr << "I/O error.\n\n"; return -1; }
	std::ofstream fout("out.txt"); if(!fout) { std::cerr << "I/O error.\n\n"; return -1; }

//------------------------------------Main Cycle---------------------------------------------------------------------
	while(getline(fin, strIn))	{

		boost::trim(strIn);
	    boost::split(vSplit, strIn, boost::is_any_of(" "), boost::token_compress_on);

  	    for(int i=0; i<vSplit.size(); ++i) {			
			
			strTemp = vSplit[i];
			len = strTemp.length();
			if(len) c = strTemp.at(len-1);			

			//std::cout << "Inside(): " << strTemp << "   len: " << len << "   c: " << c << "    i: " << i << "   vsize: " << vSplit.size() << "\n"; system("pause"); 

			for(std::string::iterator it = strTemp.begin();  it != strTemp.end(); ++it) if(*it == '�') *it = '�';   // �������� � �� �

			for(size_t i=0; i<2; ++i)                                                                               // �� ������� ����� �� ������� ����������
				if(strTemp.c_str() == wordsExceptions[i]) continue;      
				
	        if(strTemp[0] == '(' && strTemp.back() == ')') {       // ��������� (�������)
				strTemp = strTemp.substr(1);
			    strTemp = strTemp.substr(0, len-2);			    
				stemRussianWord(strTemp); 
				if(i != vSplit.size()-1) fout << "(" << strTemp << ") ";
				else fout << "(" << strTemp << ")\n"; 	continue;
			}

			if(strTemp.at(0) != '(' && strTemp.back() == ')') {       // ��������� �������)
			    strTemp = strTemp.substr(0, len-2);
			    stemRussianWord(strTemp); 
				if(i != vSplit.size()-1) fout << strTemp << ") ";
				else fout << strTemp << ")\n";  continue;
			} 


			if(c == ',' || c == '.' || c == '!' || c == '?' || c == ';') {
				strTemp = strTemp.substr(0, len-2);
				stemRussianWord(strTemp);
				if(i != vSplit.size()-1) fout << strTemp << c << " ";
				else fout << strTemp << c << "\n";  continue;
			}			
			
			stemRussianWord(strTemp);      //������ ����� �������    	

			if(i != vSplit.size()-1) fout << strTemp << " ";
			else fout << strTemp << "\n";			

	    }						     

    } //while

	fin.close(); fout.close();
	system("pause"); return 0;
}