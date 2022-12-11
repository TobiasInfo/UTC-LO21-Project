#include "partie.h"

#include <iostream>

#include <vector>

using namespace std;
// ----------------------------------------------------------------
// Getter functions
// ----------------------------------------------------------------
int Partie::getManche() const { return manche; }

int Partie::getNumDe() const { return num_de; }

int Partie::getNbJoueurs() const { return nb_joueurs; }

//int Partie::getNbMonuments() const { return nb_monuments; }

//int Partie::getNbEtablissements() const { return nb_etablissements; }

Jeu *Partie::getJeu() const { return jeu; }

Joueur *Partie::getJoueurActif()  { return joueurs1[joueur_actif]; }

Reserve *Partie::getReserve() const { return reserve; }
const vector<Joueur*>& Partie::getJoueurs() const {return joueurs1;}


// ----------------------------------------------------------------
// Setter functions
// ----------------------------------------------------------------

// ce n'est pas plutôt des void le type de retour des setters ?
void  Partie::setManche(int nbManche) { manche = nbManche; }

void  Partie::setNumDe(int nbDe) { num_de = nbDe; }//inutile ?

void  Partie::setNbJoueurs(int nbJoueurs) { nb_joueurs = nbJoueurs; }

//void  Partie::setNbMonuments(int nbMonuments) { nb_monuments = nbMonuments; }

/*void  Partie::setNbEtablissements(int nbEtablissements)
{
  nb_etablissements = nbEtablissements;
}*/


// ----------------------------------------------------------------
// Constructors and destructors functions
// ----------------------------------------------------------------
Partie::Partie()
    : manche(0),
      num_de(1),
      nb_joueurs(2)//,
      //nb_monuments(4),
      //nb_etablissements(15)
      /* joueur_actif(new Joueur),
      joueurs(new Joueur[nb_joueurs])*/
{ 
}

Partie::~Partie() {}

// ----------------------------------------------------------------
// Methods
// ----------------------------------------------------------------
void Partie::initialisation()
{
  cout << "Bonjour, bienvenu dans le jeu MachiKoro" << endl;

  // TODO: A completer
  // Il faut recuperer l'extension ? Pour pouvoir initialiser les variables aux
  // bonnes valeurs
  int lectureExtension;
retry:  
  cout << "Entrez l'extension a laquelle vous voulez jouer \t0. Originale\t1. Marina \t2. GreenValley\t3. Deluxe"
       << endl;
  do
  {
      cout << "L'extension doit etre comprise entre 0 et 3" << endl;
      cin >> lectureExtension;

  } while (lectureExtension < 0 || lectureExtension > 3);
  jeu = new Jeu(Extensions[lectureExtension]);
  /*switch (lectureExtension)
  {
  case 0:
    setNbMonuments(4);       // TODO Vérifier le nb de monuments exact
    setNbEtablissements(15); // TODO Vérifier le nb de etablissements exact
    //jeu = new Jeu(lectureExtension);
    jeu = new Jeu(Extension::Aucune);
    break;
  case 1:
    setNbMonuments(4);       // TODO Vérifier le nb de monuments exact
    setNbEtablissements(15); // TODO Vérifier le nb de etablissements exact

    jeu = new Jeu(Extension::Marina);
    break;
  case 2:
    setNbMonuments(4);       // TODO Vérifier le nb de monuments exact
    setNbEtablissements(15); // TODO Vérifier le nb de etablissements exact

    jeu = new Jeu(Extension::GreenValley);
    break;
  case 3:
    setNbMonuments(4);       // TODO Vérifier le nb de monuments exact
    setNbEtablissements(15); // TODO Vérifier le nb de etablissements exact

    jeu = new Jeu(Extension::Deluxe);
    break;
  }*/

  cout << "Entrez le nombre de joueurs :" << endl;
  int lecture = 0;
  while (lecture < 2 || lecture > jeu->getNb_joueurs_MAX())
  {
      cout << "Le nombre de joueur doit être compris entre 2 et " << jeu->getNb_joueurs_MAX() << endl;
      cin >> lecture;
  } 
  //cout << jeu->getEtablissementsDepart()[0]->getNbExemplaires();
  if ( (jeu->getEtablissementsDepart()[0]->getNbExemplaires()) < lecture) goto retry;// Si pas assez de cartes départ

  setNbJoueurs(lecture);

//Initialisation reserve
  reserve = new Reserve(*jeu);
//Initialisation joueur
  for(size_t i=0;i<getNbJoueurs();i++){
    joueurs1.push_back(new Joueur(jeu));
  }
}

