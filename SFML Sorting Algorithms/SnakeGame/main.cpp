#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>


void Render(sf::RenderWindow& _window, const std::vector<int>& _collection, const unsigned _swappedIndex) {
	// We must clear the window each time around the loop
	_window.clear();
	const float size{ 7.5f };
	for (unsigned i = 0; i < _collection.size(); ++i) {
		sf::RectangleShape rectangle({ size, 4.f * static_cast<float>(_collection[i] + 1) });
		rectangle.setOrigin(0, rectangle.getGlobalBounds().height);
		if (i == _swappedIndex) {
			rectangle.setFillColor(sf::Color::Green);
		} else {
			rectangle.setFillColor(sf::Color::White);
		}
		rectangle.setPosition(static_cast<float>(i + i * size), _window.getSize().y);
		_window.draw(rectangle);
	}
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

void BubbleSort(std::vector<int>& _collection, sf::RenderWindow& _window) {
	for (unsigned i = 0; i < _collection.size() - 1; ++i) {
		bool swapped = false;
		for (unsigned j = 0; j < _collection.size() - i - 1; ++j) {
			if (_collection[j] > _collection[j + 1]) {
				Swap(_collection, j, j + 1);
				swapped = true;
				Render(_window, _collection, j + 1);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		}

		if (!swapped) {
			break;
		}
	}
}

void SelectionSort(std::vector<int>& _collection, sf::RenderWindow& _window) {
	for (int i = 0; i < _collection.size(); ++i) {
		int minIndex = i;
		for (int j = i + 1; j < _collection.size(); ++j) {
			if (_collection[j] < _collection[minIndex]) {
				minIndex = j;
			}

			Swap(_collection, minIndex, i);
			Render(_window, _collection, i);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void Merge(std::vector<int>& _collection, const int _left, const int _middle, const int _right, sf::RenderWindow& _window) {
	const int size1 = _middle - _left + 1;
	const int size2 = _right - _middle;

	//create temporary vectors
	std::vector<int> left(size1);
	std::vector<int> right(size2);
	
	//copy data into the temporary vectors
	for (int i = 0; i < size1; ++i) {
		left[i] = _collection[_left + i];
	}

	for (int j = 0; j < size2; j++) {
		right[j] = _collection[_middle + 1 + j];
	}

	//merge the temporary arrays into the main array
	int i{ 0 };
	int j{ 0 };
	int k{ _left };

	while(i < size1 && j < size2)
	{
		if(left[i] <= right[j])
		{
			_collection[k] = left[i];
			++i;
		}
		else
		{
			_collection[k] = right[j];
			++j;
		}
		Render(_window, _collection, k);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		++k;
	}

	//copy the remaining elements of the left subarray, if there are any
	while(i < size1)
	{
		_collection[k] = left[i];
		++i;
		++k;
	}

	//copy the remaining elements of the right subarray, if there are any
	while(j < size2)
	{
		_collection[k] = right[j];
		++j;
		++k;
	}
}

void MergeSort(std::vector<int>& _collection, const int _left, const int _right, sf::RenderWindow& _window) {
	if (_left < _right) {
		const int middle = (_left + _right) / 2;

		//sort the first half
		MergeSort(_collection, _left, middle, _window);
		//sort the second half
		MergeSort(_collection, middle + 1, _right, _window);

		//merge the halves
		Merge(_collection, _left, middle, _right, _window);
	}
}


void InsertionSort(std::vector<int>& _collection, sf::RenderWindow& _window) {
	for (int i = 1; i < _collection.size(); i++) {
		int key = _collection[i];
		int j = i - 1;
		while (j >= 0 && _collection[j] > key) {
			_collection[j + 1] = _collection[j];
			j -= 1;
			Render(_window, _collection, key);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		_collection[j + 1] = key;
	}
}


int main() {
	srand(static_cast<unsigned>(time(nullptr)));

	// We can still output to the console window
	std::cout << "SnakeGame: Starting" << std::endl;

	std::vector<int> numbers;
	GenerateNumbersList(100, numbers);
	RandomiseList(numbers);

	std::cout << "WELCOME TO THE AMAZING SORTING ALGORITHMS VISUALISER" << std::endl;
	std::cout << "Which algorithm would you like to see? Bubble sort(1), Selection Sort(2), Insertion Sort(3) or Merge Sort(4)?" << std::endl;
	int choice;
	std::cin >> choice;


	sf::RenderWindow window((sf::VideoMode(960, 540)), "Visual Sorting Algorithms");
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
				BubbleSort(numbers, window);
				break;
			case 2:
				std::cout << "Alright, Selection Sort it is...." << std::endl;
				SelectionSort(numbers, window);
				break;
			case 3:
				std::cout << "Alright, Insertion Sort it is...." << std::endl;
				InsertionSort(numbers, window);
				break;
			case 4:
				std::cout << "Alright, Merge Sort it is...." << std::endl;
				MergeSort(numbers, 0, numbers.size() - 1, window);
				break;
			}
			complete = true;
		}
	}

	std::cout << "SnakeGame: Finished" << std::endl;

	return 0;
}
