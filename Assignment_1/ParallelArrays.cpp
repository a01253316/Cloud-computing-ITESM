#include <iostream>
#include <fstream>
#include <iomanip>   // setw, left, fixed, setprecision
#include <omp.h>

#define N 1000
#define CHUNK 100

// Reads exactly N numbers (one per line or space-separated) into array.
// Returns false if the file can't be opened or doesn't contain enough values.
bool readArrayFromFile(const std::string& filename, float* array)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    for (int i = 0; i < N; i++)
    {
        if (!(file >> array[i])) return false;
    }
    return true;
}

int main()
{
    float a[N], b[N], c[N];
    int choice;

    // -------- MENU: choose data source --------
    std::cout << "Choose how to initialize the arrays:\n";
    std::cout << "1) Generate arrays in the program\n";
    std::cout << "2) Load arrays from two text files (arrayA.txt, arrayB.txt)\n";
    std::cout << "Enter option (1 or 2): ";
    std::cin >> choice;

    if (choice == 1)
    {
        // Generate arrays programmatically
        for (int i = 0; i < N; i++)
        {
            a[i] = i * 10.0f;
            b[i] = (i + 3) * 3.7f;
        }
    }
    else if (choice == 2)
    {
        // Load arrays from text files
        if (!readArrayFromFile("arrayA.txt", a))
        {
            std::cerr << "Error: Could not read arrayA.txt (need " << N << " numbers).\n";
            return 1;
        }
        if (!readArrayFromFile("arrayB.txt", b))
        {
            std::cerr << "Error: Could not read arrayB.txt (need " << N << " numbers).\n";
            return 1;
        }
    }
    else
    {
        std::cerr << "Invalid option. Please choose 1 or 2.\n";
        return 1;
    }

    // -------- PRINT TABLE HEADER --------
    std::cout << "\n"
              << std::left
              << std::setw(8)  << "Index"
              << std::setw(10) << "Thread"
              << std::setw(16) << "# Array A"
              << std::setw(16) << "# Array B"
              << std::setw(12) << "Result"
              << "\n";

    std::cout << std::string(62, '-') << "\n";

    // -------- PARALLEL SUM + TABLE ROWS --------
    // NOTE: Printing in parallel requires synchronization to avoid mixed output.
    #pragma omp parallel for schedule(static, CHUNK)
    for (int i = 0; i < N; i++)
    {
        int tid = omp_get_thread_num();
        c[i] = a[i] + b[i];

        #pragma omp critical
        {
            std::cout << std::left
                      << std::setw(8)  << i
                      << std::setw(10) << tid
                      << std::setw(16) << std::fixed << std::setprecision(2) << a[i]
                      << std::setw(16) << std::fixed << std::setprecision(2) << b[i]
                      << std::setw(12) << std::fixed << std::setprecision(2) << c[i]
                      << "\n";
        }
    }

    return 0;
}

// g++ -fopenmp  ParallelArrays.cpp -o ParallelArrays

// ./ParallelArrays  