void Partie::distribuer()
{
  // TODO: A completer
}

void Partie::joueur_next(){
  if (joueur_actif==joueurs1.size()-1)//Quand c'est le dernier joueur
    joueur_actif = 0;
  else
    joueur_actif++;
}

/* Joueur Partie::tour_joueur(Joueur joueur)
{
  int i = 0;
  while (i < getNbJoueurs() && getJoueurActif()!= joueur)
  {
    i++;
  }
  int indice = (i + 1) % getNbJoueurs();
  return getJoueurI(indice);
}
Etablissement* Partie::regarder_etablissements(Joueur joueur, Couleur couleur)
{
  //???
}


void Partie::application_regle_standards(Couleur couleur)
{
  // TODO: A completer
} */



void Partie::transaction_piece(Joueur* emetteur, Joueur*destinataire, int montant){
  if (montant>emetteur->getCompte())
    cout << "Erreur ! Le joueur " << emetteur->getPseudo() << " n'a pas de ressource suffisante pour effectuer la transaction.";
  else{
    emetteur->ajouterMontant(montant*(-1));
    destinataire->ajouterMontant(montant);
  }
}

void Partie::transaction_carte(Joueur* emetteur, Joueur*destinataire, const Etablissement* etab){
  if (destinataire->retirer_etablissement(etab))
    emetteur->ajouter_etablissement(etab);
}

bool Partie::achat_carte(Pile_Etablissement* pile_reserve) {
    if (pile_reserve->getEffectif()>0){
        if (pile_reserve->getEtablissement()->getPrix()<=getJoueurActif()->getCompte()){
             getJoueurActif()->ajouterMontant(0-pile_reserve->getEtablissement()->getPrix());
             getJoueurActif()->ajouter_etablissement(pile_reserve->getEtablissement());
             pile_reserve->retirerCarte();
             return true;
        }
        else cout<<"Prix de l'etablissement superieur au montant de votre compte !"<<endl;
    }
    else cout<<"La pile est vide !"<<endl;
    return false;
}



//ACHAT MONUMENTS 
bool Partie::construire_monument(const Monument* monument_choisi){
  Carte_Monument* cible = nullptr;
  for (auto i : getJoueurActif()->getMonuments()){
    if (monument_choisi==i->getMonument()){
      cible = i;
      break;
    }
  }

  if (cible->estConstruit())
    return false;
  else{
    if (cible->getMonument()->getPrix()>getJoueurActif()->getCompte()){
      cout << "Construction échouée" << endl;
      return false;
    }else{
    cible->construire();
    getJoueurActif()->ajouterMontant((-1)*cible->getMonument()->getPrix());
    return true; 
    }
  }



}

//void Partie::regarder_etablissements(Joueur* joueur, Couleur couleur) {

/*
void Partie::achat_carte(Joueur *joueur, Pile_Etablissement *pile)
{
  if (joueur->getCompte() >= pile->getPrix())
  {
    if (pile->retirerCarte(1))
    {
      joueur->ajouter_etablissement(pile->getEtablissement());
      joueur->ajouterMontant((-1)*pile->getPrix());
      cout << "Achat terminé !" << endl;
    }
    else
    {
      cout << "Plus de carte disponible !" << endl;
    }
  }
  else
  {
    cout << "Le joueur" << joueur->getId() << "n'a pas de ressource suffisante !" << endl;
  }
*/


