

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>
#include<string.h>


using namespace std;


class Guess {

    /*This class stores the user's guess and what the status of each letter was
    when entered into the game. */

private:

    // User's guess gets stored here.

    string guess;

public:

    // Each letter that is marked green in the game, gets stored in this array in its respective position.

    char knownLetters[5] = { '_', '_', '_', '_', '_' };

    // Each letter that is marked yellow in the game, gets stored in this array in its respective position.

    char unknownLetters[5] = { '_', '_', '_', '_', '_' };

    // Each letter that is marked grey in the game gets stored in this array in its respective position.

    char notInUseLetters[5] = { '_', '_', '_', '_', '_' };
    

    void setGuess(string guess) {

        // This sets the 'guess' attribute to the specified word. 

        this->guess = guess;
    }

    string getGuess() {

        // This method returns the 'guess' attribute.

        return this->guess;
    }

    char* getKnownLetters() {

        // This method returns array of all letters marked green in the game.

        return this->knownLetters;
    }

    char* getUnknownLetters() {

        // This method returns array of all letters marked yellow in the game.

        return this->unknownLetters;
    }

    char* getNotInUseLetters() {

        // This method returns array of all letters marked grey in the game.

        return this->notInUseLetters;
    }

};


class Words {

    /*This class stores the list of words we will be checking our 'guess' against. */

public:

    // This vector stores all the words.

    vector<string> words;

    

    void populate(string filename) {

        //This method populates the empty vector attribute with the list of words, extracted from a local file.

        fstream newfile;
        newfile.open(filename, ios::in);  // Open a file to perform write operation using file object.
        if (newfile.is_open()) // Checks whether the file is open.
        {
            
            string word;
            while (getline(newfile, word)) { // Reads data from file object and put it into string.
                words.push_back(word); // Appends word to vector.
                
            }
            newfile.close(); // Close the file object.
        }
    }

    void removeWord(int position) {

        // This method removes a word in a specified position in the vector.

        this->words.erase(words.begin() + position);
    }

    void display() {

        // This method displays every word in the vector.

        for (int count = 1; count <= words.size(); count++) {
            cout << (count) << ". " << words[count-1] << endl;
        }
    }

    void addWord(string word) {

        // This adds a word to the vector.

        this->words.push_back(word);
    }

    void overWrite(vector<string> newWords) {

        // This overwrites the current list of words in the vector, with the list of words in a specified vector.

        this->words = newWords;
    }

    vector<string> getWords() {

        // This method returns full vector of words.

        return this->words;
    }

};


void layoutLetters(string word, int position) {

    // This function displays the guess, with an arrow pointing the letter in the specified position.

    cout << " - " << word[0] << " " << word[1] << " " << word[2] << " " << word[3] << " " <<  word[4] << " - " << endl;
    if (position == 0) cout << "   ^\n   |" << endl;
    else if (position == 1) cout << "     ^\n     |" << endl;
    else if (position == 2) cout << "       ^\n       |" << endl;
    else if (position == 3) cout << "         ^\n         |" << endl;
    else if (position == 4) cout << "           ^\n           |" << endl;
}


