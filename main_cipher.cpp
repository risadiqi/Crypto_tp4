#include <iostream>
#include <string>
using namespace std;

class Vigenere
{
public:
  string key;
 
  Vigenere(string key)
  {
    // Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
    for(unsigned int i = 0; i < key.size(); ++i)
    {
      if(key[i] >= 'A' && key[i] <= 'Z')
        this->key += key[i];
      else if(key[i] >= 'a' && key[i] <= 'z')
        this->key += key[i] + 'A' - 'a';
    }
  }

  string agrandirKey(unsigned int taille)
  {
    for(unsigned int i=0; i<taille;i++)
    {
      key +=key[i];
    }
    return key;
  }
 
  string encrypt(string text)
  {
    string out;
    string  resultat;

    // Modifying all characters other than uppercase : lowercase -> uppercase, other -> delete
    for(unsigned int i = 0; i < text.length(); ++i)
    {
      if(text[i] >= 'A' && text[i] <= 'Z')
        out += text[i];
      else if(text[i] >= 'a' && text[i] <= 'z')
        out += text[i] + 'A' - 'a';
    }

    // ADD THE VIGENERE CRYPTION
    if(out.size()>key.size())
      agrandirKey(out.size()-key.size());

    for(unsigned int i = 0; i < out.size(); ++i)
    {
      resultat += (out[i]-key[i] + 2*'A')%26 + 'A';
    }
 
    return resultat;
  }
 
  string decrypt(string text)
  {
    string out;

    if(text.size()>key.size())
      agrandirKey(text.size()-key.size());

    for(unsigned int i = 0; i < text.size(); ++i)
    {
      out += (text[i]+key[i] - 2*'A')%26 + 'A';
    }
    return out;
  }
};

//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

int main()
{
  Vigenere cipher("MYKEY");
 
  string original_en  = "Principe de Kerckhoffs - Toute la securite d'un systeme cryptographique doit reposer sur la clef et pas sur le systeme lui meme.";
  string encrypted_en = cipher.encrypt(original_en);
  string decrypted_en = cipher.decrypt(encrypted_en);
 
  cout << original_en << endl;
  cout << "Encrypted: " << encrypted_en << endl;
  cout << "Decrypted: " << decrypted_en << endl;
  /*
  string original_fr  = "Principe de Kerckhoffs - Toute la securite d'un systeme cryptographique doit reposer sur la clef et pas sur le systeme lui meme.";

  string encrypted_fr = cipher.encrypt(original_fr);
  string decrypted_fr = cipher.decrypt(encrypted_fr);
 
  cout << original_fr << endl;
  cout << "Encrypted: " << encrypted_fr << endl;
  cout << "Decrypted: " << decrypted_fr << endl;
  */
}

