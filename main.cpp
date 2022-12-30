#include <iostream>
#include <locale.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

void anamenu();
void findwordortag(std::vector<std::string> onlytextpaths);
void findandreplace(std::vector<std::string> onlytextpaths);
void replace_all(std::string& s,std::string const& toReplace,std::string const& replaceWith);
void setbastir(std::set<std::string> printarray);
void bastir( std::vector<std::string> printarray);
void recFileList(char *basePath, std::vector<std::string> &allpaths, std::vector<std::string> &filenames);
void openfile(std::vector<std::string> allpaths, std::vector<std::string> &contentoftexts);
void findsquarebrackets(std::vector<std::string> contentoftexts,  std::vector<std::string> &tagwords);
void yetimorregulartag(std::vector<std::string> tagwords,  std::vector<std::string> filenames);
void createfolder();


std::set<std::string> nonrepetittags;       //tekrar etmeyen etiketlerin hepsi
std::set<std::string> yetimetiket;          //yetim etiket set
std::vector<std::string> yetimetiketvec;    //yetim etiket vektor
std::set<std::string> etiket;               //etiket set
std::vector<std::string> etiketvec;         //etiket vektoru
std::set<std::string> istenenetiket;        //istenen etiket set
std::vector<std::string> istenenetiketvec;  //istenen etiket vektor
std::vector<std::string> allpaths;          // klasorun icindeki tum pathler
std::vector<std::string> onlytextpaths;     // sadece textlerin pathleri
std::vector<std::string> contentoftexts;    //tum textlerin icerikleri
std::vector<std::string> tagwords;          //taglerin ici parantez yok
std::vector<std::string> filenames;         //.txt olan dosya isimleri

int main()
{
    setlocale(LC_ALL,"tr_TR.UTF-8");

    char basePath[100] = "C:\\Users\\frknk\\Desktop\\Üniversite";

    recFileList(basePath,allpaths,filenames);

    int arrsize = allpaths.size();
    string istext = ".txt";
    for (int i = 0; i < arrsize; i++){
        if(strstr(allpaths[i].c_str(),istext.c_str()))
            onlytextpaths.push_back(allpaths[i]);
    }

    openfile(onlytextpaths,contentoftexts);
    findsquarebrackets(contentoftexts,tagwords);
    yetimorregulartag(tagwords,filenames);

    anamenu();


}

void recFileList(char *basePath, std::vector<std::string> &allpaths, std::vector<std::string> &filenames)
{
    char path[200];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            //cout << dp->d_name << endl;
            if(strstr(dp->d_name,".txt"))
            filenames.push_back(dp->d_name);

            strcpy(path, basePath);
            strcat(path, "\\");
            strcat(path, dp->d_name);

            allpaths.push_back(path);

            recFileList(path,allpaths,filenames);
        }
    }
    closedir(dir);

}

void openfile(std::vector<std::string> allpaths, std::vector<std::string> &contentoftexts){
    string mytext;
    ifstream myreadfile;

    for(int i = 0 ; i < allpaths.size() ; i++){
    myreadfile.open(allpaths[i]);

    while(getline(myreadfile,mytext)){
        contentoftexts.push_back(mytext);
    }
    myreadfile.close();
    }
}

void findsquarebrackets(std::vector<std::string> contentoftexts,  std::vector<std::string> &tagwords)
{
    for(string text : contentoftexts)
    {
    for (auto i = text.find("[["); i != string::npos; i = text.find("[[", i))
        {
            i += 2;                         // "[[" == 2
            if (text[i] == '[') continue;   // "[[["
            auto x = text.find("]", i);

            if (x != string::npos && text[x + 1] == ']'){
            if (text[x+2] == ']') continue;
            string temptext = text.substr(i, x - i);
            int space = 0;
            for(int a = 1 ; a < temptext.length()-1 ; a++){
                if(temptext[a] == ' ')
                space++;
            }
            if(space < 2){
            tagwords.push_back(text.substr(i, x - i));
            nonrepetittags.insert(text.substr(i, x - i));
            }
            }
        }
    }
}

