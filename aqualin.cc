// Implementatie van klasse Aqualin

#include "aqualin.h"

#include <cmath>
#include <cstdlib>
#include <fstream>  // voor inlezen van bord
#include <iostream>

#include "standaard.h"

Hand::Hand(void) {
    heeft = &heeft_opslag[MaxDimensie * MaxDimensie / 2 + 1];
    index = &index_opslag[MaxDimensie * MaxDimensie / 2];

    heeft[-1] = false;

    for (int i = 0; i < MaxKeuzeAantal; i++) {
        hand[i] = -1;
    }

    // pad de pot met unieke negatieve stenen zodat er geen if
    // statements nodig zijn in de rest van de code.
    for (int i = 0; i < MaxDimensie * MaxDimensie / 2; i++) {
        pot[i] = -(i + 2);  // +2 om -1 voor heeft[-1] false te laten.
    }
}

inline int min(int a, int b) { return (a < b) ? a : b; }

void Hand::zet_formaat(int _formaat) { formaat = _formaat; }
void Hand::geef_init(int steen) {
    if (aantal < formaat) {
        hand[aantal] = steen;
        heeft[steen] = true;
        index[steen] = aantal;
        aantal++;
    } else {
        pot[aantal_pot++] = steen;
    }
}
int Hand::lees_formaat(void) { return formaat; }
int Hand::lees_aantal(void) {
    int a = formaat + -(pot_index - aantal_pot);
    return min(a, formaat);
}
int Hand::lees_steen(int i) { return hand[i]; }

void Hand::vooruit(int steen) {
    int p = pot[pot_index++];  // steen uit pot
    int i = index[steen];
    prul[prul_index++] = hand[i];
    hand[i] = p;
    index[p] = i;
    heeft[p] = true;
    heeft[steen] = false;
}

void Hand::achteruit(void) {
    int p = prul[--prul_index];  // neem uit prullebak.
    int i = index[p];            // oude index van p.
    int s = hand[i];             // huidige steen @ oude index van p.
    heeft[s] = false;
    heeft[p] = true;
    hand[i] = p;
    pot_index--;
}

bool Hand::heeft_steen(int steen) { return heeft[steen]; }

void Hand::laat_zien(steen_t *hash_naar_steen) {
    std::cout << "(kleur,vorm):";
    for (int i = 0; i < formaat; i++) {
        int s = hand[i];
        if (s >= 0) {
            steen_t steen = hash_naar_steen[s];
            std::cout << "(" << s << ": " << steen.first << ","
                      << steen.second << ")";
        }
    }
}

//*************************************************************************

Aqualin::Aqualin() {}  // default constructor

//*************************************************************************

void Aqualin::init_lut_steen(int _aantal_vormen, int aantal_kleuren) {
    hash_naar_steen =
        &hash_naar_steen_storage[MaxDimensie * MaxDimensie];

    // Aangezien alle GeenStenen in de pot uniek zijn word hiervoor
    // ook een unieke hash naar steen map gemaakt. Zie init_bord()
    // voor meer info.
    for (int i = MaxDimensie * MaxDimensie * -1; i < 0; i++) {
        hash_naar_steen[i] = {i, i};
    }

    for (int k = 0; k < aantal_kleuren; k++) {
        for (int v = 0; v < _aantal_vormen; v++) {
            hash_naar_steen[k * _aantal_vormen + v] = {k, v};
        }
    }
}

inline void Aqualin::init_pot(int _hoogte, int _breedte) {
    for (int i = _hoogte * _breedte;
         i < _hoogte * _breedte + MaxKeuzeAantal * 2; i++) {
        pot[i] = -1;
        // padd de pot met lege stenen, (hoogte * breedte) / 2 formaat
        // van padding ie groote van het bord gedeeld door het aantal
        // handen. Voorkomt extra checks in pot_neem.
    }
}

