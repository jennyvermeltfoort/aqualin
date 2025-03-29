// Enkele constantes voor implementatie Aqualin

#ifndef ConstantesHVar  // voorkom dat dit bestand meerdere keren
#define ConstantesHVar  // ge-include wordt

#include <utility>
using namespace std;

typedef pair<int, int> steen_t;
typedef pair<int, int> pos_t;

const int AlgoOptScore = 1;
const int AlgoGrootsteCluster = 2;
const int AlgoMonteCarlo = 3;

const int MaxDimensie = 10;  // maximaal aantal rijen en maximaal
                             // aantal kolommen op het bord
const int MaxKleurenVormen = 10;  // maximaal aantal kleuren of
                                  // verschillende vormen
const int MaxKeuzeAantal =
    5;                // maximaal aantal stenen in hand van speler
const int Leeg = -1;  // een niet-geldig nummer voor kleur of vorm
const int NrSimulaties = 100;  // voor Monte Carlo simulatie
const pair<int, int> GeenSteen =
    make_pair(-1, -1);  // ongeldige steen

#endif
