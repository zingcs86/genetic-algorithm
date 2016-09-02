/*
* Name: Genetic algorithm for 0-1 Knapsack problem
* Author: Lee-Tse, TING
* @Usage: g++ -out genetic genetic.cpp
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#define lengthOfGene 20 	// Length of gene.
#define popNum 30			// Number of population
#define elitismNum 10		// Selected number of elites
#define generationNum 100	// Number of generations that process runs
#define mutationRate 0.2	// Mutation rate
#define crossoverRate 0.8	// Crossover rate
#define limit 300			// Limit
#define _linux

#if defined(_linux) 
	unsigned int seed = time(NULL);
	#define RND ((double) rand_r(&seed) / RAND_MAX)
#endif 

// The name of items.
char name[20][100] = { 
		"#0ポッキー", "#1うまい棒", "#2じゃがりこ", "#3ベビースターラーメン", "#4チロルチョコ", "#5かっぱえびせん",
		"#6サッポロポテト", "#7都こんぶ", "#8ヨーグレットハイレモン", "#9おにぎりせんべい", "#10コアラのマーチ", 
		"#11チップスター", "#12梅干しグミ", "#13トッポ", "#14アポロ", "#15ハイレモン", 
		"#16さいころキャラメル", "#17シゲキックス", "#18ハイチュウ", "#19ばなな" };

// Price represents value.
int price[20] = { 
		168, 10, 145, 60, 10,
		124, 124, 105, 126, 184,
		186, 135, 89, 98, 62,
		132, 86, 125, 126, 0 };

// Calorie represents weight.
int calorie[20] = { 
		496, 45, 325, 347, 61,
		486, 446, 22, 110, 475,
		520, 325, 221, 322, 152,
		216, 322, 105, 221, 210 };

class chromo{
private:
	bool *gene; // The gene.
	double fitness;
public:
	// Constructor
	chromo(bool b = 0) { 
		gene = new bool[lengthOfGene]; 
		for (int i = 0; i < lengthOfGene; i++) gene[i] = b;
		calculateFitness();
	}
	chromo(const chromo& c): fitness(c.fitness) {
		gene = new bool[lengthOfGene];
		for (int i = 0; i < lengthOfGene; i++) 
			gene[i] = c.gene[i];
		calculateFitness();
	}
	~chromo() { delete[] gene; }
	
	const double& getFitness() const{ return fitness; }

	bool& operator[] (const int& index) {
		if (index < 0) return gene[0];
		else if (index >= lengthOfGene) return gene[lengthOfGene - 1];
		else return gene[index];
	}

	chromo& operator= (const chromo& c) {
		for (int i = 0; i < lengthOfGene; i++) 
			gene[i] = c.gene[i];
		fitness = c.fitness;

		return *this;
	}  

	chromo& mutate() {
		for (int i = 0; i < lengthOfGene; i++) {
			if (RND <= mutationRate)
				gene[i] = !gene[i];
		}
		return *this;
	}

	chromo& crossover(chromo& father, chromo& mother, int pivot) {
		for (int i = 0; i < lengthOfGene; i++) {
			gene[i] = i < pivot ? father[i] : mother[i];
		} // i
		return *this;
	}

	chromo& calculateFitness() {
		int totalCalories = 0;
		fitness = 0;

		for (int i = 0; i < lengthOfGene; i++) {
			totalCalories += gene[i] * calorie[i];
			fitness += gene[i] * price[i];
		} // i

		if (totalCalories > limit) fitness -= 7 * (totalCalories - limit);
		return *this;
	}
};

// Comparison
bool cmpValueOfWeight(const pair<int, double>& p1, const pair<int, double>& p2) { return p1.second > p2.second; }
bool cmpFitness(const chromo& c1, const chromo& c2) { return c1.getFitness() > c2.getFitness(); }

class generation{
private:
	chromo *population;
public:
	generation() { population = new chromo[popNum]; }
	~generation() { delete[] population; }
	
	chromo& operator[] (const int& index) {
		if (index < 0) return population[0];
		else if (index >= popNum) return population[popNum - 1];
		else return population[index];
	}

	generation& initializesChromosomes() {
		vector< pair<int, double> > valueOfweight(lengthOfGene);
		chromo intialChromo;

		// Initializes the chromosomes with the results of a greedy algorithm.
		for (int i = 0; i < lengthOfGene; i++) {
			valueOfweight.push_back(pair<int, double>(make_pair(i, (double) price[i] / calorie[i])));
		} // i

		// Sort first.
		std::sort(valueOfweight.begin(), valueOfweight.end(), cmpValueOfWeight);

		// Pick the topest items until beyond the limit.
		int tempCalorie = 0, index;
		for (int i = 0; i < lengthOfGene; i++) {
			index = valueOfweight[i].first;
			if (tempCalorie + calorie[index] <= limit) { // Greedy
				tempCalorie += calorie[index];
				intialChromo[index] = 1;
			}
		} // i

		intialChromo.calculateFitness();
		for (int i = 0; i < popNum; i++) {
			population[i] = intialChromo;
		} // i

		return *this;
	}

	void sort(int begin, int end) { std::sort(population + begin, population + end, cmpFitness); }
	generation& selectChromosomes() {
		this->sort(0, popNum);

		for (int i = elitismNum; i < popNum; i++) {
			
			if (RND <= crossoverRate) {

				for (int j = 0; j < elitismNum; j++) {
					int indexFathter = floor(RND * elitismNum);
					// cout << "father: " << indexFathter << endl;
					int indexMother = floor(RND * elitismNum);
					// cout << "mother: " << indexMother << endl;
					int pivot = round(RND * lengthOfGene);
					// cout << "pivot: " << pivot << endl;
					population[i].crossover(population[indexFathter], population[indexMother], pivot).calculateFitness();
				} // j
			} else { 
				population[i].mutate().calculateFitness();
			}
			
		} // i

		return *this;
	}	
};

int main() {

	generation generations;
	generations.initializesChromosomes();

	int indexOfMax;
  	for (int i = 1; i <= generationNum; i++) {
  		double max = -numeric_limits<float>::max(), min = numeric_limits<float>::max();
    	double sum = 0.0;

    	generations.selectChromosomes();

 		for (int j = 0; j < popNum; j++) {
 			sum += generations[j].getFitness();
 			if (max < generations[j].getFitness()) max = generations[j].getFitness(), indexOfMax = j;
 			if (min > generations[j].getFitness()) min = generations[j].getFitness();
 		} // j

 		if (i % 5 == 0) {
 			cout << "generation#" << i << ": " << endl;
			cout << "max: " << max << " min: " << min << endl;
			cout << "average: " << (double) sum / popNum << endl; 
			cout << "----------------------------------" << endl;	
 		}
  	}

  	int weightOfBest = 0;
  	int valueOfBest = 0;
  	cout << "The Best Chromosome: ";
  	for (int i = 0; i < lengthOfGene; i++) {
  		cout << generations[indexOfMax][i];
  		weightOfBest += generations[indexOfMax][i] * calorie[i];
  		valueOfBest += generations[indexOfMax][i] * price[i];
  	}
  	cout << endl;
  	for (int i = 0; i < lengthOfGene; i++) {
  		if (generations[indexOfMax][i]) cout << name[i] << endl;
  	}
  	cout << endl;
  	cout << "limit: " << limit << endl;
  	cout << "calorie: " << weightOfBest << endl;
  	cout << "price: " << valueOfBest << endl;

	return 0;
}