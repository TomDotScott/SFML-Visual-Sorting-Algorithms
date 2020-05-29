#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

//---------------------------------------------------------------------------------------------------------------
//-------------------------------------------------UTILITIES-----------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

void Swap(std::vector<int>& _collection, const size_t _indexA, const size_t _indexB) {
	const int temp = _collection[_indexA];
	_collection[_indexA] = _collection[_indexB];
	_collection[_indexB] = temp;
}

size_t Min(const size_t _left, const size_t _right) {
	return _left < _right ? _left : _right;
}

void GenerateColoursList(const int _amount, std::vector<sf::Color>& _colourList) {
	const int step = std::pow(2, ceil(log(_amount) / log(2))) / _amount;
	int r = 255;
	int g = 0;
	int b = 0;

	while (g < 255) {
		g += step;
		sf::Color c(r, g, b);
		_colourList.push_back(c);
	}


	while (r > 0) {
		r -= step;
		sf::Color c(r, g, b);
		_colourList.push_back(c);
	}



	while (b < 255) {
		b += step;
		sf::Color c(r, g, b);
		_colourList.push_back(c);
	}


	while (g > 0) {
		g -= step;
		sf::Color c(r, g, b);
		_colourList.push_back(c);
	}

	int currentIndex{ 0 };
	int counter{ 0 };
}

void GenerateNumbersList(const int _amount, std::vector<int>& _collection) {
	_collection.reserve(_amount);
	for (int i = 0; i < _amount; ++i) {
		_collection.push_back(i + 1);
	}
};

