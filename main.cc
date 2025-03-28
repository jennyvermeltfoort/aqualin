// Hoofdprogramma voor oplossing voor eerste programmeeropdracht
// Algoritmiek, voorjaar 2025: Aqualin
//
// Biedt de gebruiker een menustructuur om
// * het spel te spelen
//   - vanaf een nieuw, leeg bord
//   - vanaf een in te lezen, en mogelijk (deels) gevuld bord
//   Voor de mogelijkheden bij het spelen van het spel, zie functie
//   doeSpel.
// * experimenten uit te voeren, waarbij
//   - voor een reeks spellen de ene speler volgens grootste cluster,
//     en de andere speler optimaal speelt
//   - voor een reeks spellen de ene speler volgens Monte Carlo
//     en de andere speler optimaal speelt
//   - voor een maximaal spel de ene speler volgens grootste cluster
//     en de andere speler volgens Monte Carlo speelt
//
// Namen studenten, datum

#include <ctime>  // voor clock() en clock_t
#include <iostream>

#include "aqualin.h"
using namespace std;
const int MaxBestandsNaamLengte =
    30;                   // maximale lengte van een bestandsnaam
const int NrTests = 100;  // aantal tests voor zelfde set parameters
                          // bij experiment

//*************************************************************************

// Vraag de gebruikers om waardes in te voeren voor hoogte, breedte,
// vormen, alOpBord, keuzeAantal[.].
void vraagParameters(int &hoogte, int &breedte, int &vormen,
                     int &alOpBord, int keuzeAantal[2]) {
    int vakjes;

    cout << "Geef de hoogte van het bord (1.." << MaxDimensie
         << "): ";
    cin >> hoogte;
    cout << "Geef de breedte van het bord (1.." << MaxDimensie
         << "): ";
    cin >> breedte;
    vakjes = hoogte * breedte;
    cout << "Geef het aantal vormen: (1.." << MaxKleurenVormen
         << "): ";
    cin >> vormen;
    cout << "Geef het aantal stenen dat al op het bord ligt (0.."
         << vakjes << "): ";
    cin >> alOpBord;
    cout << "Geef het aantal stenen waaruit speler 0 kan kiezen (1.."
         << MaxKeuzeAantal << "): ";
    cin >> keuzeAantal[0];
    cout << "Geef het aantal stenen waaruit speler 1 kan kiezen (1.."
         << MaxKeuzeAantal << "): ";
    cin >> keuzeAantal[1];

}  // vraagParameters

//*************************************************************************

// Schrijf het menu op het scherm en vraag een keuze van de gebruiker.
// Retourneer: de keuze van de gebruiker
int keuzeUitMenu() {
    int keuze;

    cout << endl;
    cout << "1. Een zet uitvoeren" << endl;
    cout << "2. Laatste zet ongedaan maken" << endl;
    cout << "3. Optimale score (met optimale zet) bepalen" << endl;
    cout << "4. Een zet bepalen voor grootste cluster" << endl;
    cout << "5. Een zet bepalen met Monte Carlo" << endl;
    cout << "6. Score grootste cluster tegen optimaal bepalen"
         << endl;
    cout << "7. Score Monte Carlo tegen optimaal bepalen" << endl;
    cout << "8. Score grootste cluster tegen Monte Carlo bepalen"
         << endl;
    cout << "9. Stoppen met dit spel" << endl;
    cout << endl;
    cout << "Maak een keuze: ";
    cin >> keuze;

    return keuze;

}  // keuzeUitMenu

//*************************************************************************

// Roep aq1->optScore aan, meet de benodigde tijd, en zet de relevante
// data op het scherm.
void roepOptScoreAan(Aqualin *aq1) {
    clock_t t1, t2;
    pair<int, int> optZet;
    int score;
    long long
        aantalStanden;  // aantal bekeken standen bij aanroep optScore

    t1 = clock();
    score = aq1->optScore(optZet, aantalStanden);
    t2 = clock();
    cout << endl;
    cout << "Optimale score is: " << score << endl;
    cout << "Een optimale zet is: (" << optZet.first << ","
         << optZet.second << ")" << endl;
    cout << "We hebben hiervoor " << aantalStanden
         << " standen bekeken." << endl;
    cout << "Dit kostte " << (t2 - t1) << " clock ticks, ofwel "
         << (((double)(t2 - t1)) / CLOCKS_PER_SEC) << " seconden."
         << endl;

}  // roepOptScoreAan