void yetimorregulartag(std::vector<std::string> tagwords,  std::vector<std::string> filenames)
{
    for(string nonrepetittagsiter : tagwords)    //yetim veya normal etiket arıyor
    {
        bool found = false;

        for(string filenamesiter: filenames)
        {
        if(nonrepetittagsiter == filenamesiter.substr(0,filenamesiter.size()-4) ){
            found = true;
            break;
        }
        }
        if(found){
             etiket.insert(nonrepetittagsiter);
             etiketvec.push_back(nonrepetittagsiter);
        }
        else{
            yetimetiket.insert(nonrepetittagsiter);
            yetimetiketvec.push_back(nonrepetittagsiter);
        }
    }

    bool found2 = false;
    for(string filenamesiter2: filenames)       // istenen etiket arıyor
    {
        found2 = false;

        for(string nonrepetittagsiter2 : tagwords)
        {
        if( filenamesiter2.substr(0,filenamesiter2.size()-4) == nonrepetittagsiter2  ){
            found2 = true;
            break;
        }

        }
        if(!found2){
             istenenetiket.insert(filenamesiter2.substr(0,filenamesiter2.size()-4));
             istenenetiketvec.push_back(filenamesiter2.substr(0,filenamesiter2.size()-4));
        }
    }
}

void createfolder()
{
    map<string, int> freq;
    for (int i = 0 ; i < tagwords.size() ; i++){
        if(freq.find(tagwords[i]) == freq.end()){
            freq[tagwords[i]] = 1;
        }
        else
            freq[tagwords[i]]++;
    }

    ofstream fout;
    fout.open("C:/Users/frknk/Desktop/output.txt",std::fstream::in | std::fstream::out | std::fstream::app); // append modda acılıs
    if(fout){

            fout << setw(10) << "Etiket Listesi ---" << setw(10) << " Tekrar Sayısı" << endl;
            for (auto& it : freq) {
                    int templen = it.first.length();
            fout << it.first << setw(30-templen) << it.second << '\n';
            }
            fout << endl;
            fout << setw(10) << "Yetim Etiketler" << endl;
            for(string yetetiketit : yetimetiket)
            fout << yetetiketit << endl ;
            fout << endl;

            fout.close();

    }
     string outputstr;
     ifstream fin;
     fin.open("C:/Users/frknk/Desktop/output.txt");
     while(getline(fin,outputstr)){
        cout << outputstr << endl ;
     }
     fin.close();
}

void findwordortag(std::vector<std::string> onlytextpaths)
{
    cout<< "Yetim Etiketler: " << endl;
    for(string yetbas : yetimetiket)
        {
        cout <<yetbas << " -- ";
        }
    cout<< endl << "İstenen Etiketler: "<< endl;
    for(string istenenbas : istenenetiket)
        {
        cout << istenenbas << " -- ";
        }

    string searchstr;
    bool foundflag = false;
    cout << endl << endl << "Aranacak kelimeyi giriniz : ";
    getline(cin,searchstr);

    for(int i = 0 ; i < onlytextpaths.size() ; i++){
    string path = onlytextpaths[i];
    ifstream findFile;
    string line;

    findFile.open(path);
    unsigned int curLine = 0;

    if(!findFile){
    cout << "Dosya acılamadı" << endl;
    exit(1);
    }
    while(getline(findFile,line))
    {
        curLine++;
        if (line.find(searchstr, 0) != string::npos) {
        foundflag = true;
        cout << endl << "Bulunan kelime : " << searchstr << "       Satir : " << curLine << endl;
        cout << "Path: " << path <<  endl;
        }

    }

    findFile.close();
    }
    if(!foundflag){
       cout << endl <<"Kelime text dosyalarinin icerisinde bulunamadi" << endl;
    }
}



