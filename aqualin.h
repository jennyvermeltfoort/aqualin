// Definitie van klasse Aqualin

#ifndef AqualinHVar  // voorkom dat dit bestand meerdere keren
#define AqualinHVar  // ge-include wordt

#include "constantes.h"
using namespace std;

class Hand {
   private:
    bool heeft_opslag[MaxDimensie * MaxDimensie / 2 +
                      MaxKleurenVormen * MaxKleurenVormen + 1];

    /**
     * @brief Map een steen aan of de speler deze in zijn hand heeft.
     */
    bool *heeft;

    /**
     * @brief Map een index aan een steen in de hand van de speler.
     */
    int hand[MaxKeuzeAantal];

    /**
     * @brief Map een steen aan een index voor hand[].
     */
    int index_opslag[MaxDimensie * MaxDimensie / 2 +
                     MaxKleurenVormen * MaxKleurenVormen];
    int *index;

    /**
     * @brief Een lijst van vrije indexes.
     */
    int prul[MaxDimensie * MaxDimensie / 2];
    int pot[MaxDimensie * MaxDimensie / 2];
    int prul_index = 0;
    int pot_index = 0;
    int aantal_pot = 0;

    int aantal = 0;
    int formaat = MaxKeuzeAantal;

   public:
    /**
     * @brief Beheerd de status van een hand van de speler.
     */
    Hand(void);

    void vooruit(int steen);
    void achteruit(void);
    void geef_init(int steen);

    /**
     * @brief Heeft de hand de steen.
     * @param steen De hash van de steen.
     * @return true De hand heeft de steen.
     * @return false De hand heeft niet de steen.
     */
    bool heeft_steen(int steen);

    /**
     * @brief Lees steen op index van hand[].
     * @param i De index.
     * @return int De hash van de steen.
     */
    int lees_steen(int i);

    /**
     * @brief De hoeveelheid stenen in de hand.
     * @return int Aantal stenen.
     */
    int lees_aantal(void);

    /**
     * @brief Lees het formaat van de hand.
     * @return int Het formaat.
     */
    int lees_formaat(void);

    /**
     * @brief Zet het formaat van de hand, default is MaxKeuzeAantal.
     * @param _formaat Het formaat.
     */
    void zet_formaat(int _formaat);

    /**
     * @brief Print de hand.
     * @param hash_naar_steen De map van een hash van een steen naar
     * steen_t.
     */
    void laat_zien(steen_t *hash_naar_steen);
};

class Cluster {
   private:
    /**
     * @brief Een 2d matrix van pointer waardes. Hier worden posities
     * gemapped aan een pointer die naar de index waarde van de
     * cluster verwijzen. Dit is handig wanneer twee clusters worden
     * verbonden, aangezien je twee cluster gemakkelijk kunt verbinden
     * met *cluster_1 = *cluster_2, zonder ingewikkelde poespas.
     */
    int *clusters[MaxDimensie * MaxDimensie];

    /**
     * @brief Een 2d matrix die een cluster index mapped aan een
     * counter.
     */
    int tellers[MaxDimensie * MaxDimensie];

    /**
     * @brief Een 2d matrix die bestaat uit alle cluster indexes.
     */
    int waardes[MaxDimensie * MaxDimensie];

    /**
     * @brief Het aantal gemaakt clusters.
     */
    int aantal = 0;

    /**
     * @brief De index van de cluster waar teller[index] het grootste
     * is.
     */
    int grootste = 0;

   public:
    /**
     * @brief Een cluster is een set van posities die een pointer
     * delen naar een geindexeerde waarde. Wanneer een cluster met een
     * ander cluster word verbonden door een verbindende steen zal de
     * index waarde van een van de clusters worden veranderd zodat
     * deze gelijk is aan de index van de andere cluster. Tijdens het
     * maken van verbindingen worden in tellers[] de hoeveelheid
     * stenen in een cluster bepaald. `grootste` bevat de index waarde
     * van het grootste cluster.
     */
    Cluster();

    /**
     * @brief Verbind twee clusters met elkaar. `c2` is master.
     * @param c1 cluster 1.
     * @param c2 cluster 2.
     */
    void verbind(int *c1, int *c2);

    /**
     * @brief Verhoog de teller van de opgegeven cluster index.
     * @param waarde de index waarde van de cluster.
     */
    void verhoog(int index);

    /**
     * @brief Maak een cluster op posities `p`.
     * @param p de positie.
     */
    void maak(int p);

    /**
     * @brief Reset de set van clusters.
     * @param hoogte de hoogte van het bord voor de volgende
     * berekening.
     * @param breedte de breedte van het bord voor de volgende
     * bereking.
     */
    void reset(int hoogte, int breedte);