void gather_feedback(Guess &guess) {

    // This function allows the user to indicate the color status of each letter in their guess.


    // 'positionColor' tracks which letter in which position had what color. "_" meaning the letter hasn't been assigned a status. "G" meaning green. "Y" meaning yellow. "B" meaning blank.

    string positionColor[] = { "_", "_", "_", "_", "_" };
    



    string letterStatus;

    cout << "Specify if position was green by typing 'G', yellow by typing 'Y', or blank by typing 'B'. " << endl;


    int position = 0; // Position of letter in word we are currently handling.


    while (position != 5) { //iterates through each letter of the five letter guess.

        //Display color status of each letter.
        cout << " - " << positionColor[0] << " " << positionColor[1] << " " << positionColor[2] << " " << positionColor[3] << " " << positionColor[4] << endl;
        
        // Display each letter with arrow point towards each.
        layoutLetters(guess.getGuess(), position);

        // Prompt for color status and transform input into uppercase. 
        cout << "> ";
        cin >> letterStatus;
        std::transform(letterStatus.begin(), letterStatus.end(), letterStatus.begin(), ::toupper);


        // If letter is green, add letter to knownLetters array. 

        if (letterStatus == "G") {
            guess.knownLetters[position] = guess.getGuess()[position];
            positionColor[position] = "G";
        }

        // If letter is yellow, add letter to unknownLetters array. 

        else if (letterStatus == "Y") {
            guess.unknownLetters[position] = guess.getGuess()[position];
            positionColor[position] = "Y";
        }

        // If letter is grey, add letter to notInUseLetters array.

        else if (letterStatus == "B") {
            guess.notInUseLetters[position] = guess.getGuess()[position];
            positionColor[position] = "X";
        }
        position++;
    }
    cout << " - " << positionColor[0] << " " << positionColor[1] << " " << positionColor[2] << " " << positionColor[3] << " " << positionColor[4] << endl;
    layoutLetters(guess.getGuess(), 4);
}



void generatePossibleWords(Guess guess, Words words) {
    
    // Create vector to temporarily hold the words that have the designated Green status. Populate it with current list of words.

    vector<string> possibleWords;

    possibleWords = words.getWords();

    

    for (int index = 0; index < 5; index++) { // Iterate through each letter in the word list.

        if (guess.getKnownLetters()[index] != '_') { // Only run the following code if there is a letter designated green status in the current position.

            for (int i = 0; i < words.getWords().size(); i++) { // For every word in the list of words.

                if (guess.getKnownLetters()[index] == words.getWords()[i][index]) { // If the current selected character is found in the word in the list, in the respectibe position, record the word.

                    possibleWords.push_back(words.getWords()[i]);
                }

                if (i == words.getWords().size() - 1) { // If end of word list reached, overwrite word list with temporary list of words.

                    words.overWrite(possibleWords);

                    possibleWords.clear(); // Clear list of temporary words.

                }
            }
        }
    }

    for (int index = 0; index < 5; index++) {   // For every character in the list of unknownLetters. (Letters designated yellow status)
        if (guess.getUnknownLetters()[index] != '_') { // If letter is position isn't blank.
            for (int i = 0; i < words.getWords().size(); i++) { // For every word in the word list...
                if (words.getWords()[i].find(guess.getUnknownLetters()[index]) != std::string::npos) { // If character found in word from word list.
                    ; // Pass
                }
                else { // Else, remove the word from the word list.
                    words.removeWord(i);
                    i--;
                }
            }
        }
    }

    for (int index = 0; index < 5; index++) { // For every character in the list of unknownLetters. (Letters designated blank status)
        if (guess.getNotInUseLetters()[index] != '_') {  // If letter is position isn't blank.
            for (int i = 0; i < words.getWords().size(); i++) { // For every word in the word list...

                if (words.getWords()[i].find(guess.getNotInUseLetters()[index]) != std::string::npos) { // If character found in word from word list.
                    words.removeWord(i); // Remove the word from the word list.
                    i--;
                }
            }
        }
    }


    // Display list of words.

    if (words.getWords().size() != 0) {
        cout << "Possible Words:" << endl;

        words.display();
    }
    
    else {
        cout << "No possible words found." << endl;
    }
}




int main() {

    // Initiliaze 'Words' class and populate it with words from list
    
    Words words; 

    words.populate("words.txt");  

    // Initialize 'Guess' class and prompt user for word guess.

    string guess;

    Guess userGuess;

    while (guess.length() != 5) {

        cout << "What word did you choose? ";
        cin >> guess;
    }

    std::transform(guess.begin(), guess.end(), guess.begin(), ::toupper); // Transform user input to uppercase.

    userGuess.setGuess(guess); // Store user's guess in guess class.


    // Assign color status to each letter in the user's guess.

    gather_feedback(userGuess); 

    // Display possible words based on color status of letters.
    
    generatePossibleWords(userGuess, words); 

	return 0;
}