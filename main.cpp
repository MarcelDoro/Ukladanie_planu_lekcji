#include <iostream>
#include <map>
#include <fstream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <set>

using namespace std;

const int MaksIlGodz=8;

struct Lekcja /// struktura lekcji w planie
{
    string nauczyciel="";
    string przedmiot="";
};

struct ListaNauiPrzed /// lista nauczycieli i przedmiotow uczacych
{
    string ImiNaz="";
    string przedmiot="";
    int Il=0;
    ListaNauiPrzed *nast=NULL;
};

int ilklas=0; /// ilosc klas
ListaNauiPrzed *pierwszy=NULL; /// wskaźnik do pierwszego nauczyciela
vector <string> Klasy;
map <pair<int,string>,int> OczLek; /// mapa oczekiwanych lekcji [klasa][nauczyciel]=ilosc lekcji jaka powinien miec nauczyciel w danej klasie

void OdczytZpliku(bool p=false) // CO Z NAZWAMI SKLADAJACYMY SIE Z 2 WYRAZOW np: WYCHOWANIE FIZYCZNE
{
    ifstream plik;
    plik.open("dane.txt");

    if(plik.good()) /// jezeli plik sie otworzyl
    {
        ListaNauiPrzed *Info=NULL;
        string dane;
        int akklasa=-1;
        while(plik>>dane) /// wczytuj dopoki sa dane
        {
            if(dane.size()==2 && (dane[0]>='1' && dane[0]<='9') && ((dane[1]>='A' && dane[1]<='Z') || (dane[1]>='a' && dane[1]<='z'))) /// jezeli znazleziono klase
            {
                if(p==true)
                {
                    akklasa++;
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
                if(p==true) /// jezeli wczytujemy nauczycieli po raz pierwszy
                {
                    OczLek[make_pair(akklasa,Info->ImiNaz)]=Info->Il; /// dodajemy ilosc godzin nauczyciela do mapy oczekiwanych lekcji w klasie
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


    OdczytZpliku(); /// przywróci nauczycielem ich liczbę godzin ponieważ po tej funkcji rob->Il=0 dla kazdego nauczyciela ponieważ robimy rob->Il--;
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

int ObliczFitness(map <string,map<string,map<int,Lekcja>>> Plan) /// funkcja obliczająca fitness planu
{
    int fitness=0; /// 0 - idealny plan

    // mozliwe ze bedzie nalezalo to przebudowac aby liczyly kazda przerwe
    /// po pierwsze liczyby okienka uczniów
    for(int i=0;i<ilklas;i++)
    {
        for(int j=0;j<5;j++)
        {
            int poc=-1; /// poczatkowa lekcja danego dnia
            int ost=-1; /// ostatnia lekcja danego dnia
            for(int k=0;k<MaksIlGodz;k++)
            {
                if(Plan[Klasy[i]][DniTyg[j]][k].nauczyciel!="") /// jezeli jest lekcja
                {
                    if(poc==-1) poc=k; /// jezeli nie ma pierwszej lekcji to ja dodaj
                    ost=k; /// jezeli nie ma ostatniej lekcji to ja dodaj
                }
            }
            if(poc!=-1 && ost!=-1 && ost>poc) /// jezeli sa lekcje oraz cos jest miedzy nimi
            {
                for(int k=poc+1;k<ost;k++) /// do ost, poniewaz okienka nie licza sie po ostatniej lekcji
                {
                    if(Plan[Klasy[i]][DniTyg[j]][k].nauczyciel=="") /// jezeli jest przerwa
                    {
                        fitness=fitness+10; /// dodajemy 10 do fitnessu
                        break; /// nie liczyby wiecej przerw
                    }
                }
            }
        }
    }

    /// tworzymy zbior unikalnych nauczycieli
    set<string> unauczyciele; /// zbior unikalnych nauczycieli
    ListaNauiPrzed *rob=pierwszy; /// zaczynamy od pierwszego nauczyciela
    while(rob!=NULL) /// dopoki nie doszlismy do konca listy nauczycieli
    {
        unauczyciele.insert(rob->ImiNaz); /// dodajemy nauczyciela do zbioru
        rob=rob->nast; /// przechodzimy do nastepnego nauczyciela
    }

    /// sprawdzamy czy nauczyciel nie ma w tym samym czasie wiecej niz 1 lekcji

    for(string n: unauczyciele) /// przechodzimy po wszystkich unikalnych nauczycielach
    {
        for(int j=0;j<5;j++)
        {
            for(int k=0;k<MaksIlGodz;k++)
            {
                int ilg=0;
                for(int i=0;i<ilklas;i++) if(Plan[Klasy[i]][DniTyg[j]][k].nauczyciel==n) ilg++; /// zliczamy ilosc lekcji nauczyciela w tym samym czasie
                if(ilg>1) fitness=fitness+10; /// jezeli nauczyciel ma wiecej niz 1 lekcje w tym samym czasie
            }
        }
    }

    /// sprawdzamy czy ilosc godzin nauczycieli w poszczegolnych klasach sie zgadza
    rob=pierwszy; /// zaczynamy od pierwszego nauczyciela
    while(rob!=NULL) /// dopoki nie doszlismy do konca listy nauczycieli
    {
        for(int i=0;i<ilklas;i++)
        {
            int ilg=0; /// ilosc godzin nauczyciela w danej klasie
            for(int j=0;j<5;j++)
            {
                for(int k=0;k<MaksIlGodz;k++)
                {
                    if(Plan[Klasy[i]][DniTyg[j]][k].nauczyciel==rob->ImiNaz) ilg++; /// zliczamy ilosc lekcji nauczyciela
                }
            }
            if(OczLek[make_pair(i,rob->ImiNaz)]!=ilg) fitness=fitness+10; /// jezeli ilosc lekcji nauczyciela w klasie nie zgadza sie z jego iloscia planowanych godzin
        }
        rob=rob->nast; /// przechodzimy do nastepnego nauczyciela
    }


    /// teraz liczy okienka nauczycieli

    for(string n: unauczyciele)
    {
        for(int j=0;j<5;j++)
        {
            int ostlek=-1; /// ostatnia lekcja danego dnia
            int poclek=-1; /// pierwsza lekcja danego dnia
            for(int k=0;k<MaksIlGodz;k++)
            {
                for(int i=0;i<ilklas;i++)
                {
                    if(Plan[Klasy[i]][DniTyg[j]][k].nauczyciel==n)
                    {
                        if(poclek==-1) poclek=k; /// jezeli nauczyciel ma lekcje danego dnia i jest to jego pierwsza
                        ostlek=k; /// jezeli nauczyciel ma lekcje danego dnia
                    }
                }
            }

            if(ostlek!=-1 && poclek!=-1 && ostlek>poclek) /// jezeli sa lekcje oraz cos jest miedzy nimi
            {
                for(int l=poclek+1;l<ostlek;l++) /// do ost, poniewaz okienka nie licza sie po ostatniej lekcji
                {
                    bool ok=true;
                    for(int i=0;i<ilklas;i++) /// przechodzi po wszystkich klasach
                    {
                        if(Plan[Klasy[i]][DniTyg[j]][l].nauczyciel==n)
                        {
                            ok=false; /// jezeli jest lekcja to nie ma okienka
                            break; /// nie musi dalej przeszukiwac bo wiemy ze jest lekcja
                        }
                    }
                    if(ok==true) fitness++;/// jezeli jest przerwa to dodajemy 1 do fitnessu
                }
            }
        }
    }

    return fitness;
}

void Swap(Lekcja &L1,Lekcja &L2)
{
    Lekcja temp; /// tworzymy zmienną tymczasową
    temp=L1; /// kopiujemy pierwszą lekcje do zmiennej tymczasowej
    L1=L2; /// kopiujemy drugą lekcje do pierwszej
    L2=temp; /// kopiujemy zmienną tymczasową do drugiej lekcji
}

map <string,map<string,map<int,Lekcja>>> ScalPlany(map <string,map<string,map<int,Lekcja>>> P1,map <string,map<string,map<int,Lekcja>>> P2)
{
    map <string,map<string,map<int,Lekcja>>> Plan;
    int p=rand()%(5* MaksIlGodz); /// losujemy punkt podzialu

    for(int i=0;i<ilklas;i++) /// przechodzimy po wszystkich klasach
    {
        for(int j=0;j<5;j++) /// przechodzimy po wszystkich dniach tygodnia
        {
            for(int k=0;k<MaksIlGodz;k++) /// przechodzimy po wszystkich lekcjach
            {
               if(j*5+k<p) /// jezeli jestesmy przed punktem podzialu
               {
                    Plan[Klasy[i]][DniTyg[j]][k].nauczyciel=P1[Klasy[i]][DniTyg[j]][k].nauczyciel; /// kopiujemy nauczyciela z pierwszego planu
                    Plan[Klasy[i]][DniTyg[j]][k].przedmiot=P1[Klasy[i]][DniTyg[j]][k].przedmiot; /// kopiujemy przedmiot z pierwszego planu
               }
               else /// jezeli jestesmy po punkcie podzialu
               {
                    Plan[Klasy[i]][DniTyg[j]][k].nauczyciel=P2[Klasy[i]][DniTyg[j]][k].nauczyciel; /// kopiujemy nauczyciela z drugiego planu
                    Plan[Klasy[i]][DniTyg[j]][k].przedmiot=P2[Klasy[i]][DniTyg[j]][k].przedmiot; /// kopiujemy przedmiot z drugiego planu
               }
            }
        }
    }

    /// teraz robimy losowa mutacje

    int k1=rand()%ilklas; /// losujemy pierwsza klase
    int k2=rand()%ilklas; /// losujemy druga klase
    int m1=rand()%(5* MaksIlGodz); /// losujemy pierwszy punkt mutacji
    int m2=rand()%(5* MaksIlGodz); /// losujemy drugi punkt mutacji
    Swap(Plan[Klasy[k1]][DniTyg[m1/MaksIlGodz]][m1%MaksIlGodz],Plan[Klasy[k2]][DniTyg[m2/MaksIlGodz]][m2%MaksIlGodz]); /// zamieniamy lekcje w planie

    return Plan;
}


int main(){
    srand(time(NULL)); /// umożliwia generowanie liczb losowych
    OdczytZpliku(true);

    map <string,map<string,map<int,Lekcja>>> Plany[10];
    for(int j=0;j<10;j++)
    {
        Plany[j]=StworzPlan();
    }

    int k=0;
    while(ObliczFitness(Plany[0])>5)
    {
        if(k>2000)
        {
            k=0;
            for(int j=0;j<10;j++)
            {
                Plany[j]=StworzPlan();
            }
        }

        map <string,map<string,map<int,Lekcja>>> P;
        P=ScalPlany(Plany[rand()%10],Plany[rand()%10]);
        int fitP=ObliczFitness(P);
        for(int j=0;j<10;j++)
        {
            if(fitP<ObliczFitness(Plany[j]))
            {
                Plany[j]=P;
                break;
            }
        }
        k++;
        if(k%200==0) {cout<<k<<endl; cout<<ObliczFitness(Plany[0])<<endl;}
    }
    cout<<ObliczFitness(Plany[0])<<endl;
    WypiszPlan(Plany[0]);

    return 0;
}
