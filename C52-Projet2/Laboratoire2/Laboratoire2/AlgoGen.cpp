#include "AlgoGen.h"
#include <time.h>
#include <algorithm>
#include <random>
#include <vector>


AlgoGen::AlgoGen()
	: AlgoGen(0, EtendueGenes(), 0, 0, 0, nullptr)
{
}

AlgoGen::AlgoGen(size_t nbGenes, EtendueGenes etendueGene, size_t population, size_t ratioMutation, size_t nbElite, std::function<DataType(Solution)>fitness)
	: mNbGenes{ nbGenes },
	mEtendueGene{ etendueGene },
	mPopulation{ std::clamp(population, size_t{100}, 1000ull) },
	mRationMutation{ ratioMutation },
	mNbElite{ nbElite },
	mFitness{ fitness },
	mGenerationCourante{ }
{
	init();
}

AlgoGen::Population AlgoGen::getGenerationCourante()
{
	return mGenerationCourante;
}

bool AlgoGen::isReady() {

	// vérifie que la fonction fitness a été bien fournie à l'initialisation avant d'initialiser une population.

	bool rep = true;

	if (mFitness == nullptr)
		rep = false;

	return rep;
}

void AlgoGen::init()
{
	if (isReady()) {

		mGenerationCourante.resize(mPopulation);

		for (auto& individu : mGenerationCourante)
		{
			individu.solution.resize(mNbGenes);

			for (int i = 0; i < mNbGenes; ++i) {
				individu.solution[i] = random_num(mEtendueGene[i][0], mEtendueGene[i][1]);
			}
			individu.fitnessValue = mFitness(individu.solution);

		}

	}

}

bool AlgoGen::compare(const AlgoGen::Individu& a, const AlgoGen::Individu& b)
{
	return a.fitnessValue > b.fitnessValue;
}






void AlgoGen::nextGen()
{
	srand((unsigned)(time(0)));

	




	mProchaineGeneration.resize(mPopulation);


	



	double sommeFitness{};

	sommeFitness == 0;
	// Somme des fitness de la population pour choix parent ET s'assurer qu'une solution viable existe au départ.
	while (sommeFitness == 0) {
		for (auto& individu : mGenerationCourante) {
			if (individu.fitnessValue > 0.0) {
				sommeFitness += individu.fitnessValue;
			}
		}
		// Si aucune solution valable, générer une nouvelle population!
		if (sommeFitness == 0)
			init();
	}

	
	std::sort(mGenerationCourante.begin(), mGenerationCourante.end(), compare);
	// Selon le fitness, copier les meilleurs X parents dans la prochaine generation
	for (int i{}; i < mNbElite; ++i) {
		mProchaineGeneration[i] = mGenerationCourante[i];
	}

	// Calcul en prévision du choix des parents
	bool parent1Trouve, parent2Trouve;


	for (size_t i{ mNbElite }; i < mPopulation; i++)
	{
		int choixParent1 = random_num(0, sommeFitness);
		int choixParent2 = random_num(0, sommeFitness);

		int sommePartielle{};
		parent1Trouve = parent2Trouve = false;

		Individu parent1{}, parent2{};

		// Sélection
		// Choix des parents selon le poids de leur fitness relatif à la population
		for (auto& individu : mGenerationCourante) {
			sommePartielle += individu.fitnessValue;

			if (sommePartielle >= choixParent1 && !parent1Trouve) {
				parent1 = individu;
				parent1Trouve = true;
			}
			if (sommePartielle >= choixParent2 && !parent2Trouve) {
				parent2 = individu;
				if (parent2.fitnessValue != parent1.fitnessValue)
					parent2Trouve = true;
			}


		}


		// Si roulette ne trouve pas de parents différents, prendre un parent aléatoire (arrive rarement)
		if (!parent2Trouve)
			parent2 = mGenerationCourante[random_num(0,mGenerationCourante.size()-1)];

		if (!parent1Trouve)
			parent1 = mGenerationCourante[random_num(0, mGenerationCourante.size() - 1)];



		//Croisement
		Individu enfant{};
		DataType r{}, gene{}, mutation{};
		enfant.solution.resize(mNbGenes);

		for (int i = 0; i < mNbGenes; ++i)
		{
			int temp = random_num(0, 100);
			r = (DataType)temp / 100.0;
			gene = r * parent1.solution[i] + (1 - r) * parent2.solution[i];
			enfant.solution[i] = gene;
		}

		// Mutation
		if (random_num(0, 100) < mRationMutation) {
			int nbGeneAModif = random_num(1, enfant.solution.size()+1);
			for (int i{}; i < nbGeneAModif; ++i) {
				int geneAModif = random_int(0, enfant.solution.size()-1);	
				if (geneAModif == 3)
					int arret = 3;
				mutation = random_num(mEtendueGene[geneAModif][0], mEtendueGene[geneAModif][1]);
				enfant.solution[geneAModif] = mutation;
			}


		}

		enfant.fitnessValue = mFitness(enfant.solution);

		mProchaineGeneration[i] = enfant;


	}


	mGenerationCourante = mProchaineGeneration;



	std::sort(mGenerationCourante.begin(), mGenerationCourante.end(), compare);
}



int AlgoGen::random_int(int start, int end)
{
	int range = (end - start) + 1;
	int random_int = start + (rand() % range);
	return random_int;
}



AlgoGen::DataType AlgoGen::random_num(DataType min, DataType max) {
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}



