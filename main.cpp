#include <iostream>
#include <vector>

using namespace std;

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

int main()
{
    int wybor;

    do
    {
        cout << "Menu: " << endl;
        cout << "1. Dodaj cel oszczędnościowy" << endl;
        cout << "2. Wyświetl listę celów oszczędnościowych" << endl;
        cout << "3. Usuń cel oszczędnościowy" << endl;
        cout << "4. Aktualizuj cel oszczędnościowy" << endl;
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
        case 0:
            cout << "Do zobaczenia!" << endl;
            break;
        default:
            cout << "Nieprawidłowy wybór." << endl;
        }

    } while (wybor != 0);

    return 0;
}