//*************************************************************************

// Speel het spel op het bord van aq1.
// Hierbij krijgt de gebruiker herhaaldelijk de keuze om
//   - een zet uit te voeren (een steen neerleggen op het volgende
//   vakje)
//   - de laatste zet ongedaan te maken
//   - te vragen om de score voor de speler die aan de beurt is, als
//   beide
//     spelers vanaf dit moment optimaal verder spelen
//   - te vragen om een zet voor de speler die aan de beurt is, die
//   zorgt
//     voor een (direct) zo groot mogelijke cluster
//   - te vragen om een zet voor de speler die aan de beurt is, die
//   volgens
//     een Monte Carlo simulatie tot een zo hoog mogelijke score leidt
//   - te vragen om de eindscore voor de speler die aan de beurt is,
//     als hij vanaf nu
//     . volgens grootste cluster speelt, terwijl de andere speler
//       steeds een optimale zet doet
//     . volgens Monte Carlo speelt, terwijl de andere speler
//       steeds een optimale zet doet
//     . volgens grootste cluster speelt, terwijl de andere speler
//       steeds volgens Monte Carlo speelt
//     . random zetten kiest, terwijl de andere speler
//       steeds volgens Monte Carlo speelt
// Voor elke iteratie van het menu wordt de stand afgedrukt.
//
// Dit alles gaat door totdat de gebruiker aangeeft dat hij wil
// stoppen met het spel.
void doeSpel(Aqualin *aq1) {
    int keuze, kleur,
        vorm,  // van steen die we willen neerleggen bij doeZet
        score;
    pair<int, int> zet;
    long long
        aantalStanden;  // aantal bekeken standen bij aanroep optScore

    do {
        aq1->drukAf();
        keuze = keuzeUitMenu();

        switch (keuze) {
            case 1:
                cout << endl;
                // Geen suggestie voor wat kleur en vorm mogen zijn:
                // het moet immers overeenkomen met een steen in de
                // hand van de speler aan de beurt.
                cout << "Geef het nummer van de kleur: ";
                cin >> kleur;
                cout << "Geef het nummer van de vorm: ";
                cin >> vorm;
                if (!(aq1->doeZet(kleur, vorm))) {
                    cout << endl;
                    cout << "Er is geen zet gedaan." << endl;
                }
                break;
            case 2:
                if (!(aq1->unDoeZet())) {
                    cout << endl;
                    cout << "Er is geen zet ongedaan gemaakt."
                         << endl;
                }
                break;
            case 3:
                roepOptScoreAan(aq1);
                break;
            case 4:
                zet = aq1->bepaalZetGrootsteCluster();
                cout << endl;
                cout << "Een zet voor grootste cluster is: ("
                     << zet.first << "," << zet.second << ")" << endl;
                break;
            case 5:
                zet = aq1->bepaalZetMonteCarlo();
                cout << endl;
                cout << "Een zet met Monte Carlo is: (" << zet.first
                     << "," << zet.second << ")" << endl;
                break;
            case 6:
                score = aq1->speelUitScore(2, 1);
                cout << endl;
                cout << "Score grootste cluster tegen optimaal is: "
                     << score << endl;
                break;
            case 7:
                score = aq1->speelUitScore(3, 1);
                cout << endl;
                cout << "Score Monte Carlo tegen optimaal is: "
                     << score << endl;
                break;
            case 8:
                score = aq1->speelUitScore(2, 3);
                cout << endl;
                cout
                    << "Score grootste cluster tegen Monte Carlo is: "
                    << score << endl;
                break;
            case 9:
                break;
            default:
                cout << endl;
                cout << "Voer een goede keuze in!" << endl;
        }  // switch

    } while (keuze != 9);

    if (aq1->eindstand()) {
        cout << endl;
        cout << "De huidige stand is een eindstand.\n";
        cout << "De score voor de speler die aan de beurt zou zijn "
                "is: "
             << aq1->optScore(zet, aantalStanden) << endl;
    }

}  // doeSpel

