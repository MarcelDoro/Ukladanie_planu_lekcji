#include <iostream>

using namespace std;

struct Nauczyciel
{
    string im="";
    string nazw="";
    string przedmiot="";
    int ilpozgodz=0;
    Nauczyciel *nast=NULL;
};

struct Lekcja
{
    string nauczyciel="";
    string przedmiot="";
};



int main(){
    cout<<"HI"<<endl;

    return 0;
}