void RandomiseList(std::vector<int>& _collection) {
	for (unsigned i = 0; i < _collection.size() - 1; ++i) {
		const unsigned randomNumber = rand() % (_collection.size() - 1);
		Swap(_collection, i, randomNumber);
	}
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------RENDERING---------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

void Render(const std::vector<int>& _collection, const size_t _swappedIndex, const int _waitTime, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, std::vector<sf::Color>& _coloursList, sf::RenderWindow& _window) {
	// We must clear the window each time around the loop
	_window.clear();

	//set the text
	_comparisonsText.setString("Comparisons : " + std::to_string(_comparisonsCount));
	_window.draw(_comparisonsText);

	_arrayAccessesText.setString("Accesses : " + std::to_string(_arrayAccessesCount));
	_arrayAccessesText.setPosition({ _window.getSize().x - _arrayAccessesText.getGlobalBounds().width, 0 });

	_window.draw(_arrayAccessesText);

	const float rectangleWidth{ static_cast<float>(_window.getSize().x) / _collection.size() - 1 };

	const float indexratio = _coloursList.size() / _collection.size();

	for (unsigned i = 0; i < _collection.size(); ++i) {
		//std::cout << _collection[i] << " ";
		sf::RectangleShape rectangle({
			rectangleWidth,
			(static_cast<float>(_collection[i] + 1) / _collection.size()) * (static_cast<float>(_window.getSize().y) - 100)
			});
		rectangle.setOrigin(0, rectangle.getGlobalBounds().height);
		if (i == _swappedIndex) {
			rectangle.setFillColor(sf::Color::White);
		} else {
			if (i != 0) {
				const int position = _collection[i - 1] * indexratio;
				rectangle.setFillColor(_coloursList[position]);
			} else {
				rectangle.setFillColor(_coloursList[0]);
			}
		}
		rectangle.setPosition(static_cast<float>(i) + static_cast<float>(i) * rectangleWidth,
			static_cast<float>(_window.getSize().y));
		_window.draw(rectangle);
	}
	//std:: cout << std::endl;
	// Get the window to display its contents
	_window.display();
	std::this_thread::sleep_for(std::chrono::milliseconds(_waitTime));
}

//---------------------------------------------------------------------------------------------------------------
//--------------------------------------------------ALGORITHMS---------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

void BubbleSort(std::vector<int>& _collection, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	for (unsigned i = 0; i < _collection.size() - 1; ++i) {
		_arrayAccessesCount++;
		bool swapped = false;
		for (size_t j = 0; j < _collection.size() - i - 1; ++j) {
			_arrayAccessesCount++;
			if (_collection[j] > _collection[j + 1]) {
				_comparisonsCount++;
				Swap(_collection, static_cast<unsigned>(j), static_cast<unsigned>(j + 1));
				_arrayAccessesCount++;
				swapped = true;
				Render(_collection, j + 1, 3, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
			}
		}
		if (!swapped) {
			break;
		}
	}
	Render(_collection, 0, 2, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}

void SelectionSort(std::vector<int>& _collection, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	for (unsigned i = 0; i < _collection.size(); ++i) {
		_arrayAccessesCount++;
		auto minIndex = i;
		for (unsigned j = i + 1; j < _collection.size(); ++j) {
			_arrayAccessesCount++;
			if (_collection[j] < _collection[minIndex]) {
				_comparisonsCount++;
				minIndex = j;
			}
			Swap(_collection, minIndex, i);
			_arrayAccessesCount++;
			Render(_collection, i, 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		}
	}
	Render(_collection, 0, 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}

void Merge(std::vector<int>& _collection, const size_t _left, const size_t _middle, const size_t _right, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	const auto size1 = _middle - _left + 1;
	const auto size2 = _right - _middle;

	//create temporary vectors
	std::vector<int> left(size1);
	std::vector<int> right(size2);

	//copy data into the temporary vectors
	for (unsigned long long i = 0; i < size1; ++i) {
		_arrayAccessesCount++;
		left[i] = _collection[_left + i];
	}

	for (unsigned long long j = 0; j < size2; j++) {
		_arrayAccessesCount++;
		right[j] = _collection[_middle + 1 + j];
	}

	//merge the temporary arrays into the main array
	unsigned long long i{ 0 };
	unsigned long long j{ 0 };
	auto k{ _left };

	while (i < size1 && j < size2) {
		_arrayAccessesCount++;
		if (left[i] <= right[j]) {
			_comparisonsCount++;
			_collection[k] = left[i];
			++i;
		} else {
			_comparisonsCount++;
			_collection[k] = right[j];
			++j;
		}
		Render(_collection, k, 20, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		++k;
	}

	//copy the remaining elements of the left subarray, if there are any
	while (i < size1) {
		_collection[k] = left[i];
		_arrayAccessesCount++;
		++i;
		++k;
	}

	//copy the remaining elements of the right subarray, if there are any
	while (j < size2) {
		_arrayAccessesCount++;
		_collection[k] = right[j];
		++j;
		++k;
	}
}

void MergeSort(std::vector<int>& _collection, const size_t _left, const size_t _right, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	if (_left < _right) {
		const auto middle = (_left + _right) / 2;

		//sort the first half
		MergeSort(_collection, _left, middle, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		//sort the second half
		MergeSort(_collection, middle + 1, _right, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);

		//merge the halves
		Merge(_collection, _left, middle, _right, _comparisonsCount, _arrayAccessesCount, _arrayAccessesText, _comparisonsText, _coloursList, _window);
	}
	Render(_collection, 0, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}


void InsertionSort(std::vector<int>& _collection, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	for (int i = 1; i < _collection.size(); i++) {
		_arrayAccessesCount++;
		const int key = _collection[i];
		_arrayAccessesCount++;
		int j = i - 1;
		while (j >= 0 && _collection[j] > key) {
			_comparisonsCount++;
			_arrayAccessesCount++;
			_collection[1 + j] = _collection[j];
			j--;
			Render(_collection, key, 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		}
		_collection[j + 1] = key;
	}
	Render(_collection, 0, 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}

void InsertionSort(std::vector<int>& _collection, const int _left, const int _right, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	for (int i = _left + 1; i <= _right; i++) {
		const int temp = _collection[i];
		int j = i - 1;
		while (j >= _left && _collection[j] > temp) {
			_comparisonsCount += 2;
			_collection[j + 1] = _collection[j];
			_arrayAccessesCount += 2;
			j--;
			Render(_collection, j, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		}
		_collection[j + 1] = temp;
		_arrayAccessesCount++;
		Render(_collection, j, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}
}

int Partition(std::vector<int>& _collection, int _low, int _high, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	const int pivot = _collection[_high]; // pivot
	_arrayAccessesCount++;

	auto i = (_low - 1); // Index of smaller element  

	for (auto j = _low; j <= _high - 1; j++) {
		// If current element is smaller than the pivot  
		if (_collection[j] < pivot) {
			_arrayAccessesCount++;
			_comparisonsCount++;

			i++; // increment index of smaller element

			_arrayAccessesCount++;
			Swap(_collection, i, j);

			Render(_collection, j, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		}
	}
	Swap(_collection, i + 1, _high);
	_arrayAccessesCount++;
	Render(_collection, _high, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	return (i + 1);
}

void QuickSort(std::vector<int>& _collection, size_t _low, size_t _high, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	if (_low < _high) {
		const auto partitionIndex = Partition(_collection, _low, _high, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);

		//seperately sort elements before and after the partition
		QuickSort(_collection, _low, partitionIndex - 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		QuickSort(_collection, partitionIndex + 1, _high, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}
	Render(_collection, 0, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}

void Heapify(std::vector<int>& _collection, const size_t _size, const size_t _index, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	auto largest = _index; // Initialize largest as root 
	const auto left = 2 * _index + 1; // left = 2*i + 1 
	const auto right = 2 * _index + 2; // right = 2*i + 2 

	// If left child is larger than root 
	if (left < _size && _collection[left] > _collection[largest]) {
		_arrayAccessesCount += 2;
		_comparisonsCount += 2;
		largest = left;
	}

	// If right child is larger than largest so far 
	if (right < _size && _collection[right] > _collection[largest]) {
		_arrayAccessesCount += 2;
		_comparisonsCount += 2;
		largest = right;
	}

	// If largest is not root 
	if (largest != _index) {
		_comparisonsCount++;
		Swap(_collection, _index, largest);
		_arrayAccessesCount++;
		Render(_collection, largest, 3, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);

		// Recursively heapify the affected sub-tree 
		Heapify(_collection, _size, largest, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}
}

void HeapSort(std::vector<int>& _collection, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	// Build heap (rearrange array) 
	for (int i = static_cast<int>(_collection.size()) / 2 - 1; i >= 0; i--) {
		Heapify(_collection, _collection.size(), i, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}
	// One by one extract an element from heap 
	for (int i = static_cast<int>(_collection.size()) - 1; i >= 0; i--) {
		// Move current root to end 
		Swap(_collection, 0, i);
		_arrayAccessesCount++;
		Render(_collection, i, 3, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);

		// call max heapify on the reduced heap 
		Heapify(_collection, i, 0, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}

	Render(_collection, 0, 3, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}

void ShellSort(std::vector<int>& _collection, const size_t _size, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	// Start with a big gap, then reduce the gap 
	for (auto gap = _size / 2; gap > 0; gap /= 2) {
		// Do a gapped insertion sort for this gap size. 
		for (auto i = gap; i < _size; i += 1) {
			// add arr[i] to the elements that have been gap sorted 
			// save arr[i] in temp and make a hole at position i 
			const int temp = _collection[i];
			_arrayAccessesCount++;
			Render(_collection, i, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);

			// shift earlier gap-sorted elements up until the correct location for arr[i] is found 
			size_t j;
			for (j = i; j >= gap && _collection[j - gap] > temp; j -= gap) {
				_comparisonsCount++;
				_arrayAccessesCount += 3;
				_collection[j] = _collection[j - gap];
			}

			// put temp (the original arr[i]) in its correct location 
			_collection[j] = temp;
			_arrayAccessesCount++;
		}
		Render(_collection, gap, 5, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}
	Render(_collection, 0, 0, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}


int GetNextGap(int _gap) {
	// Shrink gap by Shrink factor 
	_gap = (_gap * 10) / 13;

	if (_gap < 1) {
		return 1;
	}
	return _gap;
}

// Function to sort a[0..n-1] using Comb Sort 
void CombSort(std::vector<int>& _collection, const size_t _size, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText,
	std::vector<sf::Color>& _coloursList,
	sf::RenderWindow& _window) {
	// Initialize gap 
	auto gap = _size;

	// Initialize swapped as true to make sure that 
	// loop runs 
	bool swapped = true;

	// Keep running while gap is more than 1 and last 
	// iteration caused a swap 
	while (gap != 1 || swapped) {
		// Find next gap 
		gap = GetNextGap(static_cast<int>(gap));

		// Initialize swapped as false so that we can 
		// check if swap happened or not 
		swapped = false;

		// Compare all elements with current gap 
		for (unsigned i = 0; i < _size - gap; i++) {
			if (_collection[i] > _collection[i + gap]) {
				_comparisonsCount++;
				Swap(_collection, i, i + gap);
				_arrayAccessesCount += 3;
				swapped = true;

				Render(_collection, i + gap, 10, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
			}
		}
	}
}

void TimSort(std::vector<int>& _collection, const size_t _size, const size_t _runSize, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, std::vector<sf::Color>& _coloursList, sf::RenderWindow& _window) {
	// Sort individual subarrays of size RUN 
	for (size_t i = 0; i < _size; i += _runSize) {
		InsertionSort(_collection, i, Min(i + _runSize - 1, _size - 1), _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}

	// start merging from size RUN (or 32). It will merge 
	// to form size 64, then 128, 256 and so on .... 
	for (auto size = _runSize; size < _size; size = 2 * size) {
		// pick starting point of left sub array. We 
		// are going to merge arr[left..left+size-1] 
		// and arr[left+size, left+2*size-1] 
		// After every merge, we increase left by 2*size 
		for (size_t left = 0; left < _size; left += 2 * size) {
			// find ending point of left sub array 
			// mid+1 is starting point of right sub array 
			const auto mid = left + size - 1;
			const auto right = Min((left + 2 * size - 1), (_size - 1));

			// merge sub array arr[left.....mid] & 
			// arr[mid+1....right] 
			Merge(_collection, left, mid, right, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
		}
	}
	Render(_collection, 0, 0, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}

void SlowSort(std::vector<int>& _collection, const int _left, const int _right, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, std::vector<sf::Color>& _coloursList, sf::RenderWindow& _window) {
	if (_left >= _right) {
		_comparisonsCount++;
		return;
	}
	const int m = _left + (_right - _left) / 2;
	SlowSort(_collection, _left, m, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	SlowSort(_collection, m + 1, _right, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	if (_collection[_right] < _collection[m]) {
		_comparisonsCount++;
		_arrayAccessesCount += 2;
		Swap(_collection, m, _right);
		_arrayAccessesCount += 2;
		Render(_collection, m, 3, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
	}
	SlowSort(_collection, _left, _right - 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _coloursList, _window);
}

int main() {
	sf::Font font;
	font.loadFromFile("font.ttf");

	sf::Text comparisonsText("Comparisons: 0", font);
	sf::Text arrayAccessesText("Array Accesses: 0", font);

	int height, width;

	srand(static_cast<unsigned>(time(nullptr)));
	int choice;
	std::cout << "WELCOME TO THE AMAZING SORTING ALGORITHMS VISUALISER" << std::endl;

	std::cout << "Enter your desired window width" << std::endl;
	std::cin >> width;

	std::cout << "Enter your desired window height" << std::endl;
	std::cin >> height;


	do {
		//choose amount of items
		std::cout << "How many items would you like to compare?" << std::endl;
		int amount{ 0 };
		std::cin >> amount;

		std::vector<int> numbers;
		GenerateNumbersList(amount, numbers);
		RandomiseList(numbers);
		std::vector<sf::Color> coloursList;

		GenerateColoursList(numbers.size(), coloursList);

		std::cout <<
			"Which algorithm would you like to see?\n1 - Bubble sort\n2 - Selection Sort\n3 - Insertion Sort\n4 - Merge Sort\n5 - QuickSort\n6 - Heap Sort\n7 - Shell Sort\n8 - Comb Sort\n9 - Tim Sort\n10 - Slow Sort"
			<< std::endl;
		std::cin >> choice;


		sf::RenderWindow window((sf::VideoMode(width, height)), "The Amazing Sorting Algorithms Visualiser");
		comparisonsText.setPosition(0, 0);

		bool complete{ false };
		int comparisons{ 0 };
		int arrayAccesses{ 0 };

		// Main loop that continues until we call window.close()
		while (window.isOpen()) {
			// Handle any pending SFML events
			// These cover keyboard, mouse,joystick etc.
			sf::Event e{};
			while (window.pollEvent(e)) {
				switch (e.type) {
				case sf::Event::Closed:
					window.close();
					break;
				default:
					break;
				}
			}

			if (!complete) {
				switch (choice) {
				case 1:
					std::cout << "Alright, Bubble Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Bubble Sort");
					BubbleSort(numbers, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 2:
					std::cout << "Alright, Selection Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Selection Sort");
					SelectionSort(numbers, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 3:
					std::cout << "Alright, Insertion Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Insertion Sort");
					InsertionSort(numbers, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 4:
					std::cout << "Alright, Merge Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Merge Sort");
					MergeSort(numbers, 0, numbers.size() - 1, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 5:
					std::cout << "Alright, Quick Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Quick Sort");
					QuickSort(numbers, 0, numbers.size() - 1, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 6:
					std::cout << "Alright, Heap Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Heap Sort");
					HeapSort(numbers, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 7:
					std::cout << "Alright, Shell Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Shell Sort");
					ShellSort(numbers, numbers.size() - 1, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 8:
					std::cout << "Alright, Comb Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Comb Sort");
					CombSort(numbers, numbers.size() - 1, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 9:
					std::cout << "Alright, Tim Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Tim Sort");
					TimSort(numbers, numbers.size() - 1, 32, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				case 10:
					std::cout << "If you have to.... Slow Sort it is...." << std::endl;
					window.setTitle("The Amazing Sorting Algorithms Visualiser - Slow Sort");
					SlowSort(numbers, 0, numbers.size() - 1, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, coloursList, window);
					break;
				default:
					std::cout << "We don't... We don't do that here..." << std::endl;
				}
				complete = true;
			}

			std::this_thread::sleep_for(std::chrono::seconds(5));
			window.close();
		}

		std::cout << "Would you like to go again? 1 for yes, 2 for no" << std::endl;
		std::cin >> choice;
	} while (choice == 1);

	return 0;
}
