# Visual Sorting Algorithms
A visual tool to show how the sorting algorithms work using C++ and the SFML Graphics Library. Intended to use a learning tool, or just to show off and visualise the standard sorting algorithms within programming. 

This repo has 10 sorting algorithms, each in their standard implementations, tweaked to work with C++ `std::vector<int>`. The Algorithms implemented are Bubble Sort, Selection Sort, Merge Sort, Insertion Sort, Quick Sort, Heap Sort, Shell Sort, Comb Sort, Tim Sort and, for fun, the Slow Sort. 

# Visualising
This tool is very simple, it has a user-set amount of elementes in the `std::vector` that each get represented by a rectangle with a different size and hue depending on the integer value of them. The size of the window, amount of items, time delay and algorithm to use are defined by the user in a simple CLI.

<img src="http://www.tomdotscott.com/images/Github/Sorting/BubbleSort1.png">

The elements start off randomised and then get drawn into a pleasing rainbow.

<img src="http://www.tomdotscott.com/images/Github/Sorting/BubbleSort2.png">

There are text indicators in the top left and top right to show the amount of Array Accesses that are being done and the amount of comparisons that are being made. This goes to show the Big-O properties of each algorithm rather well. 

<img src="http://www.tomdotscott.com/images/Github/Sorting/HeapSort.gif">
