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

	boost::regex WRONG_NAMES("[^a-zа-яё0-9._-]", boost::regex::icase);
	boost::regex WRONG_ENDINGS("(ост(ь)?)", boost::regex::icase);

void stemRussianWord(std::string& word){

	std::wstring wStem;

	wchar_t* UnicodeTextBuffer = new wchar_t[word.length()+1];             //прогнать через стеммер
    std::wmemset(UnicodeTextBuffer, 0, word.length()+1);
    std::mbstowcs(UnicodeTextBuffer, word.c_str(), word.length());
    wStem = UnicodeTextBuffer;		
    stemRussian(wStem);

	word = word.substr(0, wStem.length());

	delete[] UnicodeTextBuffer; 

}

const char* wordsExceptions[] = {"канал", "экран"};



int main(int argc, char **argv) {
//----------------------Definitions------------------------------------------------------------
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	if(argc > 1) filename = argv[1];
	else { 
	//------------------------------------------------MENU MODULE---------------------------------------------------
        std::cout << "\nВведите имя файла входного текста (например \"name.txt\"),\n's' - пропустить или 'q' - выход:\n";	
	 
	    while(1){		
			std::cin >> filename;		
			if(!boost::regex_search(filename, WRONG_NAMES)) break;		
			else { system("cls"); std::cout << "\nИмя файла должно состоять только из строчных или заглавных букв или цифр.\n Пожалуйста введите заново.\n"; }
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

			for(std::string::iterator it = strTemp.begin();  it != strTemp.end(); ++it) if(*it == 'ё') *it = 'е';   // Заменяем ё на е

			for(size_t i=0; i<2; ++i)                                                                               // Не трогаем слова из массива исключений
				if(strTemp.c_str() == wordsExceptions[i]) continue;      
				
	        if(strTemp[0] == '(' && strTemp.back() == ')') {       // Обработка (кавычек)
				strTemp = strTemp.substr(1);
			    strTemp = strTemp.substr(0, len-2);			    
				stemRussianWord(strTemp); 
				if(i != vSplit.size()-1) fout << "(" << strTemp << ") ";
				else fout << "(" << strTemp << ")\n"; 	continue;
			}

			if(strTemp.at(0) != '(' && strTemp.back() == ')') {       // Обработка кавычек)
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
			
			stemRussianWord(strTemp);      //Прогон через стеммер    	

			if(i != vSplit.size()-1) fout << strTemp << " ";
			else fout << strTemp << "\n";			

	    }						     

    } //while

	fin.close(); fout.close();
	system("pause"); return 0;
}