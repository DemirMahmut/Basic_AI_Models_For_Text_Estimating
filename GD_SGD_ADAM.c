#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WORD_LENGTH 100    // Max word length
#define MAX_WORD 70000     // Max word number
#define LEARNING_RATE 0.01 // Learning rate
#define FIRST_WEIGHT 0.02 // Starting weight value
#define ITERATIONS 200     // Number of iterations

// Function declarations
// We used clock_t to measure the execution time of the functions
clock_t gradientDescent(int **oneHotVectorA, int **oneHotVectorB, double w[30000], int wordNumber); // Gradient descent function

clock_t stochasticGradientDescent(int **oneHotVectorA, int **oneHotVectorB, double w[30000], int wordNumber); // Stochastic gradient descent function

clock_t adam(int **oneHotVectorA, int **oneHotVectorB, double w[30000], int wordNumber); // Adam function

int main()
{
    clock_t start, end;                                            // Start and end time
    char words[WORD_LENGTH];                                       // Words in the file
    char fileName[WORD_LENGTH];                                    // File name
    int i, j, wordNumber = 0;                                      // i and j are loop variables, wordNumber is the number of words in the dictionary
    char **dictionary = (char **)calloc(MAX_WORD, sizeof(char *)); // Dictionary

    for (i = 0; i < MAX_WORD; i++) // Memory allocation for dictionary
    {
        dictionary[i] = (char *)calloc(WORD_LENGTH, sizeof(char));
    }

    for (i = 51; i <= 250; i++) // Economy news 51-150, Politics news 151-250
    {
        snprintf(fileName, sizeof(fileName), "%d.txt", i);
        FILE *dosya = fopen(fileName, "r");

        if (dosya == NULL)
        {
            fprintf(stderr, "ERROR Opening File: %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        while (fscanf(dosya, "%s", words) != EOF)
        {
            strcpy(dictionary[wordNumber], words);
            wordNumber++;
        }
        printf("%s--Number of Words:%d\n", fileName, wordNumber);
        fclose(dosya);
    }

    for (i = 0; i < wordNumber; i++) // Removing duplicate words
    {
        int j;
        for (j = i + 1; j < wordNumber; j++)
        {
            if (dictionary[i][0] != '\0' && strcmp(dictionary[i], dictionary[j]) == 0)
            {
                dictionary[j][0] = '\0';
            }
        }
    }

    for (i = 0; i < wordNumber; i++) // Removing empty words
    {
        if (dictionary[i][0] == '\0')
        {
            j = i;
            while (j < wordNumber && dictionary[j][0] == '\0')
            {
                j++;
            }
            if (j < wordNumber)
            {
                strcpy(dictionary[i], dictionary[j]);
                dictionary[j][0] = '\0';
            }
            else
            {
                wordNumber = i;
            }
        }
    }

    int **oneHotVectorA = (int **)calloc(wordNumber, sizeof(int *)); // One hot vector for economy news
    for (i = 0; i < wordNumber; i++)
    {
        oneHotVectorA[i] = (int *)calloc(100, sizeof(int)); // 100 is the number of economy news
    }

    int **oneHotVectorB = (int **)calloc(wordNumber, sizeof(int *)); // One hot vector for politics news
    for (i = 0; i < wordNumber; i++)
    {
        oneHotVectorB[i] = (int *)calloc(100, sizeof(int)); // 100 is the number of politics news
    }

    for (i = 51; i <= 150; i++)
    {
        snprintf(fileName, sizeof(fileName), "%d.txt", i);
        FILE *dosya = fopen(fileName, "r");

        if (dosya == NULL)
        {
            fprintf(stderr, "ERROR Opening File: %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        while (fscanf(dosya, "%s\n", words) != EOF)
        {
            for (j = 0; j < wordNumber; j++)
            {
                if (strcmp(words, dictionary[j]) == 0)
                {
                    oneHotVectorA[j][i - 51] = 1; // For economy news 51-150, 51.txt -> 0. column, 52.txt -> 1. column, 53.txt -> 2. column...
                }
            }
        }
        fclose(dosya);
    }

    for (i = 151; i <= 250; i++)
    {
        snprintf(fileName, sizeof(fileName), "%d.txt", i);
        FILE *dosya = fopen(fileName, "r");

        if (dosya == NULL)
        {
            fprintf(stderr, "ERROR Opening File: %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        while (fscanf(dosya, "%s\n", words) != EOF)
        {
            for (j = 0; j < wordNumber; j++)
            {
                if (strcmp(words, dictionary[j]) == 0)
                {
                    oneHotVectorB[j][i - 151] = 1; // For politics news 151-250, 151.txt -> 0. column, 152.txt -> 1. column, 153.txt -> 2. column...
                }
            }
        }
        fclose(dosya);
    }

    for (i = 0; i < wordNumber; i++) // Printing dictionary
    {
        printf("%s\n", dictionary[i]);
    }

    double w[30000]; // Weight vector

    // srand(time(NULL));
    // for (i = 0; i < wordNumber; i++)
    // {
    //     w[i] = ((double)rand() / RAND_MAX) * 0.01 - 0.005; // Random weight values between -0.005 and 0.005
    // }

    for (i = 0; i < wordNumber; i++) // Starting weight values
    {
        w[i] = FIRST_WEIGHT;
    }

    double classA = 0, classB = 0; // Class A and B values

    start = clock();
    gradientDescent(oneHotVectorA, oneHotVectorB, w, wordNumber); // Gradient Descent
    end = clock();
    printf("\nGradient Descent Execution Time: %lf seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    printf("\n****************************\n"); // Printing results
    printf("Gradient Descent\n");
    classA = 0;
    classB = 0;

    for (j = 80; j < 100; j++)
    {
        classA = 0;
        for (i = 0; i < wordNumber; i++)
        {
            classA += w[i] * (double)oneHotVectorA[i][j];
        }
        printf("\nTest results for Class A[%d]:%lf,--tanh:%lf", j, classA, tanh(classA));
    }
    for (j = 80; j < 100; j++)
    {
        classB = 0;
        for (i = 0; i < wordNumber; i++)
        {
            classB += w[i] * (double)oneHotVectorB[i][j];
        }
        printf("\nTest results for Class B[%d]:%lf,--tanh:%lf", j, classB, tanh(classB));
    }

    for (i = 0; i < wordNumber; i++) // Starting weight values
    {
        w[i] = FIRST_WEIGHT;
    }

    start = clock();
    stochasticGradientDescent(oneHotVectorA, oneHotVectorB, w, wordNumber); // Stochastic Gradient Descent
    end = clock();
    printf("\nStochasticGradient Descent Execution Time: %lf seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    printf("\n****************************\n"); // Printing results
    printf("Stochastic Gradient Descent\n");
    classA = 0;
    classB = 0;

    for (j = 80; j < 100; j++)
    {
        classA = 0;
        for (i = 0; i < wordNumber; i++)
        {
            classA += w[i] * (double)oneHotVectorA[i][j];
        }
        printf("\nTest results for Class A[%d]:%lf,--tanh:%lf", j, classA, tanh(classA));
    }
    for (j = 80; j < 100; j++)
    {
        classB = 0;
        for (i = 0; i < wordNumber; i++)
        {
            classB += w[i] * (double)oneHotVectorB[i][j];
        }
        printf("\nTest results for Class B[%d]:%lf,--tanh:%lf", j, classB, tanh(classB));
    }

    for (i = 0; i < wordNumber; i++) // Starting weight values
    {
        w[i] = FIRST_WEIGHT;
    }

    start = clock();
    adam(oneHotVectorA, oneHotVectorB, w, wordNumber); // ADAM
    end = clock();
    printf("\nADAM Execution Time: %lf seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    printf("\n****************************\n"); // Printing results
    printf("*************ADAM***************\n");
    classA = 0;
    classB = 0;

    for (j = 80; j < 100; j++)
    {
        classA = 0;
        for (i = 0; i < wordNumber; i++)
        {
            classA += w[i] * oneHotVectorA[i][j];
        }
        printf("\nTest results for Class A[%d]:%lf,--tanh:%lf", j, classA, tanh(classA));
    }
    for (j = 80; j < 100; j++)
    {
        classB = 0;
        for (i = 0; i < wordNumber; i++)
        {
            classB += w[i] * oneHotVectorB[i][j];
        }
        printf("\nTest results for Class B[%d]:%lf,--tanh:%lf", j, classB, tanh(classB));
    }

    return 0;
}

clock_t gradientDescent(int **oneHotVectorA, int **oneHotVectorB, double w[30000], int wordNumber) // Gradient descent function
{
    int it = 0, i, j;
    double error = 1.0;                                // Error value
    while (it < ITERATIONS && error / 160 > 0.0001)    // 160 is the number of training data
    {
        FILE *outputFile = fopen("Gradient_Descent.txt", "w"); // Opening output file
        if (outputFile == NULL)
        {
            fprintf(stderr, "Error opening output file.\n"); // Error message
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < wordNumber; j++) // Printing weight values
        {
            fprintf(outputFile, "%lf ", w[j]);
        }
        fprintf(outputFile, "\n");

        fclose(outputFile); // Closing output file

        double gradient[30000] = {0.0}; // Gradient vector
        error = 0.0;                    // Error value

        for (i = 0; i < 80; i++) // 80 is the number of training data
        {
            double prediction = 0.0;

            for (j = 0; j < wordNumber; j++)
            {
                prediction += w[j] * (double)oneHotVectorA[j][i]; // pn = w1x1 + w2x2 + ... + wnxn
            }
            double diff = 1.0 - tanh(prediction);
            error += diff * diff; // Error = Σ(yi − pi)^2

            for (j = 0; j < wordNumber; j++)
            {
                gradient[j] += (-2 * diff) * ((1.0) - tanh(prediction) * tanh(prediction)) * oneHotVectorA[j][i]; // Gradient = Σ−2(yi − pi)(1 − tanh^2(pi))xi
            }
        }

        for (i = 0; i < 80; i++)
        {
            double prediction = 0.0;

            for (j = 0; j < wordNumber; j++)
            {
                prediction += w[j] * (double)oneHotVectorB[j][i]; // pi = w1x1 + w2x2 + ... + wnxn
            }

            double diff = (-1.0) - tanh(prediction);
            error += diff * diff; // Error = Σ(yi − pi)^2

            for (j = 0; j < wordNumber; j++)
            {
                gradient[j] += (-2 * diff) * ((1.0) - tanh(prediction) * tanh(prediction)) * oneHotVectorB[j][i]; // Gradient = Σ−2(yi − pi)(1 − tanh^2(pi))xi
            }
        }

        for (j = 0; j < wordNumber; j++)
        {
            w[j] -= LEARNING_RATE * gradient[j]; // wi = wi − αΣ−2(yi − pi)(1 − tanh^2(pi))xi
        }

        printf("%d %lf\n", it + 1, error / 160); // Printing error value

        it++; // Increasing the number of iterations
    }
}

clock_t stochasticGradientDescent(int **oneHotVectorA, int **oneHotVectorB, double w[30000], int wordNumber)
{
    int it = 0, i, j;
    double error = 1.0; // Starting error value

    while (it < ITERATIONS && error / 40 > 0.0001) // 40 is the number of training data
    {
        FILE *outputFile = fopen("SGD.txt", "w"); // Opening output file
        if (outputFile == NULL)
        {
            fprintf(stderr, "Error opening output file.\n"); // Error message
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < wordNumber; j++) // Printing weight values
        {
            fprintf(outputFile, "%lf ", w[j]);
        }
        fprintf(outputFile, "\n"); 

        fclose(outputFile); // Closing output file

        double gradient[30000] = {0.0}; // Gradient vector
        error = 0.0;                    // Error value

        for (i = 0; i < 20; i++) // 20 is the number of training data
        {
            int randomIndexA = rand() % 80;
            int randomIndexB = rand() % 80;

            // Randomly choose an example from both classes
            double predictionA = 0.0;
            double predictionB = 0.0;

            for (j = 0; j < wordNumber; j++)
            {
                predictionA += w[j] * (double)oneHotVectorA[j][randomIndexA];
                predictionB += w[j] * (double)oneHotVectorB[j][randomIndexB];
            }

            double diffA = 1.0 - tanh(predictionA);
            double diffB = (-1.0) - tanh(predictionB);

            error += diffA * diffA + diffB * diffB; // Error = Σ(yi − pi)^2

            for (j = 0; j < wordNumber; j++)
            {
                gradient[j] += (-2 * diffA) * ((1.0) - tanh(predictionA) * tanh(predictionA)) * oneHotVectorA[j][randomIndexA];
                gradient[j] += (-2 * diffB) * ((1.0) - tanh(predictionB) * tanh(predictionB)) * oneHotVectorB[j][randomIndexB];
            }
        }

        for (j = 0; j < wordNumber; j++)
        {
            w[j] -= LEARNING_RATE * gradient[j]; // wi = wi − αΣ−2(yi − pi)(1 − tanh^2(pi))xi
        }

        printf("\n%d %lf", it + 1, error / 40); // Printing error value

        it++; // Increasing the number of iterations
    }
}

clock_t adam(int **oneHotVectorA, int **oneHotVectorB, double w[30000], int wordNumber) // Adam function
{
    int it = 0, i, j;
    double m[MAX_WORD] = {0.0}, v[MAX_WORD] = {0.0};   // 1st and 2nd moment vectors
    double beta1 = 0.9, beta2 = 0.999, epsilon = 1e-8; // Adam parameters
    double error = 1.0;                                // Error value

    while (it < ITERATIONS && error / 160 > 0.0001) // 160 is the number of training data
    {
        FILE *outputFile = fopen("ADAM.txt", "w"); // Opening output file
        if (outputFile == NULL)
        {
            fprintf(stderr, "Error opening output file.\n"); // Error message
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < wordNumber; j++) // Printing weight values
        { 
            fprintf(outputFile, "%lf ", w[j]);
        }
        fprintf(outputFile, "\n");

        fclose(outputFile); // Closing output file

        double gradient[30000] = {0.0}; // Gradient vector
        error = 0.0;                    // Error value

        for (i = 0; i < 80; i++) // 80 is the number of training data
        {
            double prediction = 0.0;

            for (j = 0; j < wordNumber; j++)
            {
                prediction += w[j] * (double)oneHotVectorA[j][i]; // pn = w1x1 + w2x2 + ... + wnxn
            }
            double diff = 1.0 - tanh(prediction);
            error += diff * diff; // Error = Σ(yi − pi)^2

            for (j = 0; j < wordNumber; j++)
            {
                gradient[j] += (-2 * diff) * ((1.0) - tanh(prediction) * tanh(prediction)) * oneHotVectorA[j][i]; // Gradient = Σ−2(yi − pi)(1 − tanh^2(pi))xi
            }
        }

        for (i = 0; i < 80; i++)
        {
            double prediction = 0.0;

            for (j = 0; j < wordNumber; j++)
            {
                prediction += w[j] * (double)oneHotVectorB[j][i]; // pi = w1x1 + w2x2 + ... + wnxn
            }

            double diff = (-1.0) - tanh(prediction);
            error += diff * diff; // Error = Σ(yi − pi)^2

            for (j = 0; j < wordNumber; j++)
            {
                gradient[j] += (-2 * diff) * ((1.0) - tanh(prediction) * tanh(prediction)) * oneHotVectorB[j][i]; // Gradient = Σ−2(yi − pi)(1 − tanh^2(pi))xi
            }
        }

        // Adam update
        for (i = 0; i < wordNumber; i++)
        {
            m[i] = beta1 * m[i] + (1 - beta1) * gradient[i];
            v[i] = beta2 * v[i] + (1 - beta2) * (gradient[i] * gradient[i]);
            double m_hat = m[i] / (1 - pow(beta1, it + 1));
            double v_hat = v[i] / (1 - pow(beta2, it + 1));
            w[i] -= LEARNING_RATE * m_hat / (sqrt(v_hat) + epsilon);
        }

        printf("\n%d %lf", it + 1, error / 160); // Printing error value

        it++;
    }
}
