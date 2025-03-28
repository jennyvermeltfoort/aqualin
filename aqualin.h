// Definitie van klasse Aqualin

#ifndef AqualinHVar  // voorkom dat dit bestand meerdere keren
#define AqualinHVar  // ge-include wordt

#include "constantes.h"
using namespace std;

typedef enum : bool {
    speler_kleur = 0,
    speler_vorm = 1,
} speler_e;

class Hand {
   private:
    bool heeft[MaxKleurenVormen * MaxKleurenVormen];
    int hand[MaxKeuzeAantal];
    int index[MaxKleurenVormen * MaxKleurenVormen];
    int vrij[MaxKeuzeAantal];

    int vrij_index = 0;
    int aantal = 0;
    int formaat = MaxKeuzeAantal;

   public:
    Hand(void);
    void geef_steen(int steen);
    void neem_steen(int steen);
    bool heeft_steen(int steen);
    int lees_steen(int i);
    int lees_aantal(void);
    int lees_formaat(void);
    void zet_formaat(int _formaat);
    void laat_zien(steen_t *hash_naar_steen);
};

class Cluster {
   private:
    int *clusters[MaxDimensie * MaxDimensie];
    int tellers[MaxDimensie * MaxDimensie];
    int waardes[MaxDimensie * MaxDimensie];

    int breedte;
    int hoogte;

    int aantal = 0;
    int grootste = 0;

   public:
    Cluster();
    void verbind(int p, int *cluster);
    void verhoog(int waarde);
    void maak(int p);
    void reset(int hoogte, int breedte);
    void zet(int p1, int p2);
    int waarde_grootste(void);
    int waarde_kwadraat(void);
    void laat_zien(void);
};

class Aqualin {
   public:
    // Default constructor.
    Aqualin();

    // Constructor met parameters voor hoogte en breedte van het
    // bord, het aantal vormen, het aantal stenen dat al op het
    // bord ligt, en de keuzeAantallen van beide spelers. Pre:
    // * De parameters van deze constructor vallen binnen de
    // grenzen
    //   van de opdracht.
    // Post:
    // * Er is een bord van nwHoogte*nwBreedte
    //   gemaakt, met een random vulvolgorde van de vakjes,
    //   met nwVormen vormen en een daarmee corresponderend aantal
    //   kleuren, met een random volgorde van de stenen in de pot,
    //   waarvan de eerste nwAlOpBord stenen al op het bord
    //   liggen, en de volgende nwKeuzeAantal[0] en
    //   nwKeuzeAantal[1] stenen (zo mogelijk) in de handen van
    //   speler 0 en speler 1 zitten (eerst krijgt speler 0 zijn
    //   stenen, dan speler 1). Speler 0 is aan de beurt.
    Aqualin(int nwHoogte, int nwBreedte, int nwVormen, int nwAlOpBord,
            int nwKeuzeAantal[2]);
    ~Aqualin(void);

    // Retourneer de hoogte van het bord.
    int getHoogte() {  // TODO: implementeer deze memberfunctie
        return 0;
    }

    // Retourneer de breedte van het bord.
    int getBreedte() {  // TODO: implementeer deze memberfunctie
        return 0;
    }

    // Retourneer het aantal verschillende kleuren van de stenen.
    int getKleuren() {  // TODO: implementeer deze memberfunctie
        return 0;
    }

    // Retourneer het aantal verschillende vormen van de stenen.
    int getVormen() {  // TODO: implementeer deze memberfunctie
        return 0;
    }

    // Getter voor de inhoud van vakje (rij,kolom) op het bord
    // (rijen genummerd van 0..hoogte-1, van boven naar beneden,
    //  kolommen van 0..breedte-1, van links naar rechts)
    // Retourneer:
    // * (kleur,vorm) van de steen als het vakje gevuld is
    // * GeenSteen, als het vakje niet gevuld is
    pos_t getVakje(
        int rij,
        int kolom) {  // TODO: implementeer deze memberfunctie
        return GeenSteen;
    }

