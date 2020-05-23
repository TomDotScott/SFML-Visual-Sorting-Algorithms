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
		if(i == _swappedIndex)
		{
			rectangle.setFillColor(sf::Color::Green);
		}
		else {
			rectangle.setFillColor(sf::Color::White);
		}
		rectangle.setPosition(static_cast<float>(i + i*size), _window.getSize().y);
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

void SelectionSort(std::vector<int>& _collection, sf::RenderWindow& _window)
{
	for(int i = 0; i < _collection.size(); ++i)
	{
		int minIndex = i;
		for(int j = i + 1; j < _collection.size(); ++j)
		{
			if(_collection[j] < _collection[minIndex])
			{
				minIndex = j;
			}

			Swap(_collection, minIndex, i);
			Render(_window, _collection, i);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}


int main() {
	srand(static_cast<unsigned>(time(nullptr)));
	sf::RenderWindow window((sf::VideoMode(960, 540)), "Visual Sorting Algorithms");

	// We can still output to the console window
	std::cout << "SnakeGame: Starting" << std::endl;

	std::vector<int> numbers;
	GenerateNumbersList(100, numbers);
	RandomiseList(numbers);




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



		SelectionSort(numbers, window);


	}

	std::cout << "SnakeGame: Finished" << std::endl;

	return 0;
}