//*************************************************************************

typedef struct {
    int hoogte;
    int breedte;
    int vormen;
    int k0;
    int k1;
} config_t;

config_t configs_e1[4] = {{4, 4, 4, 2, 2},
                          {4, 5, 4, 2, 2},
                          {3, 5, 5, 2, 2},
                          {4, 4, 4, 3, 2}};

// Voer experiment uit met vier kleine configuraties van spellen,
// met algoritmes voor grootste cluster en optimaal.
void doeExperiment1() {
    for (int z = 0; z < 4; z++) {
        config_t c = configs_e1[z];
        double sum = 0;

        for (int i = 0; i < 100; i++) {
            int k[2] = {c.k0, c.k1};
            Aqualin *aq1 =
                new Aqualin(c.hoogte, c.breedte, c.vormen, 0, k);
            sum += aq1->speelUitScore(2, 1);
            delete aq1;
        }

        cout << "config " << z
             << ": De gemiddelde score van brian is: " << sum / 100.0f
             << endl;
    }
}  // doeExperiment1

//*************************************************************************

// Voer experiment uit met vier kleine configuraties van spellen,
// met algoritmes met Monte Carlo en optimaal.
void doeExperiment2() {
    for (int z = 0; z < 4; z++) {
        config_t c = configs_e1[z];
        double sum = 0;

        for (int i = 0; i < 100; i++) {
            int k[2] = {c.k0, c.k1};
            Aqualin *aq1 =
                new Aqualin(c.hoogte, c.breedte, c.vormen, 0, k);
            sum += aq1->speelUitScore(3, 1);
            delete aq1;
        }

        cout << "config " << z
             << ": De gemiddelde score van brian is: " << sum / 100.0f
             << endl;
    }
}  // doeExperiment2

//*************************************************************************

// Voer experiment uit met een maximale configuratie,
// voor de twee gretige algoritmes.
void doeExperiment3() {
    double sum = 0;

    for (int i = 0; i < 100; i++) {
        int k[2] = {5, 5};
        Aqualin *aq1 = new Aqualin(10, 10, 10, 0, k);
        sum += aq1->speelUitScore(2, 3);
        delete aq1;
    }

    cout << "De gemiddelde score van brian is: " << sum / 100.0f
         << endl;

}  // doeExperiment3

//*************************************************************************

void hoofdmenu() {
    Aqualin *aq1;  // pointer, om makkeijk nieuwe objecten te kunnen
                   // maken en weer weg te gooien
    int keuze, hoogte, breedte, vormen, alOpBord, keuzeAantal[2];
    char invoernaam[MaxBestandsNaamLengte + 1];

    do {
        cout << endl;
        cout << "1. Een nieuw spel starten" << endl;
        cout << "2. Een spel inlezen" << endl;
        cout << "3. Experiment 1 uitvoeren" << endl;
        cout << "4. Experiment 2 uitvoeren" << endl;
        cout << "5. Experiment 3 uitvoeren" << endl;
        cout << "6. Stoppen" << endl;
        cout << endl;
        cout << "Maak een keuze: ";
        cin >> keuze;
        switch (keuze) {
            case 1:
                vraagParameters(hoogte, breedte, vormen, alOpBord,
                                keuzeAantal);
                aq1 = new Aqualin(hoogte, breedte, vormen, alOpBord,
                                  keuzeAantal);
                doeSpel(aq1);
                delete aq1;  // netjes opruimen
                break;
            case 2:
                aq1 = new Aqualin();
                cout << "Geef de naam van het tekstbestand met het "
                        "spel: ";
                cin >> invoernaam;
                if (aq1->leesInSpel(invoernaam)) doeSpel(aq1);
                delete aq1;  // netjes opruimen
                break;
            case 3:
                doeExperiment1();
                break;
            case 4:
                doeExperiment2();
                break;
            case 5:
                doeExperiment3();
                break;
            case 6:
                break;
            default:
                cout << endl;
                cout << "Voer een goede keuze in!" << endl;
        }

    } while (keuze != 6);

}  // hoofdmenu

//*************************************************************************

int main() {
    hoofdmenu();

    return 0;
}
