#ifndef TYPESENUM_H
#define TYPESENUM_H

enum class Type {
	aucun,
    agriculture,
    elevage,
    ressources,
    maritime,
    commerce,
    industrie,
    maraichage,
    entreprise,
    restauration,
    special,
}

enum class Extension {
    Aucune,
    Marina,
    GreenValley,
    Deluxe
};

enum Type{agriculture,magasin,restauration,rouage};
string toString(Type t) {
		switch (t) {
		case Type::agriculture: return "agriculture";
		case Type::magasin: return "magasin";
		case Type::restauration: return "restauration";
		case Type::rouage: return "rouage";
		default: throw SetException("Type inconnu");
		}
	}
enum Couleur{bleu,rouge,violet,vert};
string toString(Couleur c) {
		switch (c) {
		case Couleur::bleu: return "bleu";
		case Couleur::rouge: return "rouge";
		case Couleur::violet: return "violet";
		case Couleur::vert: return "vert";
		default: throw SetException("Couleur inconnue");
		}
	}


#endif