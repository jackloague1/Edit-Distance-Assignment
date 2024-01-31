#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>


// Represents an element in the edit distance matrix.
struct MatrixElement
{
    int value;

    bool rightArrow;
    bool downArrow;
    bool diagonalArrow;

    bool exhausted;

    MatrixElement()
    {
        value = -1;

        rightArrow = false;
        downArrow = false;
        diagonalArrow = false;

        exhausted = false;
    }
};


// Validates that the words the user inputs contain only lower or uppercase letters.
bool inputValidation(std::string & word)
{
    for (int i = 0; i < word.length(); i++)
    {
        if (std::isalpha(word[i]) == false)
        {
            return false;
        }
    }

    return true;
}


// Receives the two words from the user.
void userInput(std::string & firstWord, std::string & secondWord)
{
    std::cout << "Please enter the first word: ";
    getline(std::cin, firstWord);

    // Validates that the word only contains lower or uppercase letters.
    inputValidation(firstWord);

    // If the input was invalid, keeps asking the user for input until a valid string is given.
    while (inputValidation(firstWord) == false)
    {
        std::cout << std::endl;
        std::cout << "Input must only contain lower and uppercase letters. Please try again.";
        std::cout << std::endl << std::endl;
        std::cout << "Please enter the first word: ";
        getline(std::cin, firstWord);

        inputValidation(firstWord);
    }

    std::cout << std::endl;

    std::cout << "Please enter the second word: ";
    getline(std::cin, secondWord);

    // Validates that the word only contains lower or uppercase letters.
    inputValidation(secondWord);

    // If the input was invalid, keeps asking the user for input until a valid string is given.
    while (inputValidation(secondWord) == false)
    {
        std::cout << std::endl;
        std::cout << "Input must only contain lower and uppercase letters. Please try again.";
        std::cout << std::endl << std::endl;
        std::cout << "Please enter the second word: ";
        getline(std::cin, secondWord);

        inputValidation(secondWord);
    }

    std::cout << std::endl;

    std::cout << "First Word: " << firstWord;
    std::cout << std::endl;
    std::cout << "Second Word: " << secondWord;
    std::cout << std::endl << std::endl;
}


