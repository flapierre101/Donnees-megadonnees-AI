/*

420-C52-IN DONNÉES, MÉGADONNÉES ET INTELLIGENCE ARTIFICIELLE
Projet 2 - Algorithme génétique
17 décembre 2020

Équipe:
Viens, Dany
Lapierre, François
Gaudreault,  Vincent
Emond-Serret, Caroline
Genest, Carl

AlgoGen : 

Est un algorithme génétique générique. Il est conçu pour générer et ensuite modifier plusieurs générations de données.
Il recevra comme paramètre :
- le nombre de Gènes faisant partit du chromosome
- une étendue de données possible pour chaque de ces gènes
- la population requise pour résoudre le problème
- Le ratio de mutation (entre 0 et 100)
- Le nombre d'élite à garder.
- Un pointeur de fonction pour calculer le fitness du problème.

Logique de l'algorithme :
- Lors de son initialisation, l'algorithme générera une première population de manière aléatoire.
	Une fonction "random_num" sert à fournir les données selon une étendue donnée.
- Lors du calcul de la prochaine génération
- L'algorithme va calculer la somme des fitness de sa population.
- Si la somme est de 0, cela indique qu'aucune solution viable n'a été trouvée dans la première génération
L'algorithme initialisera une nouvelle génération avec de nouvelles données jusqu'à tant qu'au moins une solution viable sera trouvée.
- Un tri de la population est effectué selon le fitness, l'individu le plus performant sera au début du vecteur.
- Selon le nombre d'élite fourni à l'initialisation de l'algorithme :
- Un nombre X d'individus sera recopié dans la prochaine génération.

POUR COMBLER LA PROCHAINE GÉNÉRATION :

- SÉLECTION (roulette wheel) :
	- Un chiffre est choisi entre 0 et la somme des fitness calculés plus tôt pour chacun des parents.
	- Une itération est effectuée dans la population en calculant une nouvelle somme des fitness.
	- Lorsque la somme partielle est calculée et qu'elle dépasse le chiffre choisi pour parent 1 ou parent 2,      cet individu est alors choisi comme parent.
		- Cette méthode permet de choisir plus souvent un parent ayant un meilleur fitness.
	- Une vérification est effectuée pour s'assurer que les deux parents ne soient pas le même individu.
	- Si aucun parent n'est trouvé - un parent est choisi de manière aléatoire (failsafe).

- CROISEMENT :
	- Pour chaque gène, une moyenne pondérée est réalisée de manière aléatoire.
	- Ces gènes sont ensuite donnés à un enfant.

- MUTATION :
	- Calcul aléatoire pour déterminer si l'enfant aura une mutation selon le ratio fourni.
	- SI il y a une mutation, un calcul aléatoire est effectué pour déterminer le nombre de gènes à modifier.
	- Une mutation aura lieu X fois et choisira un gène Y à modifier.
	- La mutation choisira un nouveau nombre dans l'étendue initiale et l'assignera à l'enfant.
	- Un calcul du fitness de l'enfant est fait.
	- L'enfant est rajouté dans la prochaine génération.


- Une fois que la prochaine génération a atteint la taille désirée, elle remplace la génération courante    et un nouveau tri est effectué.

	

	Inspiration initiale : https://www.geeksforgeeks.org/genetic-algorithms/




*/

#pragma once

#include <Qvector>
#include "Q2DGeometryUtilities.h"
#include <array>
#include <functional>


class AlgoGen
{
public:
	using DataType = double;
	using Solution = QVector<DataType>; // les genes 
	using EtendueGenes = QVector<std::array<DataType,2>>;



	struct Individu {
		Solution solution;
		DataType fitnessValue;
	};

	using Population = QVector<Individu>;
	AlgoGen();
	AlgoGen(size_t nbGenes, EtendueGenes etendueGene, size_t population, size_t ratioMutation, size_t nbElite, std::function<DataType(Solution)>fitness);

	Population getGenerationCourante();

	bool isReady();

	void nextGen();

	
	


	

private:

	size_t mNbGenes, mPopulation, mRationMutation, mNbElite;
	EtendueGenes mEtendueGene;
	Population mProchaineGeneration{}, mGenerationCourante{};


	
	void init();
	static bool compare(const AlgoGen::Individu& a, const AlgoGen::Individu& b);
	
	static DataType random_num(DataType min, DataType max);
	static int random_int(int start, int end);
	

	std::function<DataType(Solution const&)>mFitness;  // Solution const&

};