void findandreplace(std::vector<std::string> onlytextpaths)
{
    string searchstr;
    string changestr;
    bool foundflag = false;
    cout << "Degistirilecek etiket adini giriniz : ";
    getline(cin,searchstr);
    cout << "Yeni etiket : ";
    getline(cin,changestr);

    for(int i = 0 ; i < onlytextpaths.size() ; i++){
    string path = onlytextpaths[i];

    ofstream tempfile;
    char pathtemp[150] = "temp.txt";
    ifstream inFile;

    string line;

    inFile.open(path);
    unsigned int curLine = 0;

    if(!inFile){
    cout << "Dosya acılamadı" << endl;
    exit(1);
    }
    tempfile.open(pathtemp);
    if(!tempfile){
    cout << "Dosya acılamadı" << endl;
    exit(1);
    }


    while(getline(inFile,line))
    {
        if (line.find(searchstr, 0) != string::npos) {
        foundflag = true;
        replace_all(line,searchstr,changestr);
        }
        tempfile << line << "\n";

    }

    tempfile.close();
    inFile.close();


    remove(path.c_str());
    rename(pathtemp,path.c_str());
    bool flagbulundu = false;
    for (string a : etiket ){
            if (a == searchstr)
                flagbulundu = true;

    }
    if (path.find(searchstr) != std::string::npos && flagbulundu){
            std::size_t found = path.find_last_of("/\\");
            string onpath = path.substr(0,found);
            string lastpath = onpath+ "/" + changestr +".txt";
            rename(path.c_str(),lastpath.c_str());
    }

    }
    if(foundflag){
        cout << endl <<"Kelime degistirildi" << endl;
    }
    else{
        cout << endl <<"Kelime text dosyalarinin icerisinde bulunamadi" << endl;
    }

}

void replace_all(
    std::string& tempstr,            //line
    std::string const& toReplace,    //searchstr
    std::string const& replaceWith)  //changestr
 {
    std::string bufstr;
    std::size_t pos = 0;
    std::size_t prevPos;

    // stringin son boyutunu tahminini saklar
    bufstr.reserve(tempstr.size());

    while (true) {
        prevPos = pos;
        pos = tempstr.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        bufstr.append(tempstr, prevPos, pos - prevPos);
        bufstr += replaceWith;
        pos += toReplace.size();
    }

    bufstr.append(tempstr, prevPos, tempstr.size() - prevPos);
    tempstr.swap(bufstr);
}

void anamenu()
{
    string choice;
        cout<< "------------ Dokuwiki Dosya ------------"<<endl<<endl;   //menü
        cout<< " 1-Arama"<<endl<<endl;
        cout<< " 2-Güncelleme"<<endl<<endl;
        cout<< " 3-Dosyaya Yazma"<<endl<<endl;
        cout<< " 0-Çıkış"<<endl<<endl;
        cout<< "Seçim: ";

   while(true){
        getline(cin,choice);

        if(choice=="0"){
            cout << "Çıkış yapılıyor";
            exit(0);
            }
        else if(choice=="1"){
            system("cls");
            cout<< "------------------------------------------------------------------------------------"<<endl;
            cout<< "---------- Menüye geri dönmek için 4'e - çıkış yapmak için 0'a tıklayınız ----------"<<endl;
            cout<< "------------------------------------------------------------------------------------"<<endl<<endl<<endl;
            findwordortag(onlytextpaths);
            }
        else if(choice=="2"){
            system("cls");
            cout<< "------------------------------------------------------------------------------------"<<endl;
            cout<< "---------- Menüye geri dönmek için 4'e - çıkış yapmak için 0'a tıklayınız ----------"<<endl;
            cout<< "------------------------------------------------------------------------------------"<<endl<<endl<<endl;
            findandreplace(onlytextpaths);
            }
        else if(choice=="3"){
            system("cls");
            cout<< "------------------------------------------------------------------------------------"<<endl;
            cout<< "---------- Menüye geri dönmek için 4'e - çıkış yapmak için 0'a tıklayınız ----------"<<endl;
            cout<< "------------------------------------------------------------------------------------"<<endl<<endl<<endl;
            createfolder();
            }
        else if(choice=="4"){
            system("cls");
            cout<< "------------ Dokuwiki Dosya ------------"<<endl<<endl;   //menü
            cout<< " 1-Arama"<<endl<<endl;
            cout<< " 2-Güncelleme"<<endl<<endl;
            cout<< " 3-Dosyaya Yazma"<<endl<<endl;
            cout<< " 0-Çıkış"<<endl<<endl;
            cout<< "Seçim: ";
            }
        else
             cout<< "Yanlış seçim yaptınız(0-4) ";

        }
    }


void bastir(std::vector<std::string> printarray){
    int i = 1;
    for(string bas : printarray)
        {
        cout << i <<". " <<bas << endl;
        i++;
        }
}
void setbastir(std::set<std::string> printarray){
    int i = 1;
    for(string bas : printarray)
        {
        cout << i <<". " <<bas << endl;
        i++;
        }
}
