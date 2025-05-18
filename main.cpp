#include <iostream>
#include <map>

using namespace std;

struct Lekcja
{
    string nauczyciel="";
    string przedmiot="";
};

class PlanLekcji
{
private:
    map<string, map<string, map<int,Lekcja>>> Plan;
public:
    PlanLekcji(); /// konstruktor
    ~PlanLekcji(); /// destruktor
};

int main(){
    cout<<"HI"<<endl;

    return 0;
}