#include <iostream>
#include <map>
#include <fstream>
#include <ctime>
#include <vector>
#include <iomanip>

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
ListaNauiPrzed *pierwszy=NULL; /// wskaźnik do pierwszego nauczyciela
vector <string> Klasy;

void OdczytZpliku(bool p=true) // CO Z NAZWAMI SKLADAJACYMY SIE Z 2 WYRAZOW np: WYCHOWANIE FIZYCZNE
{
    ifstream plik;
    plik.open("dane.txt");

    if(plik.good()) /// jezeli plik sie otworzyl
    {
        ListaNauiPrzed *Info=NULL;
        string dane;
        while(plik>>dane) /// wczytuj dopoki sa dane
        {
            if(dane.size()==2 && (dane[0]>='1' && dane[0]<='9') && ((dane[1]>='A' && dane[1]<='Z') || (dane[1]>='a' && dane[1]<='z'))) /// jezeli znazleziono klase
            {
                if(p==true)
                {
                    Klasy.push_back(dane); /// dodaj na koncu wektora nowa klase
                    ilklas++;
                }
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

string DniTyg[5]={"Poniedziałek","Wtorek","Środa","Czwartek","Piątek"};

map <string,map<string,map<int,Lekcja>>> StworzPlan() /// funckja tworzaca plan lekcji
{
    map <string,map<string,map<int,Lekcja>>> Plan={}; /// mapa przedstawiajaca plan lekcji [klasa][dzien][numer lekcji]
    ListaNauiPrzed *rob=pierwszy;

    while (rob!=NULL)
    {
        while (rob->Il>0)
        {
            int nrkl=rand()%ilklas; /// losuj klasę
            int nrdnia=rand()%5; /// losuj dzień tygodnia
            int nrlek=rand()%MaksIlGodz; /// losuj numer lekcji
            while(Plan[Klasy[nrkl]][DniTyg[nrdnia]][nrlek].nauczyciel!="") /// dopoki na wylosowanym miejscu ktoś sie znajduje to losuj dalej
            {
                nrkl=rand()%ilklas;
                nrdnia=rand()%5;
                nrlek=rand()%MaksIlGodz;
            }

            Plan[Klasy[nrkl]][DniTyg[nrdnia]][nrlek].nauczyciel=rob->ImiNaz; /// na puste miejsce wstaw im i naz wylosowanego nauczyciela
            Plan[Klasy[nrkl]][DniTyg[nrdnia]][nrlek].przedmiot=rob->przedmiot; /// na puste miejsce wstaw przedmiot wylosowanego nauczyciela
            rob->Il=rob->Il-1;
        }
        rob=rob->nast;
    }
    

    OdczytZpliku(false); /// przywróci nauczycielem ich liczbę godzin ponieważ po tej funkcji rob->Il=0 dla kazdego nauczyciela ponieważ robimy rob->Il--;
    return Plan;
}

void WypiszPlan(map <string,map<string,map<int,Lekcja>>> Plan)
{
    for(int i=0;i<ilklas;i++)
    {
        cout<<"====================================================Oto plan klasy "<<Klasy[i]<<"===================================================="<<endl;
        cout<<"   ";
        for(int j=0;j<5;j++) cout<<left<<setw(22)<<DniTyg[j];
        cout<<endl;
        for(int j=0;j<MaksIlGodz;j++)
        {
            cout<<j<<". ";
            for(int k=0;k<5;k++)
            {
                if(Plan[Klasy[i]][DniTyg[k]][j].nauczyciel=="") cout<<left<<setw(22)<<"Brak";
                else cout<<left<<setw(22)<<Plan[Klasy[i]][DniTyg[k]][j].nauczyciel;
            }
            cout<<endl;
            cout<<"    ";
            for(int k=0;k<5;k++)
            {
                if(Plan[Klasy[i]][DniTyg[k]][j].przedmiot=="") cout<<left<<setw(22)<<"brak";
                else cout<<left<<setw(22)<<Plan[Klasy[i]][DniTyg[k]][j].przedmiot;
            }
            cout<<endl;
        }
    }
}

int main(){
    srand(time(NULL)); /// umożliwia generowanie liczb losowych

    OdczytZpliku();
    WypiszListeNaucz();
    map <string,map<string,map<int,Lekcja>>> Plan;
    Plan=StworzPlan();
    cout<<ilklas<<endl;
    WypiszPlan(Plan);

    return 0;
}