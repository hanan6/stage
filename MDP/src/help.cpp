#include "help.h"

#include <string>

using namespace std;

void Help::print(std::ostream& os) {
    static string help_string =
    "NAME\n"
    "\tmdp - .\n"
    "\n"
    "SYNOPSIS\n"
    "\tmdp [OPTIONS]\n"
    "\n"
    "DEFINITIONS\n"
    "\tUn \"alphabet\" est un ensemble de symboles.\n"
    "\tOn peut enrichir un alphabet avec un symbole, dit \"joker\", qui représente l'ensemble des autres symboles de l'alphabet.\n"
    "\tOn appelle \"symboles solides\" l'ensemble des symboles qui ne sont pas des jokers.\n"
    "\tUn \"patron\" (pattern) est une suite de symboles appartenant à un alphabet prédéfini.\n"
    "\tLa \"variabilité\" (slack) d'un mot correspond au nombre maximal de jokers consécutifs de ce mot. \"La variabilité\" d'un ensemble de mots"
    " correspond à la variabilité du mot de variabilité maximale.\n"
    "\tLa \"longueur\" (length) d'un mot correspond au nombre de symboles solides du mot. \"La longueur\" d'un ensemble de mots"
    " correspond à la somme des longueurs des mots.\n"
    "\n"
    "\t Une \"séquence\" est un mot.\n"
    "\t Une \"base de séquences\" est un ensemble de séquences indicées, c'est-à-dire qu'on associe un indice unique à toutes les séquences.\n"
    "\t Un \"motif\" pour une base de séquences B est un ensemble de patrons, de plus on associe à chaque patron du motif une localisation unique pour chacune des séquences de B.\n"
    "\n"
    "REQUIRED\n"
    "\tLes options \"--input\" et \"--output\" sont obligatoires.\n"
    "\n"
    "OPTIONS\n"
    "\t-i, --input FILE\n"
    "\tNom du fichier  qui contient la base séquences avec leurs classes respectives. Il peut être au format ecif, fasta ou csv.  Plus d'information à propos "
    "du format ecif:\n"
    "\thttp://forge.info.univ-angers.fr/~gh/Idas/ecif.php\n"
    "\t\n"
    "\t-o, --output FILE\n"
    "\tNom du fichier de sortie (format xml).\n"
    "\t\n"
    "\t-m, --minslack\n"
    "\tSlack minimum des motifs\n"
    "\t\n"
    "\t-M, --maxslack\n"
    "\tSlack maximum des motifs\n"
    "\t\n"
    "\t-v, --verbose\n"
    "\t\n"
    "EXAMPLE\n"
    "\tL'exemple suivant exécute le mdp sur la  base  de  séquences  \"leap.fasta\" "
    "(--input),   puis  enregistre  le  résultat  dans  le fichier \"leap.xml\" "
    "(--output). Dans le fichier de sortie le symbole joker sera réprésenté par \"#\" (-j) et il n'y aura aucun espacement entre deux symboles d'un même patron (-w).\n"
    "\n"
    "\tmdp --input leap.fasta --output leap.xml\n"
    "\t\n"
    "FORMAT DU FICHIER DE SORTIE\n";

    os << help_string;
}


Help::Help()
{
}
