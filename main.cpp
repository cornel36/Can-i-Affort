#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
#include <fstream>

using namespace std;

const string NazwaPliku = "Goals.txt";

class CelOszczednosciowy
{
public:
    int numerCelu;
    string nazwa;
    double kwotaDocelowa;
    double aktualnaKwota;
    bool zrealizowany;

    CelOszczednosciowy(int numer, const string &n, double kwota) : numerCelu(numer), nazwa(n), kwotaDocelowa(kwota), aktualnaKwota(0.0), zrealizowany(false) {}
};

vector<CelOszczednosciowy> celeOszczednosciowe;

void zapiszdoPliku(){
    ofstream plik(NazwaPliku);


    if(plik.is_open()){
        for(const auto &cel : celeOszczednosciowe){
            plik << cel.numerCelu << " " << cel.nazwa << " " << cel.kwotaDocelowa << " " << cel.aktualnaKwota << " " << cel.zrealizowany << endl; 
        }
    plik.close();
    cout << "Cele zapisane!" << endl;
    }
    else
    {
        cout << "Nie można otworzyć pliku zapisu." << endl;
    } 
}

void wczytajPlik(){

    celeOszczednosciowe.clear();

    ifstream plik(NazwaPliku);

    if(plik.is_open()){
        int numerCelu;
        string nazwa;
        double kwotaDocelowa;
        double aktualnaKwota;
        bool zrealizowany;

        while(plik >> numerCelu >> nazwa >> kwotaDocelowa >> aktualnaKwota >> zrealizowany){
            celeOszczednosciowe.push_back(CelOszczednosciowy(numerCelu,nazwa,kwotaDocelowa));
            celeOszczednosciowe.back().aktualnaKwota = aktualnaKwota;
            celeOszczednosciowe.back(). zrealizowany = zrealizowany;
        }
        plik.close();
        cout << "Cele wczytane z zapisu!" << endl;
    }
    else
    {
        cout << "Nie można wczytać ostatniego zapisu. Tworzenie nowego pliku." << endl;
        ofstream nowyPLIK(NazwaPliku);
        nowyPLIK.close();
    }
}


void dodajCel()
{
    string nazwa;
    double kwotaDocelowa;

    cout << "Podaj nazwę celu: " << endl;
    cin >> nazwa;

    cout << "Podaj kwotę docelową: " << endl;
    cin >> kwotaDocelowa;

    int numerCelu = celeOszczednosciowe.size() + 1;

    celeOszczednosciowe.push_back(CelOszczednosciowy(numerCelu, nazwa, kwotaDocelowa));

    cout << "Dodano nowy cel!" << endl;
}

void wyswietlCele()
{
    cout << "Twoje cele: " << endl;

    for (const auto &cel : celeOszczednosciowe)
    {
        cout << "Numer: " << cel.numerCelu << ", Cel: " << cel.nazwa << ", Kwota docelowa: " << cel.kwotaDocelowa
             << ", Aktualna kwota: " << cel.aktualnaKwota << ", Status: " << (cel.zrealizowany ? "Zrealizowany" : "Niezrealizowany") << endl;
    }
}

void usunCel()
{
    if (celeOszczednosciowe.empty())
    {
        cout << endl
             << "Brak celów." << endl;
        return;
    }

    int numerCelu;

    wyswietlCele();

    cout << "Podaj numer celu do usunięcia: " << endl;
    cin >> numerCelu;

    if (numerCelu >= 1 && numerCelu <= celeOszczednosciowe.size())
    {
        celeOszczednosciowe.erase(celeOszczednosciowe.begin() + numerCelu - 1);
        cout << "Cel został usunięty!" << endl;
    }
    else
    {
        cout << "Nieprawidłowy numer celu." << endl;
    }
}

void aktualizujCel()
{
    if (celeOszczednosciowe.empty())
    {
        cout << endl
             << "Brak celów do aktualizacji." << endl;
        return;
    }

    int numerCelu;
    double nowaKwota;

    wyswietlCele();

    cout << "Podaj numer celu do aktualizacji: " << endl;
    cin >> numerCelu;

    if (numerCelu >= 1 && numerCelu <= celeOszczednosciowe.size())
    {
        cout << "Podaj nową kwotę dla celu " << numerCelu << ": ";
        cin >> nowaKwota;

        celeOszczednosciowe[numerCelu - 1].aktualnaKwota = nowaKwota;

        if (celeOszczednosciowe[numerCelu - 1].aktualnaKwota >= celeOszczednosciowe[numerCelu - 1].kwotaDocelowa)
        {
            celeOszczednosciowe[numerCelu - 1].zrealizowany = true;
            cout << "Gratulacje! Cel został osiągnięty!" << endl;
        }
        else
        {
            cout << "Aktualizacja zakończona pomyślnie!" << endl;
        }
    }
    else
    {
        cout << "Nieprawidłowy numer celu." << endl;
    }
}

bool ZgodaPowiadomienie()
{
    char odpowiedz;

    cout << "Czy zgadzasz sie na codzienne powiadomienia? (T/N): ";
    cin >> odpowiedz;

    return (odpowiedz == 'T' || odpowiedz == 't');
}

void zmienZgodePowiadomienia(bool &zgoda)
{
    zgoda = !zgoda;
    cout << "Zgoda na powiadomienia została " << (zgoda ? "udzielona." : "wycofana.") << endl;
}

void wyslijPowiadomienie(const CelOszczednosciowy &cel) {
    wstring tresc = L"Przypomnienie o celu: " + wstring(cel.nazwa.begin(), cel.nazwa.end()) +
                    L"Kwota docelowa: " + to_wstring(cel.kwotaDocelowa) +
                    L"\nAktualna kwota: " + to_wstring(cel.aktualnaKwota);

    MessageBox(NULL, tresc.c_str(), L"Przypomnienie o celu", MB_ICONINFORMATION | MB_OK);
}

int main()
{
    wczytajPlik();

    bool zgodaNaPowiadomienia = ZgodaPowiadomienie();
    
    int wybor;


    do
    {
        cout << "Menu: " << endl;
        cout << "1. Dodaj cel oszczędnościowy" << endl;
        cout << "2. Wyświetl listę celów oszczędnościowych" << endl;
        cout << "3. Usuń cel oszczędnościowy" << endl;
        cout << "4. Aktualizuj cel oszczędnościowy" << endl;
        cout << "5. Zmień zgodę dotyczącą powiadomień" << endl;
        cout << "0. Wyjście" << endl;

        cout << "Wybierz opcję:" << endl;
        cin >> wybor;

        switch (wybor)
        {
        case 1:
            dodajCel();
            break;
        case 2:
            wyswietlCele();
            break;
        case 3:
            usunCel();
            break;
        case 4:
            aktualizujCel();
            break;
        case 5:
            zmienZgodePowiadomienia(zgodaNaPowiadomienia);
            break;
        case 0:
            cout << "Zapisywanie celów przed wyjściem." << endl;
            zapiszdoPliku();
            cout << "Do zobaczenia!" << endl;
            break;
        default:
            cout << "Nieprawidłowy wybór." << endl;
        }
        if(zgodaNaPowiadomienia){
            wyslijPowiadomienie();
        }

    } while (wybor != 0);

    return 0;
}