    // Lees een spel in vanuit tekstbestand invoernaam.
    // Controleer daarbij
    // * of het bestand wel te openen is,
    // Retourneer:
    // * true, als het bestand te openen is
    // * false, anders
    // Pre:
    // * Als het bestand te openen is, vallen de getallen daarin
    // binnen
    //   de grenzen van de opdracht, en vormen de vulvolgorde en
    //   de stenen in de pot daadwerkelijk permutaties van
    //   0,1,...,(hoogte*breedte-1)
    // Post:
    // * Als het bestand te openen is, is alle ingelezen data
    //   opgeslagen in membervariabelen, zijn de stenen die al op
    //   het bord moeten liggen op hun plaatsen gelegd, zijn de
    //   handen met stenen van beide spelers (zo mogelijk) gevuld,
    //   en is speler 0 aan de beurt.
    // * Als het bestand niet te openen was, is het bestaande bord
    // niet
    //   veranderd.
    bool leesInSpel(const char *invoernaam);

    // Druk de hele stand (bord met stenen, vulvolgorde van
    // resterende vakjes, de aantallen kleuren en vormen, de
    // volgorde van resterende stenen in de pot, en de handen met
    // stenen van beide spelers) af op het scherm.
    void drukAf();

    // Controleer of we een eindstand hebben bereikt, dat wil
    // zeggen: of de speler die aan de beurt is, geen stenen meer
    // heeft om op het bord te leggen. Retourneer:
    // * true, als we een eindstand hebben bereikt
    // * false, als we geen eindstand hebben bereikt
    bool eindstand();

    // Doe een zet voor de speler die aan de beurt is:
    // het plaatsen van steen (kleur,vorm) op het vakje dat aan de
    // beurt is om gevuld te worden. Controleer eerst of het wel
    // een geldige zet is, dat wil zeggen,
    // - of (kleur,vorm) een steen in de hand van de speler is
    // Retourneer:
    // * true, als dit een geldige zet is
    // * false, als dit geen geldige zet is.
    // Post:
    // * als het een geldige zet is, is de zet uitgevoerd:
    //   - de steen ligt op het bord in het vakje dat aan de beurt
    //   is
    //   - de steen is niet meer in de hand van de speler aan
    //   beurt
    //   - de speler aan beurt heeft zo mogelijk een nieuwe steen
    //   uit
    //     de pot in zijn hand
    //   - de speler aan beurt is gewisseld,
    // * als het geen geldige zet is, is de stand niet veranderd.
    bool doeZet(int kleur, int vorm);

    // Maak de laatst gedane zet ongedaan.
    // Controleer eerst of er wel een zet is om ongedaan te maken,
    // dwz of er wel een zet gedaan is sinds constructie of sinds
    // het inlezen van het spel. Retourneer:
    // * true, als er een zet was om ongedaan te maken
    // * false, anders
    // Post:
    // * als returnwaarde true is, is de zet ongedaan gemaakt:
    //   - de speler aan beurt is teruggewisseld,
    //   - als de speler een steen uit de pot had gehaald,
    //     is deze steen weer uit zijn hand gehaald
    //   - de steen die gelegd was, is weer terug in de hand van
    //   de speler
    //   - het vakje op het bord is leeggemaakt
    // * als returnwaarde false is, is de stand niet veranderd
    bool unDoeZet();

    // Bepaal met behulp van brute force en recursie de eindscore
    // voor de speler die in de huidige stand (= de stand van de
    // huidige recursieve aanroep) aan de beurt is, wanneer beide
    // spelers vanaf dit punt optimaal verder spelen. De score is
    // gelijk aan het aantal vakjes van de eigen grootste cluster
    // min het aantal vakjes van de grootste cluster van de andere
    // speler. Post:
    // * als de huidige stand geen eindstand was, bevat parameter
    //   optZet de steen (kleur,vorm) die de huidige speler op
    //   het volgende vakje moet leggen, om de optimale score te
    //   bereiken
    // * anders bevat parameter optZet de default waarde GeenSteen
    // * aantalStanden is gelijk aan het aantal standen dat we
    // hebben
    //   bekeken bij het bepalen van de optimale eindscore
    // * de stand in het spel is nog onveranderd
    int optScore(pos_t &optZet, long long &aantalStanden);

