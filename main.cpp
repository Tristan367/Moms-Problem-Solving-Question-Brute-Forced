#include <iostream>
#include <unordered_set>
#include <vector>

std::vector<std::vector<int>> correctCombinations; // list which holds the correct solutions

bool next_combination(int item[], int n, int N) { // I yoinked this function from stackoverflow https://stackoverflow.com/questions/5076695/how-can-i-iterate-through-every-possible-combination-of-n-playing-cards
	for (int i = 1; i <= n; ++i) {
		if (item[n - i] < N - i) {
			++item[n - i];
			for (int j = n - i + 1; j < n; ++j) {
				item[j] = item[j - 1] + 1;
			}
			return true;
		}
	}
	return false;
}

void checkIfCorrectAndAddToCorrectCombinations(int* indices) {	// each row, column, and diagonal must have an even number of crosses

	std::unordered_set<int> indicesHash; // making a hash version of "indices" so we can efficiently check if it contains certain numbers (without having to loop through "indices" and compare each item to find out)
	indicesHash.reserve(10);
	for (int i = 0; i < 10; i++) {
		indicesHash.insert(indices[i]);
	}

	int rowCounts[4] = { 0,0,0,0 }; // to hold the cross count of each row
	int columnCounts[4] = { 0,0,0,0 };
	int diagonalCounts[2] = { 0,0 };

	// rows and columns
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			const int i = y * 4 + x;
			if (indicesHash.find(i) != indicesHash.end()) { // if this square is a cross (if "indices" has "i" in it)
				rowCounts[y]++;
				columnCounts[x]++;
			}
		}
	}
	// diagonals
	for (int d = 0; d < 4; d++) {
		const int i = d * 4 + d; // bottom left to top right
		if (indicesHash.find(i) != indicesHash.end()) {
			diagonalCounts[0]++;
		}
		const int j = (3 - d) * 4 + d; // bottom right to top left
		if (indicesHash.find(j) != indicesHash.end()) {
			diagonalCounts[1]++;
		}
	}

	// checking correctness
	for (int i = 0; i < 4; i++) {
		if (rowCounts[i] % 2 != 0) { return; } // skipping the rest of the function if any cross count is odd. (% is called the "modulo" operator, it does the same thing as the "mod" button on a calculator)
		if (columnCounts[i] % 2 != 0) { return; }
	}
	if (diagonalCounts[0] % 2 != 0) { return; }
	if (diagonalCounts[1] % 2 != 0) { return; }

	// adding the combo to "correctCombinations" if it is correct
	std::vector<int> newVec;
	for (int i = 0; i < 10; i++) { newVec.push_back(indices[i]); }
	correctCombinations.push_back(newVec);
}

void getAllCombinations() { // looks at all combinations of 10 unique numbers, 0-15, the 16 indices in a 4x4 grid
	
	int currentCombination[10];
	for (int i = 0; i < 10; ++i) { currentCombination[i] = i; } // initializing array to 0-9, the first combination
	checkIfCorrectAndAddToCorrectCombinations(currentCombination);

	int count = 1;
	bool moreCombinationsLeft = true;
	while (moreCombinationsLeft) { // loop until there are no more combinations to check
		moreCombinationsLeft = next_combination(currentCombination, 10, 15);
		checkIfCorrectAndAddToCorrectCombinations(currentCombination); // adds "currentCombination" to "correctCombinations" if correct
		count++;
	}

	std::cout << "total possible combinations: " << count << '\n';
	std::cout << "correct combinations: " << correctCombinations.size() << "\n\n";
}

void displayCombinations() {
	for (const auto& c : correctCombinations) {
		std::unordered_set<int> indicesHash;
		indicesHash.reserve(10);
		for (int i = 0; i < 10; i++) {
			indicesHash.insert(c[i]);
		}
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				const int i = y * 4 + x;
				if (indicesHash.find(i) != indicesHash.end()) {
					std::cout << "X";
				}
				else {
					std::cout << "O";
				}
			}
			std::cout << '\n';
		}
		std::cout << "\n\n";
	}
}

int main() {
	getAllCombinations();
	displayCombinations();
}