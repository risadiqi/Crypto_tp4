#include <iostream>
#include <string>
#include <vector>
#include <map>        // Permet d'utiliser le conteneur map pour stocker des paires clé-valeur
#include <algorithm>  // Permet d'utiliser les fonctions utilitaires de la STL comme max_element, min_element, etc.
#include <array>      
#include <cctype>     // Permet d'utiliser les fonctions de manipulation de caractères comme toupper et isalpha
#include <cmath>      // Permet d'utiliser des fonctions mathématiques comme pow pour les calculs de puissance

using namespace std;

// Typedef pour faciliter la manipulation d'un tableau de fréquences avec 26 lettres (A-Z)
typedef array<pair<char, double>, 26> FreqArray;

// Classe principale pour l'analyse cryptographique de Vigenère
class VigenereCryptanalysis
{
private:
    // Tableau des fréquences cibles de la langue, ici celles de l'anglais
    array<double, 26> targets;

public:

    // Constructeur : Initialise les fréquences cibles utilisées pour le test du chi-carré
    VigenereCryptanalysis(const array<double, 26>& targetFreqs)
    {
        targets = targetFreqs;
    }

    // Fonction "Frequency" : Calcule la fréquence des lettres dans un texte donné
    FreqArray Frequency(const string& input)
    {
        FreqArray tab;
        // Initialisation du tableau avec les lettres de A à Z et des fréquences à 0
        for (int i = 0; i < 26; i++)
        {
            tab[i] = make_pair('A' + i, 0);
        }
        // Parcourt le texte et compte les occurrences de chaque lettre
        for (size_t i = 0; i < input.size(); i++)
        {
            if (isalpha(input[i]))  // Vérifie si c'est une lettre
            {
                char upperChar = toupper(input[i]);  // Conversion en majuscule
                tab[upperChar - 'A'].second++;  // Incrémente la fréquence de la lettre correspondante
            }
        }
        return tab;
    }

    // Fonction "IndexIC" : Calcule l'indice de coïncidence d'un texte donné.
    // Cet indice aide à déterminer si le texte est en clair ou chiffré et la longueur probable de la clé.
    double IndexIC(const string& input)
    {
        FreqArray tab = Frequency(input);
        double IC = 0.0;
        int n = input.size();
        if (n > 1)
        {
            // Formule pour calculer l'indice de coïncidence
            IC = 1.0 / (n * (n - 1));
            double sum = 0.0;
            for (int i = 0; i < 26; i++)
            {
                double freq = tab[i].second;
                sum += freq * (freq - 1);  // Somme des produits des fréquences
            }
            IC *= sum;
        }
        return IC;
    }

    // Fonction "AvgIc" : Calcule l'indice de coïncidence moyen pour différentes longueurs de clé (de 2 à 15).
    // Cela permet d'estimer la longueur de la clé utilisée dans le chiffre de Vigenère.
    vector<pair<double, double>> AvgIc(const string& input)
    {
        vector<pair<double, double>> averageIC(14);
        // Teste les longueurs de clé de 2 à 15
        for (int keyLength = 2; keyLength <= 15; keyLength++)
        {
            vector<string> sequences(keyLength, "");
            double totalIC = 0.0;
            // Sépare le texte en différentes séquences selon la longueur de la clé
            for (size_t i = 0; i < input.size(); i++) 
            {
                sequences[i % keyLength] += input[i];
            }
            // Calcule l'indice de coïncidence pour chaque séquence
            for (int i = 0; i < keyLength; i++)
            {
                totalIC += IndexIC(sequences[i]);
            }
            averageIC[keyLength - 2] = {keyLength, totalIC / keyLength};  // Stocke l'IC moyen
        }
        return averageIC;
    }

    // Fonction "periodFind" : Trouve la période probable de la clé en cherchant les meilleures valeurs d'indice de coïncidence (IC).
    void periodFind(const string& input)
    {
        vector<pair<double, double>> IC = AvgIc(input); 
        // Trouve la période avec l'IC maximal
        auto max1 = max_element(IC.begin(), IC.end(), [](const pair<double, double>& a, const pair<double, double>& b) {
            return a.second < b.second;
        });
        cout << "max1 (key length): " << max1->first << " with IC: " << max1->second << endl;

        IC.erase(max1);  // Supprime la période avec le meilleur IC pour en trouver un autre

        auto max2 = max_element(IC.begin(), IC.end(), [](const pair<double, double>& a, const pair<double, double>& b) {
            return a.second < b.second;
        });
        cout << "max2 (key length): " << max2->first << " with IC: " << max2->second << endl;
    }

    // Fonction "chiSquared" : Calcule le test du chi-carré pour comparer les fréquences observées avec les fréquences attendues.
    // Cela aide à évaluer la correspondance des lettres d'un texte chiffré avec une langue connue.
    double chiSquared(const string& input)
    {
        FreqArray tab = Frequency(input);
        double chiSquaredValue = 0.0;

        for (int i = 0; i < 26; i++)
        {
            double observed = tab[i].second;
            double expected = targets[i] * input.size();
            if (expected > 0)
            {
                chiSquaredValue += pow(observed - expected, 2) / expected;  // Formule du chi-carré
            }
        }
        return chiSquaredValue;
    }