void Aqualin::init_bord(int _hoogte, int _breedte, int _aantal_vormen,
                        int leg_op_bord, int formaat_kleuren,
                        int formaat_vormen) {
    hoogte = _hoogte;
    breedte = _breedte;
    aantal_vormen = _aantal_vormen;
    int aantal_kleuren = ceil((float)_hoogte * (float)_breedte /
                              (float)_aantal_vormen);

    hand_kleuren.zet_formaat(formaat_kleuren);
    hand_vormen.zet_formaat(formaat_vormen);

    cluster_speler[speler_kleur] = &cluster_kleur;
    cluster_speler[speler_vorm] = &cluster_vorm;
    hand_speler[speler_kleur] = &hand_kleuren;
    hand_speler[speler_vorm] = &hand_vormen;

    // Alle GeenStenen zijn uniek op het bord om
    // de cluster bereking efficienter te maken.
    // Voorkomt namelijk een extra if statement
    // in de bereken_clusters loop (geen steen != -1 controle
    // noodzakelijk).
    neg_counter = _hoogte * breedte * -1;
    for (int i = _hoogte * breedte - 1; i >= 0; i--) {
        pos_t p = posities[i];
        bord[p.second * _breedte + p.first] = neg_counter++;
    }
    init_lut_steen(_aantal_vormen, aantal_kleuren);

    for (int i = 0; i < leg_op_bord; i++) {
        leg_steen(positie_volgende(), pot[pot_index++]);
    }

    for (int i = 0; i < hand_kleuren.lees_formaat(); i++) {
        hand_kleuren.geef_init(pot[pot_index++]);
    }

    for (int i = 0; i < hand_vormen.lees_formaat(); i++) {
        hand_vormen.geef_init(pot[pot_index++]);
    }

    bool toggle = 0;  // speler_kleur;
    for (int i = pot_index; i < _hoogte * _breedte; i++) {
        hand_speler[toggle]->geef_init(pot[i]);
        toggle = !toggle;
    }

    pot_index_start = pot_index;
}

// Constructor met parameters.
Aqualin::Aqualin(int nwHoogte, int nwBreedte, int nwVormen,
                 int nwAlOpBord, int nwKeuzeAantal[2]) {
    int buf[MaxDimensie * MaxDimensie] = {};

    genereerRandomPermutatie(nwHoogte * nwBreedte, buf);
    for (int y = 0; y < nwHoogte; y++) {
        for (int x = 0; x < nwBreedte; x++) {
            posities[buf[y * nwBreedte + x]] = {x, y};
        }
    }

    init_pot(nwHoogte, nwBreedte);
    genereerRandomPermutatie(nwHoogte * nwBreedte, buf);
    for (int i = 0; i < nwHoogte * nwBreedte; i++) {
        pot[i] = buf[i];
    }

    init_bord(nwHoogte, nwBreedte, nwVormen, nwAlOpBord,
              nwKeuzeAantal[0], nwKeuzeAantal[1]);
}  // constructor met parameters

Aqualin::~Aqualin() {}

//*************************************************************************

inline pos_t Aqualin::positie_volgende(void) {
    return posities[positie_index++];
}

inline pos_t Aqualin::positie_vorige(void) {
    return posities[--positie_index];
}

inline void Aqualin::speler_wissel(void) {
    speler_actief =
        (speler_actief == speler_kleur) ? speler_vorm : speler_kleur;
}

inline void Aqualin::leg_steen(pos_t pos, int steen) {
    bord[breedte * pos.second + pos.first] = steen;
    neg_counter--;  // zie init_bord voor meer info.
}

inline void Aqualin::verwijder_steen(pos_t pos) {
    bord[breedte * pos.second + pos.first] =
        neg_counter++;  // zie init_bord voor meer info.
}

inline int Aqualin::lees_steen(pos_t pos) {
    return bord[breedte * pos.second + pos.first];
}

inline pair<bool, bool> Aqualin::zijn_verbonden(int pos1, int pos2) {
    steen_t steen1 = hash_naar_steen[bord[pos1]];
    steen_t steen2 = hash_naar_steen[bord[pos2]];
    return pair<bool, bool>{(steen1.first == steen2.first),
                            (steen1.second == steen2.second)};
}

