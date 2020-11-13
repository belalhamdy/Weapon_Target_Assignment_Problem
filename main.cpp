#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

mt19937 g_RNG(chrono::steady_clock::now().time_since_epoch().count());

/*
 * Generate Random Chromosomes (Done)
 * Calculate fitness foreach (Done)
 * pick the parents to mate
 * crossover (Done)
 * mutate (Done)
 */
class Chromosome {
public:
    vector<int> data;
    int maxValue = 0;

    Chromosome() = default;

    Chromosome(int nSize, int maxValue) : maxValue(maxValue), data(nSize) {
        uniform_int_distribution<int> uid(0, maxValue);
        for (int &v : data)
            v = uid(g_RNG);
    }

    void mutate() {
        uniform_int_distribution<int> mutateIdx(0, data.size() - 1);
        uniform_int_distribution<int> mutateValue(0, maxValue);
        data[mutateIdx(g_RNG)] = mutateValue(g_RNG);
    }

    [[nodiscard]] pair<Chromosome, Chromosome> combineWith(const Chromosome &other) const {
        Chromosome ret1 = *this;
        Chromosome ret2 = other;
        uniform_int_distribution<int> mutateOffset(0, data.size());
        int off = mutateOffset(g_RNG);
        for (int i = 0; i < off; ++i) {
            ret1.data[i] = other.data[i];
            ret2.data[i] = this->data[i];
        }
        return {ret1, ret2};
    }

};

class GeneticAlgorithm {
    const vector<int> &weaponInstances;
    const vector<vector<double>> &success;
    const vector<int> &threat;
    vector<Chromosome> population;

    Chromosome bestChromosome;
    double bestFitness = INT_MAX;

    double getFitness(const Chromosome &ch) {
        vector<double> survive;
        survive.assign(threat.size(), 1.0);
        for (int x = 0; x < weaponInstances.size(); ++x) {
            int i = weaponInstances[x];
            int j = ch.data[x];
            survive[j] *= 1.0 - success[i][j];
        }
        double ans = 0;
        for (int i = 0; i < survive.size(); ++i) {
            ans += survive[i] * threat[i];
        }
        return ans;
    }

    vector<double> getChromosomesFitness() {
        vector<double> fitness;
        for (auto &i : population) fitness.push_back(getFitness(i));
        return fitness;
    }

    void nextGeneration() {
        // Construct probability for every chromosome
        vector<double> chromosomesFitness = getChromosomesFitness();
        vector<double> probs;
        double totalFitness = 0.0;

        for (auto &i : chromosomesFitness) totalFitness += i;
        probs.reserve(chromosomesFitness.size());
        for (auto &i : chromosomesFitness) probs.push_back(i /= totalFitness);

        // Roulette wheel construction
        discrete_distribution<> rouletteWheel(probs.begin(), probs.end());

        // select 2 parents
        int firstParentIdx = rouletteWheel(g_RNG), secondParentIdx = rouletteWheel(g_RNG);
        Chromosome firstParent = population[0];
        Chromosome secondParent = population[1];

        // get the children
        pair<Chromosome, Chromosome> offSprings = firstParent.combineWith(secondParent);

        // remove the parents from the population
        population.erase(population.begin() + max(firstParentIdx, secondParentIdx));
        population.erase(population.begin() + min(firstParentIdx, secondParentIdx));

        // mutate children
        offSprings.first.mutate();
        offSprings.second.mutate();

        // add children to population
        population.push_back(offSprings.first);
        population.push_back(offSprings.second);

        // check if any of the children fitness is the current best in the population
        double firstFitness = getFitness(offSprings.first);
        double secondFitness = getFitness(offSprings.second);

        if (firstFitness < bestFitness) {
            bestFitness = firstFitness;
            bestChromosome = offSprings.first;
        }
        if (secondFitness < bestFitness) {
            bestFitness = secondFitness;
            bestChromosome = offSprings.second;
        }

    }

public:
    GeneticAlgorithm(const vector<int> &wi,
                     const vector<int> &th,
                     const vector<vector<double>> &su, int populationSize)
            : weaponInstances(wi), threat(th), success(su) {
        for (int i = 0; i < populationSize; ++i) {
            Chromosome curr(wi.size(), su[0].size());
            population.push_back(curr);
        }

        for (const auto &i : population) {
            double currFitness = getFitness(i);
            if (currFitness < bestFitness) {
                bestChromosome = i;
                bestFitness = currFitness;
            }
        }
    }

    pair<Chromosome, double> execute(int epochs) {
        while (epochs--) {
            nextGeneration();
        }
        return {bestChromosome, bestFitness};
    }


};

int main() {
    cout << "Enter the weapon types and the number of instances of each type: (Enter 'x' when you're done)" << endl;
    vector<string> weapons;         //nWeapons
    vector<int> weaponCount;
    vector<int> weaponInstances;    //nWeaponInstances

    while (true) {
        string name;
        int count;
        cin >> name;
        if (name == "x") break;
        weapons.push_back(name);
        cin >> count;
        weaponCount.push_back(count);
        while (count--)
            weaponInstances.push_back(weapons.size() - 1);
    }

    cout << "Enter the number of targets:" << endl;
    int nTargets;
    cin >> nTargets;

    cout << "Enter the threat coefficient of each target:" << endl;
    vector<int> threat(nTargets);
    for (int &cur : threat)
        cin >> cur;

    cout << "Enter the weapons' success probabilities matrix:" << endl;
    vector<vector<double>> success(weapons.size(), vector<double>(nTargets));
    for (int i = 0; i < weapons.size(); ++i)
        for (int j = 0; j < nTargets; ++j)
            cin >> success[i][j];

    cout << "Please wait while running the GA..." << endl;

    //Do processing here
    int populationSize = 20;
    GeneticAlgorithm algorithmModel(weaponInstances, threat, success, populationSize);

    int epochs = 1000;
    pair<Chromosome, double> solution = algorithmModel.execute(epochs);
    // chromosome solution is nWeaponInstances size

    cout << "The final WTA solution is:" << endl;
    vector<int> solutionData = solution.first.data;
    vector<int> weaponCountTemp(weaponCount);

    for (int i = 0; i < solutionData.size(); ++i) {
        int currWeaponInstance = weaponInstances[i];
        cout << weapons[currWeaponInstance] << " " << "#" <<
             weaponCount[currWeaponInstance] - --weaponCountTemp[currWeaponInstance] <<
             " is assigned to target #" << solutionData[i] + 1 << ",\n";
    }
    cout << "The expected total threat of the surviving targets is " << solution.second << "." << endl;


    return 0;

}
/* Input:
Tank 2
Aircraft 1
Grenade 2
x
3
16
5
10
0.3 0.6 0.5
0.4 0.5 0.4
0.1 0.2 0.2
 */
/* Best Solution Generated by this code:
The final WTA solution is:
Tank #1 is assigned to target #2,
Tank #2 is assigned to target #3,
Aircraft #1 is assigned to target #1,
Grenade #1 is assigned to target #3,
Grenade #2 is assigned to target #1,
The expected total threat of the surviving targets is 14.64.
 */