    // Fonction "applyShift" : Applique un décalage de type César à une séquence donnée.
    // Ce décalage est utilisé pour tester différentes clés sur les séquences du texte.
    string applyShift(const string& sequence, int shift) {
        string shiftedSequence = sequence;
        for (size_t k = 0; k < shiftedSequence.size(); k++) {
            if (isalpha(shiftedSequence[k])) {  // Vérifie si c'est une lettre
                shiftedSequence[k] = 'A' + (toupper(shiftedSequence[k]) - 'A' - shift + 26) % 26;  // Décale la lettre
            }
        }
        return shiftedSequence;
    }

    // Fonction "SequenceDePeriod" : Sépare le texte chiffré en plusieurs séquences selon la période de la clé estimée.
    // Chaque séquence correspond aux lettres qui ont été chiffrées avec la même lettre de la clé.
    vector<string> SequenceDePeriod(const string& input, int period)
    {
        vector<string> sequences(period);
        for (size_t i = 0; i < input.size(); i++)
        {
            sequences[i % period] += input[i];  // Distribue les lettres dans les différentes séquences
        }

        // Affiche les séquences obtenues
        for (size_t i = 0; i < sequences.size(); i++) {
            cout << "Sequence " << i + 1 << ": " << sequences[i] << endl;
        }

        return sequences;
    }

    // Fonction "calculdeChiSurSequences" : Calcule le chi-carré pour chaque séquence après différents décalages.
    // Elle identifie le meilleur décalage pour chaque séquence, ce qui correspond à une partie de la clé de Vigenère.
    vector<pair<int, double>> calculdeChiSurSequences(const string& input, int period)
    {
        vector<string> sequences = SequenceDePeriod(input, period);
        vector<pair<int, double>> resultatChi(sequences.size());  

        // Pour chaque séquence, teste tous les décalages possibles (0 à 25)
        for (size_t i = 0; i < sequences.size(); i++)
        {
            vector<pair<int, double>> tableau(26);  

            for (int shift = 0; shift < 26; shift++)
            {
                string shiftedSequence = applyShift(sequences[i], shift);  // Applique le décalage à la séquence
                cout << "Sequence " << i + 1 << " after shift " << shift << ": " << shiftedSequence << endl;

                tableau[shift].first = shift;
                tableau[shift].second = chiSquared(shiftedSequence);  // Calcule le chi-carré pour cette séquence décalée
            }

            // Trouve le décalage qui minimise le chi-carré
            auto minChi = min_element(tableau.begin(), tableau.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
                return a.second < b.second;
            });

            resultatChi[i].first = minChi->first;  
            resultatChi[i].second = minChi->second;  
        }

        // Affiche le meilleur décalage pour chaque séquence
        for (size_t i = 0; i < resultatChi.size(); i++)
        {
            std::cout << "Best shift for sequence " << i + 1 << ": " << resultatChi[i].first << " with chi-squared: " << resultatChi[i].second << std::endl;
        }

        return resultatChi;
    }

    // Fonction "reconstitutionKey" : Reconstitue la clé de chiffrement en utilisant les meilleurs décalages trouvés pour chaque séquence.
    string reconstitutionKey(const string& input, int period)
    {
        vector<pair<int, double>> statChi = calculdeChiSurSequences(input, period);
        string key(period, ' ');  // Initialise la clé avec des espaces
        for (int i = 0; i < period; i++)
        {
            key[i] = 'A' + statChi[i].first;  // Assigne la lettre correspondant au meilleur décalage
        }
        return key;
    }

};

// Fonction principale pour tester l'analyse cryptographique
int main()
{
    // Fréquences des lettres en anglais
    array<double, 26> english = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
    0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
    0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
    0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
    0.01974, 0.00074
    };

    // Création d'un objet VigenereCryptanalysis avec les fréquences en anglais
    VigenereCryptanalysis vc_en(english);

    // Analyse du texte chiffré pour trouver la longueur de la clé
    vc_en.periodFind("vptnvffuntshtarptymjwzirappljmhhqvsubwlzzygvtyitarptyiougxiuydtgzhhvvmum"
                     "shwkzgstfmekvmpkswdgbilvjljmglmjfqwioiivknulvvfemioiemojtywdsajtwmtcgluy"
                     "sdsumfbieugmvalvxkjduetukatymvkqzhvqvgvptytjwwldyeevquhlulwpkt");

    // Supposons que la période trouvée soit 7
    int period = 7;  
    // Reconstitution de la clé à partir du texte et de la période
    cout << "Reconstructed key: " << vc_en.reconstitutionKey("vptnvffuntshtarptymjwzirappljmhhqvsubwlzzygvtyitarptyiougxiuydtgzhhvvmum"
                                  "shwkzgstfmekvmpkswdgbilvjljmglmjfqwioiivknulvvfemioiemojtywdsajtwmtcgluy"
                                  "sdsumfbieugmvalvxkjduetukatymvkqzhvqvgvptytjwwldyeevquhlulwpkt", period) << endl;

    return 0;
}
