#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>  

using namespace std;

typedef array<pair<char, double>, 26> FreqArray;

class VigenereCryptanalysis
{
private:
    array<double, 26> targets;

public:

    VigenereCryptanalysis(const array<double, 26>& targetFreqs)
    {
        targets = targetFreqs;
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
            if (isalpha(input[i]))  
            {
                char upperChar = toupper(input[i]);
                tab[upperChar - 'A'].second++;
            }
        }
        return tab;
    }

    double IndexIC(const string& input)
    {
        FreqArray tab = Frequency(input);
        double IC = 0.0;
        int n = input.size();
        if (n > 1)
        {
            IC = 1.0 / (n * (n - 1));
            double sum = 0.0;
            for (int i = 0; i < 26; i++)
            {
                double freq = tab[i].second;
                sum += freq * (freq - 1);
            }
            IC *= sum;
        }
        return IC;
    }

    vector<pair<double, double>> AvgIc(const string& input)
    {
        vector<pair<double, double>> averageIC(14);
        for (int keyLength = 2; keyLength <= 15; keyLength++)
        {
            vector<string> sequences(keyLength, "");
            double totalIC = 0.0;
            for (size_t i = 0; i < input.size(); i++) 
            {
                sequences[i % keyLength] += input[i];
            }
            for (int i = 0; i < keyLength; i++)
            {
                totalIC += IndexIC(sequences[i]);
            }
            averageIC[keyLength - 2] = {keyLength, totalIC / keyLength};
        }
        return averageIC;
    }

    void periodFind(const string& input)
    {
        vector<pair<double, double>> IC = AvgIc(input); 
        auto max1 = max_element(IC.begin(), IC.end(), [](const pair<double, double>& a, const pair<double, double>& b) {
            return a.second < b.second;
        });
        cout << "max1 (key length): " << max1->first << " with IC: " << max1->second << endl;

        IC.erase(max1); 

        auto max2 = max_element(IC.begin(), IC.end(), [](const pair<double, double>& a, const pair<double, double>& b) {
            return a.second < b.second;
        });
        cout << "max2 (key length): " << max2->first << " with IC: " << max2->second << endl;
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


    vector<pair<int, double>> calculdeChiSurSequences(const string& input, int period)
    {
        vector<string> sequences = SequenceDePeriod(input, period);
        vector<pair<int, double>> resultatChi(sequences.size());  

        for (size_t i = 0; i < sequences.size(); i++)
        {
            vector<pair<int, double>> tableau(26);  

            for (int shift = 0; shift < 26; shift++)
            {
                string shiftedSequence = applyShift(sequences[i], shift);  
                cout << "Sequence " << i + 1 << " after shift " << shift << ": " << shiftedSequence << endl;

                tableau[shift].first = shift;
                tableau[shift].second = chiSquared(shiftedSequence);  
            }

            auto minChi = min_element(tableau.begin(), tableau.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
                return a.second < b.second;
            });

            resultatChi[i].first = minChi->first;  
            resultatChi[i].second = minChi->second;  
        }

        for (size_t i = 0; i < resultatChi.size(); i++)
        {
            std::cout << "Best shift for sequence " << i + 1 << ": " << resultatChi[i].first << " with chi-squared: " << resultatChi[i].second << std::endl;
        }

        return resultatChi;
    }


    string reconstitutionKey(const string& input, int period)
    {
        vector<pair<int, double>> statChi = calculdeChiSurSequences(input, period);
        string key(period, ' '); 
        for (int i = 0; i < period; i++)
        {
            key[i] = 'A' + statChi[i].first; 
        }
        return key;
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

    vc_en.periodFind("vptnvffuntshtarptymjwzirappljmhhqvsubwlzzygvtyitarptyiougxiuydtgzhhvvmum"
                     "shwkzgstfmekvmpkswdgbilvjljmglmjfqwioiivknulvvfemioiemojtywdsajtwmtcgluy"
                     "sdsumfbieugmvalvxkjduetukatymvkqzhvqvgvptytjwwldyeevquhlulwpkt");


    int period = 7;  
    cout << "Reconstructed key: " << vc_en.reconstitutionKey("vptnvffuntshtarptymjwzirappljmhhqvsubwlzzygvtyitarptyiougxiuydtgzhhvvmum"
                                  "shwkzgstfmekvmpkswdgbilvjljmglmjfqwioiivknulvvfemioiemojtywdsajtwmtcgluy"
                                  "sdsumfbieugmvalvxkjduetukatymvkqzhvqvgvptytjwwldyeevquhlulwpkt", period) << endl;

    return 0;
}
