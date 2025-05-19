#include <iostream>
#include <map>
#include <fstream>

using namespace std;

const int MaksIlGodz=8;

struct Lekcja /// struktura lekcji w planie
{
    string nauczyciel="";
    string przedmiot="";
};

struct ListaNauiPrzed /// lista nauczycieli
{
    string ImiNaz="";
    string przedmiot="";
    int Il=0;
    ListaNauiPrzed *nast=NULL;
};

int ilklas=0; /// ilosc klas
map <string,map<string,map<string,int>>> IlLekwKl={}; /// mapa przedstawiajaca ilosc godzin danego nauczyciela danego przedmiotu w danej klasie
ListaNauiPrzed *pierwszy=NULL;

void OdczytZpliku() // CO Z NAZWAMI SKLADAJACYMY SIE Z 2 WYRAZOW np: WYCHOWANIE FIZYCZNE
{
    ifstream plik;
    plik.open("dane.txt");

    if(plik.good()) /// jezeli plik sie otworzyl
    {
        ListaNauiPrzed *Info=NULL;
        string dane;
        while(plik>>dane) /// wczytuj dopoki sa dane
        {
            string klasa="";
            if(dane.size()==2 && (dane[0]>='1' && dane[0]<='9') && ((dane[1]>='A' && dane[1]<='Z') || (dane[1]>='a' && dane[1]<='z'))) /// jezeli znazleziono klase
            {
                cout<<dane<<endl;
                klasa=dane;
                ilklas++;
            }
            else /// jezeli sa dane jakiego nauczyciela
            {
                if(Info==NULL) /// wprowadzam pierwszego
                {
                    Info=new ListaNauiPrzed;
                    pierwszy=Info;
                    Info->ImiNaz=dane;
                    string naz;
                    plik>>naz;
                    Info->ImiNaz=Info->ImiNaz+" "+naz;
                    plik>>Info->przedmiot;
                    plik>>Info->Il;
                }
                else /// wprowadzam kolejnego
                {
                    Info->nast=new ListaNauiPrzed;
                    Info=Info->nast;
                    Info->ImiNaz=dane;
                    string naz;
                    plik>>naz;
                    Info->ImiNaz=Info->ImiNaz+" "+naz;
                    plik>>Info->przedmiot;
                    plik>>Info->Il;
                }
                IlLekwKl[klasa][Info->ImiNaz][Info->przedmiot]++; /// ilosc godzin danego nauczyciela danego przedmiotu w danej klasie
            }
        }
    }
    else cout<<"Blad otwierania pliku"<<endl;

    plik.close();
}

void WypiszListeNaucz()
{
    ListaNauiPrzed *rob=pierwszy;
    while (rob!=NULL)
    {
        cout<<rob->ImiNaz<<" "<<rob->przedmiot<<" "<<rob->Il;
        rob=rob->nast;
        cout<<endl;
    }

}

int main(){
    OdczytZpliku();
    WypiszListeNaucz();

    return 0;
}