bool Aqualin::leesInSpel(const char *invoernaam) {
    std::fstream fs;
    fs.open(invoernaam, std::fstream::in);
    if (!fs.is_open()) {
        return false;
    }

    int _hoogte;
    int _breedte;
    fs >> _hoogte;
    fs >> _breedte;

    int index = 0;
    for (int y = 0; y < _hoogte; y++) {
        for (int x = 0; x < _breedte; x++) {
            fs >> index;
            posities[index] = {x, y};
        }
    }

    int _aantal_vormen;
    fs >> _aantal_vormen;

    init_pot(_hoogte, _breedte);
    int getal = 0;
    for (int i = 0; i < _hoogte * _breedte; i++) {
        fs >> getal;
        pot[i] = getal;
    }

    int leg_op_bord = 0;
    int formaat_kleuren = 0;
    int formaat_vormen = 0;
    fs >> leg_op_bord;
    fs >> formaat_kleuren;
    fs >> formaat_vormen;
    init_bord(_hoogte, _breedte, _aantal_vormen, leg_op_bord,
              formaat_kleuren, formaat_vormen);

    return true;
}  // leesInSpel

//*************************************************************************

void Aqualin::drukAf() {
    std::cout << std::endl;
    for (int y = 0; y < hoogte; y++) {
        for (int x = 0; x < breedte; x++) {
            int steen = lees_steen({x, y});
            if (steen >= 0) {
                steen_t s = hash_naar_steen[steen];
                std::cout << "(" << s.first << "," << s.second
                          << ")\t";
            } else {
                std::cout << "(" << -1 << "," << -1 << ")\t";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "vulvolgorde (x,y):";
    for (int i = positie_index; i < hoogte * breedte; i++) {
        std::cout << "(" << posities[i].first << ","
                  << posities[i].second << ")";
    }
    std::cout << std::endl;

    std::cout << "pot (kleur,vorm):";
    for (int i = pot_index; i < hoogte * breedte; i++) {
        steen_t steen = hash_naar_steen[pot[i]];
        std::cout << "(" << steen.first << "," << steen.second << ")";
    }
    std::cout << std::endl;

    std::cout << "hand_kleuren";
    hand_kleuren.laat_zien(hash_naar_steen);
    std::cout << std::endl;
    std::cout << "hand_vormen";
    hand_vormen.laat_zien(hash_naar_steen);
    std::cout << std::endl;
}  // drukAf

//*************************************************************************

bool Aqualin::eindstand() {
    return (positie_index >= (hoogte * breedte) ||
            hand_speler[speler_actief]->lees_aantal() == 0);
}  // eindstand

//*************************************************************************

void Aqualin::doe_zet(int steen) {
    leg_steen(positie_volgende(), steen);
    hand_speler[speler_actief]->vooruit(steen);
    speler_wissel();
    pot_index++;
}

bool Aqualin::doeZet(int kleur, int vorm) {
    int steen = kleur * aantal_vormen + vorm;
    bool heeft = hand_speler[speler_actief]->heeft_steen(steen);
    if (heeft) {
        doe_zet(steen);
    }
    return heeft;
}  // doeZet

//*************************************************************************

void Aqualin::un_doe_zet(void) {
    speler_wissel();
    verwijder_steen(positie_vorige());
    hand_speler[speler_actief]->achteruit();
    pot_index--;
}

bool Aqualin::unDoeZet() {
    if (pot_index == pot_index_start) {
        return false;  // geen zet gemaakt.
    }

    un_doe_zet();
    return true;
}  // unDoeZet

//*************************************************************************

Cluster::Cluster() { reset(MaxDimensie, MaxDimensie); }

void Cluster::reset(int hoogte, int breedte) {
    for (int i = 0; i < (hoogte * breedte); i++) {
        tellers[i] = 0;
        waardes[i] = i;
        clusters[i] = nullptr;
    }
    grootste = 0;
    aantal = 0;
}

void Cluster::verhoog(int index) {
    tellers[index]++;
    if (tellers[index] > tellers[grootste]) {
        grootste = index;
    }
}

void Cluster::maak(int p) {
    if (clusters[p] == nullptr) {
        int *index = &waardes[aantal++];
        clusters[p] = index;
        verhoog(*index);
    }
}

void Cluster::zet(int p1, int p2) {
    // roep alleen aan wanneer twee posities een verbinding delen.
    int *c1 = clusters[p1];
    int *c2 = clusters[p2];
    if (c1 == nullptr) {
        // cluster bestaat nog niet, dus verbind de positie met het
        // cluster via de pointer.
        clusters[p1] = c2;
        verhoog(*c2);
    } else if (*c1 != *c2) {
        verbind(c1, c2);
    }
}

void Cluster::verbind(int *c1, int *c2) {
    tellers[*c2] += tellers[*c1];
    tellers[*c1] = 0;
    *c1 = *c2;

    if (tellers[*c1] >= tellers[grootste]) {
        grootste = *c1;
    }
}

int Cluster::waarde_grootste(void) { return tellers[grootste]; }
int Cluster::waarde_kwadraat(void) {
    int waarde = 0;
    for (int i = 0; i < aantal; i++) {
        if (tellers[i] != 0) {
            waarde += tellers[i] * tellers[i];
        }
    }
    return waarde;
}

void Cluster::laat_zien(void) {
    cout << "Grootste waarde: " << grootste << ":"
         << waarde_grootste() << endl;

    for (int i = 0; i < aantal; i++) {
        cout << i << ":" << tellers[i] << endl;
    }
}

void Aqualin::verbind_buur(int p, int b) {
    pair<bool, bool> verbonden = zijn_verbonden(p, b);
    if (verbonden.first) {
        cluster_kleur.zet(b, p);
    } else {
        cluster_kleur.maak(b);
    }
    if (verbonden.second) {
        cluster_vorm.zet(b, p);
    } else {
        cluster_vorm.maak(b);
    }
}

//*************************************************************************
void Aqualin::bereken_clusters(void) {
    // Complexiteit van O(hoogte * breedte * 2)
    cluster_kleur.reset(hoogte, breedte);
    cluster_vorm.reset(hoogte, breedte);

    cluster_kleur.maak(0);
    cluster_vorm.maak(0);
    for (int y = 0; y < hoogte - 1; y++) {
        int p = 0;
        for (int x = 0; x < breedte - 1; x++) {
            p = y * breedte + x;
            verbind_buur(p, p + breedte);  // onder
            verbind_buur(p, p + 1);        // rechts
        }
        verbind_buur(p + 1,
                     p + 1 + breedte);  // onder van rechtse kolom
    }

    for (int i = hoogte * breedte - breedte; i < hoogte * breedte - 1;
         i++) {
        verbind_buur(i, i + 1);
    }
}

int Aqualin::opt_score(pair<int, int> &optZet,
                       long long &aantalStanden) {
    static steen_t zet = GeenSteen;
    int hscore = -__INT_MAX__;

    if (eindstand()) {
        aantalStanden++;
        bereken_clusters();
        return cluster_speler[speler_actief]->waarde_grootste() -
               cluster_speler[!speler_actief]->waarde_grootste();
    }

    for (int i = 0; i < hand_speler[speler_actief]->lees_formaat();
         i++) {
        int steen = hand_speler[speler_actief]->lees_steen(i);
        if (steen >= 0) {
            doe_zet(steen);
            int score = -opt_score(zet, aantalStanden);
            if (score > hscore) {
                hscore = score;
                optZet = hash_naar_steen[steen];
            }
            un_doe_zet();
        }
    }

    return hscore;
}

int Aqualin::optScore(pair<int, int> &optZet,
                      long long &aantalStanden) {
    if (eindstand()) {
        optZet = GeenSteen;
        aantalStanden = 0;
        return 0;
    }

    aantalStanden = 0;
    return opt_score(optZet, aantalStanden);
}  // optScore

pair<int, int> Aqualin::opt_score(void) {
    steen_t zet = GeenSteen;
    long long standen = 0;
    optScore(zet, standen);
    return zet;
}

//*************************************************************************

pair<int, int> Aqualin::bepaalZetGrootsteCluster() {
    steen_t zet = GeenSteen;
    speler_e speler = speler_actief;
    int hgw = 0;
    int hkw = 0;

    if (eindstand()) {
        return GeenSteen;
    }

    for (int i = 0; i < hand_speler[speler]->lees_formaat(); i++) {
        int steen = hand_speler[speler]->lees_steen(i);
        if (steen >= 0) {
            doe_zet(steen);
            bereken_clusters();
            int grootste_waarde =
                cluster_speler[speler]->waarde_grootste();
            int waarde_kwadraat =
                cluster_speler[speler]->waarde_kwadraat();
            if (grootste_waarde > hgw ||
                (grootste_waarde == hgw && waarde_kwadraat > hkw)) {
                hgw = grootste_waarde;
                hkw = waarde_kwadraat;
                zet = hash_naar_steen[steen];
            }
            un_doe_zet();
        }
    }

    return zet;
}  // bepaalZetGrootsteCluster

//*************************************************************************

int Aqualin::monte_carlo() {
    if (eindstand()) {
        bereken_clusters();
        return cluster_speler[speler_actief]->waarde_grootste() -
               cluster_speler[!speler_actief]->waarde_grootste();
    }

    // Vanwege mijn implatatie van de hand is dit een beetje lelijk,
    // lees_steen kan namelijk ook GeenSteen terug geven, en
    // Hand::lees_aantal geeft alleen het aantal terug, niet welke
    // indexes van Hand::hand vrij zijn.
    int steen = -1;
    while (steen < 0) {
        int z = rand() % hand_speler[speler_actief]->lees_formaat();
        steen = hand_speler[speler_actief]->lees_steen(z);
    }

    int score = 0;
    doe_zet(steen);
    score = -monte_carlo();
    un_doe_zet();

    return score;
}

pair<int, int> Aqualin::bepaalZetMonteCarlo() {
    steen_t zet = GeenSteen;
    double hgem = -__INT_MAX__;

    if (eindstand()) {
        return GeenSteen;
    }

    for (int i = 0; i < hand_speler[speler_actief]->lees_formaat();
         i++) {
        int steen = hand_speler[speler_actief]->lees_steen(i);
        if (steen >= 0) {
            doe_zet(steen);
            double sum = 0;
            for (int i = 0; i < 100; i++) {
                sum += -monte_carlo();
            }
            double gem = sum / 100.0f;
            if (gem > hgem) {
                hgem = gem;
                zet = hash_naar_steen[steen];
            }
            unDoeZet();
        }
    }

    return zet;
}  // bepaalZetMonteCarlo

//*************************************************************************

typedef pair<int, int> (Aqualin::*algo_t)(void);

int Aqualin::speelUitScore(int algo1, int algo2) {
    int aantal_zetten = 0;
    algo_t algos[3] = {&Aqualin::opt_score,
                       &Aqualin::bepaalZetGrootsteCluster,
                       &Aqualin::bepaalZetMonteCarlo};
    algo_t speler_algo[2] = {algos[algo1 - 1], algos[algo2 - 1]};

    while (!eindstand()) {
        steen_t zet = (this->*speler_algo[speler_actief])();
        doeZet(zet.first, zet.second);
        aantal_zetten++;
    }

    bereken_clusters();

    for (int i = 0; i < aantal_zetten; i++) {
        un_doe_zet();
    }

    return cluster_speler[speler_actief]->waarde_grootste() -
           cluster_speler[!speler_actief]->waarde_grootste();
    ;
}  // speelUitScore
