#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>  // For pow

using namespace std;

typedef array<pair<char, double>, 26> FreqArray;

class VigenereCryptanalysis
{
private:
    array<double, 26> targets;
    array<double, 26> sortedTargets;

public:
    VigenereCryptanalysis(const array<double, 26>& targetFreqs)
    {
        targets = targetFreqs;
        sortedTargets = targets;
        sort(sortedTargets.begin(), sortedTargets.end());
    }

    FreqArray Frequency(const string& input)
    {
        FreqArray tab;
        for (int i = 0; i < 26; i++)
        {
            tab[i] = make_pair('A' + i, 0);
        }

        for (size_t i = 0; i < input.size(); i++)
        {
            if (isalpha(input[i]))  // Check if character is a letter
            {
                char upperChar = toupper(input[i]);
                tab[upperChar - 'A'].second++;
            }
        }

        return tab;
    }

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
                chiSquaredValue += pow(observed - expected, 2) / expected;
            }
        }
        return chiSquaredValue;
    }

    string applyShift(const string& sequence, int shift) {
        string shiftedSequence = sequence;
        for (size_t k = 0; k < shiftedSequence.size(); k++) {
            if (isalpha(shiftedSequence[k])) { 
                shiftedSequence[k] = 'A' + (toupper(shiftedSequence[k]) - 'A' - shift + 26) % 26;
            }
        }
        return shiftedSequence;
    }




    vector<string> SequenceDePeriod(const string& input, int period)
    {
        vector<string> sequences(period);
        for (size_t i = 0; i < input.size(); i++)
        {
            sequences[i % period] += input[i];
        }

        for (size_t i = 0; i < sequences.size(); i++) {
            cout << "Sequence " << i + 1 << ": " << sequences[i] << endl;
        }

        return sequences;
    }


    void calculdeChiSurSequences(const string& input, int period)
    {
        vector<string> sequences = SequenceDePeriod(input, period);
        vector<pair<int, double>> resultatChi(sequences.size());  // Store best shift and chi-squared for each sequence

        for (size_t i = 0; i < sequences.size(); i++)
        {
            vector<pair<int, double>> tableau(26);  // Store chi-squared values for each shift (0-25)

            for (int shift = 0; shift < 26; shift++)
            {
                string shiftedSequence = applyShift(sequences[i], shift);  // Apply shift
                
                // Affichage pour voir chaque séquence après décalage
                cout << "Sequence " << i + 1 << " after shift " << shift << ": " << shiftedSequence << endl;

                tableau[shift].first = shift;
                tableau[shift].second = chiSquared(shiftedSequence);  // Calculate chi-squared value for shifted sequence
            }

            // Find the shift with the minimum chi-squared value
            auto minChi = min_element(tableau.begin(), tableau.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
                return a.second < b.second;
            });

            resultatChi[i].first = minChi->first;  // Store the best shift
            resultatChi[i].second = minChi->second;  // Store the best chi-squared value
        }

        for (size_t i = 0; i < resultatChi.size(); i++)
        {
            std::cout << "Best shift for sequence " << i + 1 << ": " << resultatChi[i].first << " with chi-squared: " << resultatChi[i].second << std::endl;
        }
    }

};

int main()
{
    array<double, 26> english = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
    0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
    0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
    0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
    0.01974, 0.00074
    };


    VigenereCryptanalysis vc_en(english);

    int period = 7;  // Example period
    vc_en.calculdeChiSurSequences("vptnvffuntshtarptymjwzirappljmhhqvsubwlzzygvtyitarptyiougxiuydtgzhhvvmum"
                                  "shwkzgstfmekvmpkswdgbilvjljmglmjfqwioiivknulvvfemioiemojtywdsajtwmtcgluy"
                                  "sdsumfbieugmvalvxkjduetukatymvkqzhvqvgvptytjwwldyeevquhlulwpkt", period);

    return 0;
}
