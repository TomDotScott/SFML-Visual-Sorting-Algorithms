#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>


void Render(const std::vector<int>& _collection, const unsigned _swappedIndex, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	// We must clear the window each time around the loop
	_window.clear();

	//set the text
	_comparisonsText.setString("Comparisons : " + std::to_string(_comparisonsCount));
	_window.draw(_comparisonsText);

	_arrayAccessesText.setString("Accesses : " + std::to_string(_arrayAccessesCount));
	_arrayAccessesText.setPosition({ _window.getSize().x - _arrayAccessesText.getGlobalBounds().width, 0 });

	_window.draw(_arrayAccessesText);

	const float rectangleWidth{ static_cast<float>(_window.getSize().x) / _collection.size() - 1 };
	for (unsigned i = 0; i < _collection.size(); ++i) {
		//std::cout << _collection[i] << " ";
		sf::RectangleShape rectangle({ rectangleWidth, (static_cast<float>(_collection[i] + 1) / _collection.size()) * (_window.getSize().y - 100) });
		rectangle.setOrigin(0, rectangle.getGlobalBounds().height);
		if (i == _swappedIndex) {
			rectangle.setFillColor(sf::Color::Green);
		} else {
			rectangle.setFillColor(sf::Color::White);
		}
		rectangle.setPosition(static_cast<float>(i + i * rectangleWidth), _window.getSize().y);
		_window.draw(rectangle);
	}
	//std:: cout << std::endl;
	// Get the window to display its contents
	_window.display();
}

