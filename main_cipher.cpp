#include <iostream>
#include <string>
using namespace std;

// Classe Vigenere : Cette classe implémente l'algorithme de chiffrement/déchiffrement de Vigenère.
//
// Le chiffrement de Vigenère est un chiffrement qui utilise une clé pour décaler chaque lettre du texte
// clair d'un nombre de positions dans l'alphabet, correspondant à la lettre de la clé. La clé est répétée 
// ou étendue pour correspondre à la longueur du texte. Pour le déchiffrement, le processus est inversé en 
// utilisant la même clé, réappliquant les décalages dans la direction opposée pour retrouver le texte original.

// Classe Vigenere qui implémente un chiffrement de type Vigenère
class Vigenere
{
public:
  string key;  // La clé utilisée pour le chiffrement et le déchiffrement
 
 // Constructeur qui prend une clé et la normalise (convertit tout en majuscules)
  Vigenere(string key)
  {
    // Parcourt la clé fournie, conserve uniquement les lettres de l'alphabet (majuscule et minuscule),
    // et convertit les minuscules en majuscules
    for(unsigned int i = 0; i < key.size(); ++i)
    {
      if(key[i] >= 'A' && key[i] <= 'Z')  // Si c'est déjà une majuscule, on la garde
        this->key += key[i];
      else if(key[i] >= 'a' && key[i] <= 'z')  // Si c'est une minuscule, on la convertit en majuscule
        this->key += key[i] + 'A' - 'a';
    }
  }

  // Fonction pour agrandir la clé afin qu'elle ait au moins la taille du texte à chiffrer
  string agrandirKey(unsigned int taille)
  {
    for(unsigned int i=0; i<taille;i++)
    {
      key +=key[i];
    }
    return key;
  }
 
  // Fonction qui chiffre un texte donné avec la méthode de Vigenère
  string encrypt(string text)
  {
    string out;  // Chaîne de sortie temporaire pour le texte converti en majuscule
    string resultat;  // Chaîne finale chiffrée

    // Convertit tout le texte à chiffrer en majuscule et supprime les caractères non alphabétiques
    for(unsigned int i = 0; i < text.length(); ++i)
    {
      if(text[i] >= 'A' && text[i] <= 'Z')  // Si c'est une majuscule, on la garde
        out += text[i];
      else if(text[i] >= 'a' && text[i] <= 'z')  // Si c'est une minuscule, on la convertit en majuscule
        out += text[i] + 'A' - 'a';
    }

    // Si la clé est plus courte que le texte, on l'agrandit
    if(out.size() > key.size())
      agrandirKey(out.size() - key.size());

    // Algorithme de chiffrement de Vigenère : on applique le décalage des lettres en fonction de la clé
    for(unsigned int i = 0; i < out.size(); ++i)
    {
      // Chaque caractère du texte est décalé en fonction du caractère correspondant dans la clé
      resultat += (out[i] - key[i] + 2 * 'A') % 26 + 'A';  
    }
 
    return resultat;  
  }
 
  // Fonction qui déchiffre un texte chiffré avec la méthode de Vigenère
  string decrypt(string text)
  {
    string out;  // Chaîne qui contiendra le texte déchiffré

    // Si la clé est plus courte que le texte chiffré, on l'agrandit
    if(text.size() > key.size())
      agrandirKey(text.size() - key.size());

    // Algorithme de déchiffrement de Vigenère : on applique le décalage inverse des lettres en fonction de la clé
    for(unsigned int i = 0; i < text.size(); ++i)
    {
      // Chaque caractère chiffré est décalé inversement selon la clé correspondante
      out += (text[i] + key[i] - 2 * 'A') % 26 + 'A';  
    }
    return out;  
  }
};

//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

int main()
{
  // On crée un objet Vigenere avec la clé "MYKEY"
  Vigenere cipher("MYKEY");
 
  // Texte original à chiffrer
  string original_en  = "Principe de Kerckhoffs - Toute la securite d'un systeme cryptographique doit reposer sur la clef et pas sur le systeme lui meme.";
  //On chiffre le texte original
  string encrypted_en = cipher.encrypt(original_en);
  // On déchiffre le texte chiffré
  string decrypted_en = cipher.decrypt(encrypted_en);
 
  cout << original_en << endl;
  cout << "Encrypted: " << encrypted_en << endl;
  cout << "Decrypted: " << decrypted_en << endl;
  /*
  // Un autre exemple en français, commenté ici
  string original_fr  = "Principe de Kerckhoffs - Toute la securite d'un systeme cryptographique doit reposer sur la clef et pas sur le systeme lui meme.";

  string encrypted_fr = cipher.encrypt(original_fr);
  string decrypted_fr = cipher.decrypt(encrypted_fr);
 
  cout << original_fr << endl;
  cout << "Encrypted: " << encrypted_fr << endl;
  cout << "Decrypted: " << decrypted_fr << endl;
  */
}