int Partie::fonction_service_verte(Type t){
  if (t == Type::aucun )
    return 1;
  else{
    int nombreactive=0;
    for (auto p : getJoueurActif()->getPileBleu()){
      if (p->getEtablissement()->getType()==t)
        nombreactive++;    
    }
    for (auto p : getJoueurActif()->getPileRouge()){
      if (p->getEtablissement()->getType()==t)
        nombreactive++;    
    }
    for (auto p : getJoueurActif()->getPileVert()){
      if (p->getEtablissement()->getType()==t)
        nombreactive++;    
    }
    for (auto p : getJoueurActif()->getPileViolet()){
      if (p->getEtablissement()->getType()==t)
        nombreactive++;    
    }
    return nombreactive;
  }
}

void Partie::find_carte_des(int des)
{
  cout << endl << "Bilan revenue :" << endl;
  for (auto joueur : getJoueurs())
  {
    cout << "  " << joueur->getId() << " Montant avant " << joueur->getCompte() << endl;
    cout << "    "<< "Cartes activée :" << endl;


    //Pile Rouge
    if (joueur != getJoueurActif())
    {
      for (auto pileRouge : joueur->getPileRouge())
      {

        int montant;
        for (auto k : pileRouge->getEtablissement()->getNumDe())
        {
          if (k == des)
          {
            montant = pileRouge->getEtablissement()->getMontant() * pileRouge->getEffectif();
            joueur->ajouterMontant(montant);
            cout << "      " << pileRouge->getEtablissement()->getNom() << "  Quantité : " << pileRouge->getEffectif() << endl;

            getJoueurActif()->ajouterMontant((-1) * montant); // Le joueur en cours paye
            break;
          }
        }
      }
    }

    //Pile Verte
    if (joueur == getJoueurActif())
    {
      for (auto p : joueur->getPileVert())
      {
        for (auto k : p->getEtablissement()->getNumDe())
        {
          if (k == des)
          {
            joueur->ajouterMontant(p->getEtablissement()->getMontant() * p->getEffectif()*fonction_service_verte(p->getEtablissement()->getTypeEffet()));
            cout << "      " << p->getEtablissement()->getNom() << "  Quantité : " << p->getEffectif() << endl;
            break;
          }
        }
      }
    }

    //Pile Bleue
    for (auto p : joueur->getPileBleu())
    {
      for (auto k : p->getEtablissement()->getNumDe())
      {
        if (k == des)
        {
          joueur->ajouterMontant(p->getEtablissement()->getMontant() * p->getEffectif());
          cout << "      " << p->getEtablissement()->getNom() << "  Quantité : " << p->getEffectif() << endl;
          break;
        }
      }
    }


    //Pile Violette
    if (joueur == getJoueurActif())
    {
      /* for (auto p : joueur->pileViolet)
      {
          for (auto k : p->getEtablissement()->getNumDe())
          {
              if (k == des)
              {
                  ajouterMontant(p->getEtablissement()->getMontant() * p->getEffectif());
                  cout << "  " << p->getEtablissement()->getNom() << "  Quantité : " << p->getEffectif() << endl;
                  break;
              }
          }
      } */
      for (auto p : joueur->getPileViolet()) // à discuter, maintenant je liste simplement les 3 cartes dans la version basique
      {

        for (auto k : p->getEtablissement()->getNumDe())
        {
          if (k == des)
          {
            if (p->getEtablissement()->getNom() == "Stade")
            {
              joueur->ajouterMontant(p->getEtablissement()->getMontant() * p->getEffectif());
              //pas obligé de faire * p->getEffectif() car on ne peut avoir qu'un seul établissement spécial
              cout << "      " << p->getEtablissement()->getNom() << "  Quantité : " << p->getEffectif() << endl;
              //
            }
            break;
          }
        }
      }
    }

    cout << "    " << "Montant Après " << joueur->getCompte() << endl;
    cout << endl;
  }
}

//TO DO : 
//1 acheter donc construire monument !!!!
//2 checker UN SEUL exemplaire d'établissement special !!!