void Swap(std::vector<int>& _collection, const unsigned _indexA, const unsigned _indexB) {
	const int temp = _collection[_indexA];
	_collection[_indexA] = _collection[_indexB];
	_collection[_indexB] = temp;
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

void BubbleSort(std::vector<int>& _collection, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	for (unsigned i = 0; i < _collection.size() - 1; ++i) {
		_arrayAccessesCount++;
		bool swapped = false;
		for (unsigned j = 0; j < _collection.size() - i - 1; ++j) {
			_arrayAccessesCount++;
			if (_collection[j] > _collection[j + 1]) {
				_comparisonsCount++;
				Swap(_collection, j, j + 1);
				_arrayAccessesCount++;
				swapped = true;
				Render(_collection, j + 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		}
		if (!swapped) {
			break;
		}
	}
}

void SelectionSort(std::vector<int>& _collection, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	for (int i = 0; i < _collection.size(); ++i) {
		_arrayAccessesCount++;
		int minIndex = i;
		for (int j = i + 1; j < _collection.size(); ++j) {
			_arrayAccessesCount++;
			if (_collection[j] < _collection[minIndex]) {
				_comparisonsCount++;
				minIndex = j;
			}
			Swap(_collection, minIndex, i);
			_arrayAccessesCount++;
			Render(_collection, i, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void Merge(std::vector<int>& _collection, const int _left, const int _middle, const int _right, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	const int size1 = _middle - _left + 1;
	const int size2 = _right - _middle;

	//create temporary vectors
	std::vector<int> left(size1);
	std::vector<int> right(size2);

	//copy data into the temporary vectors
	for (int i = 0; i < size1; ++i) {
		_arrayAccessesCount++;
		left[i] = _collection[_left + i];
	}

	for (int j = 0; j < size2; j++) {
		_arrayAccessesCount++;
		right[j] = _collection[_middle + 1 + j];
	}

	//merge the temporary arrays into the main array
	int i{ 0 };
	int j{ 0 };
	int k{ _left };

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
		Render(_collection, k, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
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

void MergeSort(std::vector<int>& _collection, const int _left, const int _right, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	if (_left < _right) {
		const int middle = (_left + _right) / 2;

		//sort the first half
		MergeSort(_collection, _left, middle, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
		//sort the second half
		MergeSort(_collection, middle + 1, _right, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);

		//merge the halves
		Merge(_collection, _left, middle, _right, _comparisonsCount, _arrayAccessesCount, _arrayAccessesText, _comparisonsText, _window);
	}
}


void InsertionSort(std::vector<int>& _collection, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	for (int i = 1; i < _collection.size(); i++) {
		_arrayAccessesCount++;
		int key = _collection[i];
		_arrayAccessesCount++;
		int j = i - 1;
		while (j >= 0 && _collection[j] > key) {
			_comparisonsCount++;
			_arrayAccessesCount++;
			_collection[j + 1] = _collection[j];
			j -= 1;
			Render(_collection, key, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		_collection[j + 1] = key;
	}
}

int Partition(std::vector<int>& _collection, int _low, int _high, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	const int pivot = _collection[_high]; // pivot
	_arrayAccessesCount++;
	
	int i = (_low - 1); // Index of smaller element  

	for (int j = _low; j <= _high - 1; j++) {
		// If current element is smaller than the pivot  
		if (_collection[j] < pivot) {
			_arrayAccessesCount++;
			_comparisonsCount++;
			
			i++; // increment index of smaller element

			_arrayAccessesCount++;
			Swap(_collection, i, j);

			Render(_collection, j, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
	Swap(_collection, i + 1, _high);
	_arrayAccessesCount++;
	Render(_collection, _high, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	return (i + 1);
}

void QuickSort(std::vector<int>& _collection, int _low, int _high, int& _comparisonsCount, int& _arrayAccessesCount, sf::Text& _comparisonsText, sf::Text& _arrayAccessesText, sf::RenderWindow& _window) {
	if (_low < _high) {
		const auto partitionIndex = Partition(_collection, _low, _high, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);

		//seperately sort elements before and after the partition
		QuickSort(_collection, _low, partitionIndex - 1, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
		QuickSort(_collection, partitionIndex + 1, _high, _comparisonsCount, _arrayAccessesCount, _comparisonsText, _arrayAccessesText, _window);
	}
}


int main() {
	sf::Font font;
	font.loadFromFile("font.ttf");

	sf::Text comparisonsText("Comparisons: 0", font);
	sf::Text arrayAccessesText("Array Accesses: 0", font);


	int comparisons{ 0 };
	int arrayAccesses{ 0 };

	srand(static_cast<unsigned>(time(nullptr)));

	std::cout << "WELCOME TO THE AMAZING SORTING ALGORITHMS VISUALISER" << std::endl;

	//choose amount of items
	std::cout << "How many items would you like to compare?" << std::endl;
	int amount{ 0 };
	std::cin >> amount;

	std::vector<int> numbers;
	GenerateNumbersList(amount, numbers);
	RandomiseList(numbers);


	std::cout << "Which algorithm would you like to see? Bubble sort(1), Selection Sort(2), Insertion Sort(3), Merge Sort(4) or QuickSort(5)?" << std::endl;
	int choice;
	std::cin >> choice;


	sf::RenderWindow window((sf::VideoMode(960, 540)), "Visual Sorting Algorithms");
	comparisonsText.setPosition(0, 0);

	bool complete{ false };
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
				BubbleSort(numbers, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, window);
				break;
			case 2:
				std::cout << "Alright, Selection Sort it is...." << std::endl;
				SelectionSort(numbers, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, window);
				break;
			case 3:
				std::cout << "Alright, Insertion Sort it is...." << std::endl;
				InsertionSort(numbers, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, window);
				break;
			case 4:
				std::cout << "Alright, Merge Sort it is...." << std::endl;
				MergeSort(numbers, 0, numbers.size() - 1, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, window);
				break;
			case 5:
				std::cout << "Alright, Quick Sort it is...." << std::endl;
				QuickSort(numbers, 0, numbers.size() - 1, comparisons, arrayAccesses, comparisonsText, arrayAccessesText, window);
				break;
			default:
				std::cout << "We don't... We don't do that here..." << std::endl;
			}
			complete = true;
		}
	}

	std::cout << "SnakeGame: Finished" << std::endl;

	return 0;
}