    /**
     * @brief Combineer de cluster van `p1` en `p2`. Wanneer het
     * cluster van `p1` niet bestaat zal deze aan het eind van deze
     * functie gelijk zijn met `p2`. Wanneer het cluster van `p1` wel
     * bestaat zal `p1` met `p2` verbonden worden via verbind(), met
     * `p2` als master.
     * @param p1
     * @param p2
     */
    void zet(int p1, int p2);

    /**
     * @brief Lees de teller van de grootste cluster.
     * @return int de hoeveelheid stenen van het grootste cluster.
     */
    int waarde_grootste(void);

    /**
     * @brief Bereken de sum van kwadraten van alle clusters, i.e.
     * c1^2 + c2^2 + c3^2 + ... + c_n^2 = sum
     * met n = aantal, en c_x != 0.
     * @return int De sum van kwadraten van alle clusters.
     */
    int waarde_kwadraat(void);

    /**
     * @brief Print de huidige cluster stand.
     */
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
    /**
     * @brief Hash van steen gemapped aan steen_t.
     */
    steen_t
        hash_naar_steen_storage[MaxKleurenVormen * MaxKleurenVormen +
                                1 + MaxDimensie * MaxDimensie];

    /**
     * @brief y * breedte + x gemapped aan pos_t.
     */
    pos_t posities[MaxDimensie * MaxDimensie];

    /**
     * @brief De pot met stenen. Padded met GeenStenen (steen < 0).
     */
    int pot[MaxDimensie * MaxDimensie + MaxKeuzeAantal * 2];

    /**
     * @brief Het bord.
     */
    int bord[MaxDimensie * MaxDimensie];

    /**
     * @brief Hand van speler "kleuren".
     */
    Hand hand_kleuren = Hand();

    /**
     * @brief Hand van speler "vormen".
     */
    Hand hand_vormen = Hand();

    /**
     * @brief Cluster voor cluster berekingen van speler "kleuren".
     */
    Cluster cluster_kleur = Cluster();

    /**
     * @brief Cluster voor cluster berekingen van speler "vormen".
     */
    Cluster cluster_vorm = Cluster();

    /**
     * @brief Index van de pot voor de huidige stand.
     */
    int pot_index = 0;

    /**
     * @brief Index van de huidige positie volgorde voor de huidige
     * stand.
     */
    int positie_index = 0;

    /**
     * @brief De start van de pot nadat de stenen aan het begin op het
     * bord zijn gelegd.
     */
    int pot_index_start = 0;

    /**
     * @brief Aantal stenen wat aan het begin op het bord wordt
     * gelegd.
     */
    int leg_op_bord = 0;

    /**
     * @brief Huidige actieve speler.
     */
    speler_e speler_actief = speler_kleur;

    /**
     * @brief speler_e gemapped aan de bijbehorde hand.
     * speler_kleur => hand_kleuren.
     * speler_vorm => hand_kleuren.
     */
    Hand *hand_speler[2] = {0};

    /**
     * @brief speler_e gemapped aan de bijbehorde cluster.
     * speler_kleur => cluster_kleuren.
     * speler_vorm => cluster_kleuren.
     */
    Cluster *cluster_speler[2];

    int hoogte = 0;
    int breedte = 0;
    int aantal_vormen = 0;
    int neg_counter = 0;
    steen_t *hash_naar_steen = nullptr;

    /**
     * @brief Initializeer de pot.
     * @param hoogte De hoogte van het bord.
     * @param breedte De breedte van het bord.
     */
    inline void init_pot(int hoogte, int breedte);

    /**
     * @brief Initializeer het bord.
     * @param hoogte De hoogte van het bord.
     * @param breedte De breedte van het bord.
     * @param aantal_vormen Het aantal vormen.
     * @param leg_op_bord Hoeveel stenen er aan het begin op het bord
     * moeten worden gelegd.
     * @param formaat_kleuren Het formaat van de hand van de speler
     * "kleuren".
     * @param formaat_vormen Het formaat van de hand van de speler
     * "vormen".
     */
    void init_bord(int hoogte, int breedte, int aantal_vormen,
                   int leg_op_bord, int formaat_kleuren,
                   int formaat_vormen);

    /**
     * @brief Initializeer de hash naar stenen map.
     * @param aantal_vormen Het aantal vormen.
     * @param aantal_kleuren Het aantal kleuren.
     */
    void init_lut_steen(int aantal_vormen, int aantal_kleuren);

    /**
     * @brief Neem de volgende steen uit de pot.
     * @return int De hash van de steen.
     */
    inline int pot_volgende(void);

    /**
     * @brief Geef de vorige steen terug aan de pot.
     * @return int De hash van de vorige steen.
     */
    inline int pot_vorige(void);

    /**
     * @brief Neem de volgende positie van de vulvolgorde.
     * @return pos_t De positie.
     */
    inline pos_t positie_volgende(void);

    /**
     * @brief Neem de vorige positie van de vulvolgorde.
     * @return pos_t De positie.
     */
    inline pos_t positie_vorige(void);