// Produces the edit distance matrix, as well as lists the edit distance value, and the alignment.
void editDistance(std::string & firstWord, std::string & secondWord)
{
    int matrixWidth = firstWord.length() + 1;
    int matrixHeight = secondWord.length() + 1;

    MatrixElement editDistanceMatrix[matrixHeight][matrixWidth];

    // Calculates and assigns the values and arrow directions for the edit distance matrix.
    for (int i = 0; i < matrixHeight; i++)
    {
        for (int j = 0; j < matrixWidth; j++)
        {
            // Assigns the top left element of the matrix a value of 0.
            if (i == 0 && j == 0)
            {
                editDistanceMatrix[i][j].value = 0;
            }
            // If the element is in the first row of the matrix, assigns the element
            // the value the element of the previous column plus 1.
            else if (i == 0)
            {
                editDistanceMatrix[i][j].value = editDistanceMatrix[i][j-1].value + 1;

                // Keeps track that the current element got its value from the left element.
                editDistanceMatrix[i][j-1].rightArrow = true;
            }
            // If the element is in the first column of the matrix, assigns the element
            // the value the element of the previous row plus 1.
            else if (j == 0)
            {
                editDistanceMatrix[i][j].value = editDistanceMatrix[i-1][j].value + 1;

                // Keeps track that the current element got its value from the top element.
                editDistanceMatrix[i-1][j].downArrow = true;
            }
            else
            {
                // If the row and column letters match, assign the current element the value
                // of the element that is to the top left diagonal.
                if (secondWord[i-1] == firstWord[j-1])
                {
                    editDistanceMatrix[i][j].value = editDistanceMatrix[i-1][j-1].value;

                    // Keeps track that the current element got its value from the top left diagonal element.
                    editDistanceMatrix[i-1][j-1].diagonalArrow = true;
                }
                // If the row and column letters do not match, compare the values of the left, top,
                // and top left diagonal elements of the current element and assign the current
                // element the smallest value of the three plus 1.
                else
                {
                    int smallestElement = std::min(editDistanceMatrix[i][j-1].value, 
                                                   std::min(editDistanceMatrix[i-1][j-1].value, editDistanceMatrix[i-1][j].value));

                    editDistanceMatrix[i][j].value = smallestElement + 1;

                    // Keeps track of from what element the current element got its value.
                    if (editDistanceMatrix[i][j-1].value == smallestElement && editDistanceMatrix[i-1][j-1].value == smallestElement
                        && editDistanceMatrix[i-1][j].value == smallestElement)
                    {
                        editDistanceMatrix[i][j-1].rightArrow = true;
                        editDistanceMatrix[i-1][j-1].diagonalArrow = true;
                        editDistanceMatrix[i-1][j].downArrow = true;
                    }
                    else if (editDistanceMatrix[i][j-1].value == smallestElement && editDistanceMatrix[i-1][j-1].value == smallestElement)
                    {
                        editDistanceMatrix[i][j-1].rightArrow = true;
                        editDistanceMatrix[i-1][j-1].diagonalArrow = true;
                    }
                    else if (editDistanceMatrix[i-1][j-1].value == smallestElement && editDistanceMatrix[i-1][j].value == smallestElement)
                    {
                        editDistanceMatrix[i-1][j-1].diagonalArrow = true;
                        editDistanceMatrix[i-1][j].downArrow = true;
                    }
                    else if (editDistanceMatrix[i][j-1].value == smallestElement && editDistanceMatrix[i-1][j].value == smallestElement)
                    {
                        editDistanceMatrix[i][j-1].rightArrow = true;
                        editDistanceMatrix[i-1][j].downArrow = true;
                    }
                    else if (smallestElement == editDistanceMatrix[i][j-1].value)
                    {
                        editDistanceMatrix[i][j-1].rightArrow = true;
                    }
                    else if (smallestElement == editDistanceMatrix[i-1][j-1].value)
                    {
                        editDistanceMatrix[i-1][j-1].diagonalArrow = true;
                    }
                    else
                    {
                        editDistanceMatrix[i-1][j].downArrow = true;
                    }
                }
            }
        }
    }

    bool pathFound = false;
    int i = 0;
    int j = 0;
    MatrixElement currentElement = editDistanceMatrix[i][j];
    std::vector<std::string> path;

    // Finds a path through the matrix from the top left element to the bottom right element.
    // This path will be used for the alignment.
    while (pathFound == false)
    {
        // If the current element does not equal the element in the last row and column of the matrix,
        // a path has not been found yet.
        if (i == matrixHeight-1 && j == matrixWidth-1)
        {
            pathFound = true;
        }
        // If the current element has an arrow leading to the element to the right and that element has
        // not been exhausted, move to that element.
        else if (currentElement.rightArrow == true && editDistanceMatrix[i][j+1].exhausted == false)
        {
            currentElement = editDistanceMatrix[i][j+1];
            path.push_back("Right");

            j++;
        }
        // If the current element has an arrow leading to its down right diagonal element and that element has
        // not been exhausted, move to that element.
        else if (currentElement.diagonalArrow == true && editDistanceMatrix[i+1][j+1].exhausted == false)
        {
            currentElement = editDistanceMatrix[i+1][j+1];
            path.push_back("Diagonal");

            i++;
            j++;
        }
        // If the current element has an arrow leading to the element below it and that element has
        // not been exhausted, move to that element.
        else if (currentElement.downArrow == true && editDistanceMatrix[i+1][j].exhausted == false)
        {
            currentElement = editDistanceMatrix[i+1][j];
            path.push_back("Down");

            i++;
        }
        // If the current element does not have any arrows leading to any elements or all of the elements
        // the arrows lead to have been exhausted, set the current element as exhausted and start the path
        // again from the top left element of the matrix.
        else
        {
            editDistanceMatrix[i][j].exhausted = true;

            i = 0;
            j = 0;

            currentElement = editDistanceMatrix[i][j];
            
            path.clear();
        }
    }

    // Ouputs the matrix.
    std::cout << "Matrix:";
    std::cout << std::endl;
    std::cout << "          ";
    for (int i = 0; i < firstWord.length(); i++)
    {
        std::cout << std::left << std::setw(5) << firstWord[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < matrixHeight; i++)
    {
        for (int j = 0; j < matrixWidth; j++)
        {
            if (i == 0 && j == 0)
            {              
                std::cout << "     " << std::left << std::setw(5) << editDistanceMatrix[i][j].value;
            }
            else
            {
                if (i > 0 && j == 0)
                {
                    std::cout << std::left << std::setw(5) << secondWord[i-1];
                }
                std::cout << std::left << std::setw(5) << editDistanceMatrix[i][j].value; 
            }
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // Outputs the edit distance value.
    std::cout << "The edit distance is: " << editDistanceMatrix[matrixHeight-1][matrixWidth-1].value; 

    std::cout << std::endl << std::endl;

    int wordIndex = 0;

    // Outputs the alignment.
    std::cout << "Alignment is: ";
    std::cout << std::endl;
    // Alignment for first word.
    for (int i = 0; i < path.size(); i++)
    {
        if (path[i] == "Diagonal" || path[i] == "Right")
        {
            std::cout << firstWord[wordIndex];
        }
        else if (path[i] == "Down")
        {
            std::cout << "_";
            wordIndex--;
        }
        wordIndex++;
    }

    wordIndex = 0;
    std::cout << std::endl;

    // Alignment for second word.
    for (int i = 0; i < path.size(); i++)
    {
        if (path[i] == "Diagonal" || path[i] == "Down")
        {
            std::cout << secondWord[wordIndex];
        }
        else if (path[i] == "Right")
        {
            std::cout << "_";
            wordIndex--;
        }
        wordIndex++;
    }
    
    std::cout << std::endl;
}


int main()
{
    std::string firstWord;
    std::string secondWord;

    // Prompts the user to input the two words.
    userInput(firstWord, secondWord);

    // Produces the edit distance matrix, edit distance value, and alignment.
    editDistance(firstWord, secondWord);

    return 0;
}