    // Bepaal een zet voor de speler die in de huidige stand aan
    // de beurt is, waarmee hij (na deze ene zet) zo groot
    // mogelijke clusters heeft. Controleer eerst of de huidige
    // stand geen eindstand is. Retourneer:
    // * de gevonden zet (kleur,vorm), als het geen eindstand is
    // * de default waarde GeenSteen, als het al wel een eindstand
    // is
    pos_t bepaalZetGrootsteCluster();

    // Bepaal een zet voor de speler die in de huidige stand aan
    // de beurt is, met een Monte Carlo simulatie. Controleer
    // eerst of de huidige stand geen eindstand is. Retourneer:
    // * de gevonden zet (kleur,vorm), als het geen eindstand is
    // * de default waarde GeenSteen, als het al wel een eindstand
    // is
    pos_t bepaalZetMonteCarlo();

    // Speel het spel uit vanaf de huidige stand. Laat hierbij de
    // speler die in de huidige stand aan de beurt is, steeds een
    // zet kiezen volgens algoritme algo1, terwijl de andere
    // speler een zet kiest volgens algoritme algo2. Hierbij geeft
    // algoritme 1 een optimale zet volgens optScore, geeft
    // algoritme 2 een zet volgens bepaalZetGrootsteCluster en
    // geeft algoritme 3 een zet volgens bepaalZetMonteCarlo.
    // Retourneer:
    // * de score aan het eind van het spel voor de speler die in
    // de huidige
    //   stand (de stand waarbij deze functie wordt aangeroepen
    //   dus) aan de beurt is.
    // Post:
    // * de huidige stand is weer hetzelfde als aan het begin van
    // de functie
    //   (zetten zijn dus weer ongedaan gemaakt)
    int speelUitScore(int algo1, int algo2);

   private:
    // Initializers
    inline void init_pot(int hoogte, int breedte);
    void init_bord(int hoogte, int breedte, int aantal_vormen,
                   int leg_op_bord, int formaat_kleuren,
                   int formaat_vormen);
    void init_lut_steen(int hoogte, int breedte, int aantal_vormen,
                        int aantal_kleuren);

    // All assigned memory.
    steen_t
        hash_naar_steen_storage[MaxKleurenVormen * MaxKleurenVormen +
                                1];
    pos_t posities[MaxDimensie * MaxDimensie];
    int pot[MaxDimensie * MaxDimensie + MaxKeuzeAantal * 2];
    int bord[MaxDimensie * MaxDimensie];
    Hand hand_kleuren = Hand();
    Hand hand_vormen = Hand();
    Cluster cluster_kleur = Cluster();
    Cluster cluster_vorm = Cluster();

    // Variables
    int hoogte = 0;
    int breedte = 0;
    int pot_index = 0;
    int pot_index_start = 0;
    int leg_op_bord = 0;
    int aantal_vormen = 0;
    int positie_index = 0;
    steen_t *hash_naar_steen = nullptr;
    speler_e speler_actief = speler_kleur;
    Hand *hand_speler[2] = {0};
    Cluster *cluster_speler[2];

    inline int pot_volgende(void);
    inline int pot_vorige(void);
    inline pos_t positie_volgende(void);
    inline pos_t positie_vorige(void);

    inline void speler_wissel(void);
    inline void speler_pot_neem(speler_e speler);
    inline void speler_pot_terug(speler_e speler);

    inline int lees_steen(pos_t pos);
    inline void leg_steen(pos_t pos, int steen);
    inline void verwijder_steel(pos_t pos);

    inline pair<bool, bool> zijn_verbonden(int pos1, int pos2);
    void verbind_buur(int p, int b);

    bool doe_zet(int steen);

    void bereken_score(void);
    pair<int, int> opt_score(void);
    int opt_score(pair<int, int> &optZet, long long &aantalStanden);
    int monte_carlo();
};

#endif