    /**
     * @brief Wissel de actieve speler.
     */
    inline void speler_wissel(void);

    /**
     * @brief Neem een steen uit de pot en geeft deze aan `speler`.
     * Controlleert niet of de hand van de speler vol is.
     * @param speler De speler waarvan de steen in de hand moet
     * vallen.
     */
    inline void speler_pot_neem(speler_e speler);

    /**
     * @brief Geef de vorige steen terug aan de pot en neem deze uit
     * de `speler` hand. Controlleert niet of de speler de steen
     * heeft.
     * @param speler De speler waar de steen in de hand ligt.
     */
    inline void speler_pot_terug(speler_e speler);

    /**
     * @brief Lees steen die op positie `pos` van het bord ligt.
     * @param pos De positie.
     * @return int De hash van de steen.
     */
    inline int lees_steen(pos_t pos);

    /**
     * @brief Plaats een `steen` op positie `pos` op het bord.
     * @param pos De positie.
     * @param steen De hash van de steen.
     */
    inline void leg_steen(pos_t pos, int steen);

    /**
     * @brief Verwijder de steen op `pos` van het bord.
     * @param pos De positie.
     */
    inline void verwijder_steen(pos_t pos);

    /**
     * @brief Doet hetzelfde als doeZet, maar neemt de steen als hash
     * als input.
     * @param steen hash van de steen.
     * @return true Zet gemaakt.
     * @return false Zet niet gemaakt.
     */
    void doe_zet(int steen);
    void un_doe_zet(void);

    /**
     * @brief Bepaald of de twee stenen op het bord op posities `pos1`
     * en `pos2` hetzelfde zijn; voor kleur en vorm apart.
     * @param pos1 Positie 1.
     * @param pos2 Positie 2.
     * @return pair<bool, bool> first: De twee stenen hebben de kleur
     * gemeen. second: De twee stenen hebben de vorm gemeen.
     */
    inline pair<bool, bool> zijn_verbonden(int pos1, int pos2);

    /**
     * @brief Controleert of de twee posities met elkaar zijn
     * verbonden. Zo ja, verbind twee posities op het bord met elkaar
     * met de Cluster implementatie. Zo nee, dan word er voor positie
     * `p` een cluster index aangemaakt. Het idee hierachter is dat
     * dit voorbereiding is voor de volgende iteratie binnen
     * bereken_score. In Cluster::zet(p1, p2) wordt namelijk niet
     * gecontrolleerd of deze cluster een nullptr (cluster van p2) is,
     * aangezien hier branching zoveel mogelijk verkomen moet worden
     * (vanwege optimalizatie).
     * @param p Positie `p`
     * @param b Een buur van positie `p`.
     */
    void verbind_buur(int p, int b);

    /**
     * @brief Ga over het huidige bord en bepaal alle clusters. Zie
     * Cluster implementatie voor meer informatie. berkeken_clusters
     * gaat van links boven naar rechts onder over het bord en
     * gaandeweg maakt het of verbind het clusters met elkaar. De
     * score kan hierna opgehaald worden met de relevante clusters.
     * Deze implementatie reset de clusters aan het begin, dus haal
     * eerst de score op voordat deze opnieuw word gecalled.
     */
    void bereken_clusters(void);

    /**
     * @brief Zie OptScore, dit is een helper functie.
     * @return pair<int, int> De beste zet.
     */
    pair<int, int> opt_score(void);

    /**
     * @brief Zie OptScore, dit is een helper functie.
     * @param optZet De beste zet.
     * @param aantalStanden De hoeveelheid leaves van de
     * actie-toestand boom. ((k0*(p/2)) * (k1 * (p/2)) * k0! * k1!) /
     * (n - m)
     *
     * 1/(n-m) is een correctie voor wanneer het spel eerder stopt
     * wanneer een van de spelers geen stenen meer heeft, wat voorkomt
     * wanneer de formaat van de twee handen een verschil van groter
     * dan 3 heeft.
     *
     * Met p de aantal stenen in de pot, m de hoeveelheid stenen
     * gelegd in eindstand, en n het totaal aantal te spelen stenen.
     *
     * n = hoogte * breedte - leg_op_bord
     * p = n - k0 - k1
     *
     * m is te berekenen door: (floor en ceil omdat bij een oneven p
     * brian een steen meer krijgt)
     * m = 2k0 + ceil(p/2) * 2  bij k0 < k1
     * m = 2k1 + floor(p/2) * 2 + 1  bij k0 > k1 (brian speel 1x meer)
     * m = k0 + k1 + p     bij k0 = k1
     * @return int
     */
    int opt_score(pair<int, int> &optZet, long long &aantalStanden);

    /**
     * @brief Zie bepaalZetMonteCarlo, dit is een helper functie.
     * @return int hash van de steen van de beste zet.
     */
    int monte_carlo();
};

#endif
