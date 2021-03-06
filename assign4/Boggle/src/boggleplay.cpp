// .cpp file of boggleplay, which manages user interactions for
// one game of boggle and refers to the Boggle class for data and
// fucntionality.

#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "bogglegui.h"
#include <cmath>

Boggle setUpBoard(Lexicon& dictionary);
bool isValidBoardInput(string& input);
void printHumanState(Boggle &board);
void playHuman(Boggle &board);
void playComputer(Boggle &board);
void printHumanStatusMessage(bool isValidWord, string word);
void gameResult(Boggle& board);
void printSetItem(string item);
void printToConsoleAndGUI(string message);

/**
 * @brief playOneGame
 * Manages playing of one game of Boggle. Sets up Boggle board and interaction,
 * gets human's words, and calls search function for computer. Then prints out
 * each player's scores and the result of the game.
 * @param dictionary
 */
void playOneGame(Lexicon& dictionary) {
    // set up board
    BoggleGUI::initialize(4, 4);
    Boggle board = setUpBoard(dictionary);
    
    // print initial state
    clearConsole();
    BoggleGUI::labelAllCubes(board.getCurrentBoard());
    
    // human
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");
    playHuman(board);
    
    // computer
    cout << endl << "It's my turn!" << endl;
    BoggleGUI::setStatusMessage("It's my turn!");
    playComputer(board);
    
    // game result
    gameResult(board);
}

/**
 * @brief gameResult
 * Prints game result on GUI and console.
 * @param board - Current Boggle board instance.
 */
void gameResult(Boggle& board) {
    string result;
    if (board.getScoreComputer() > board.humanScore()) { // computer wins
        result = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
    } else { // human wins
        result = "WOW, you defeated me! Congratulations!";
    }
    
    printToConsoleAndGUI(result);
}

/**
 * @brief playComputer
 * Calls computer search function from Boggle class and then prints out
 * the search results and score.
 * @param board - Current Boggle board instance.
 */
void playComputer(Boggle &board) {
    Set<string> computerWords = board.computerWordSearch();
    cout << "My words: (" << computerWords.size() << "): " << computerWords << endl;
    cout << "My score: " << board.getScoreComputer() << endl;
    
    for (string word : computerWords) {
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(board.getScoreComputer(), BoggleGUI::COMPUTER);
}

/**
 * @brief playHuman
 * Manages interaction with human to input words and tell
 * Boggle board instance to search for the words. Then prints
 * out proper messages alerting user whether the word was valid.
 * @param board - Current Boggle board instance.
 */
void playHuman(Boggle &board) {
    // get and display words from human
    bool humanHasWords = true;
    bool isValidWord = false;
    string humanWord;
    
    while(humanHasWords) {
        // print current state of game
        cout << board << endl;
        printHumanState(board);
        
        // prompt for word
        humanWord = getLine("Type a word (or Enter to stop): ");
        
        if (humanWord.empty()) {
            humanHasWords = false;
        } else {
            humanWord = toLowerCase(humanWord);
            // check to see if valid word
            BoggleGUI::clearHighlighting();
            isValidWord = board.humanWordSearch(humanWord);
            
            clearConsole();
            printHumanStatusMessage(isValidWord, humanWord);
        }
    }
}

/**
 * @brief printHumanStatusMessage
 * Prints response to a human's input for a possible word based on whether
 * it is a valid word found on the board or not.
 * @param isValidWord - Whether the user input is a valid word found on the board.
 *  Determined by Boggle class functions.
 * @param word - User inputted word.
 */
void printHumanStatusMessage(bool isValidWord, string word) {
    string message;
    
    if(isValidWord) {
        message = string("You found a new word! \"") + toUpperCase(word) + string("\"");
        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
    } else {
        message = "You must enter an unfound 4+ letter word from the dictionary.";
    }
    
    printToConsoleAndGUI(message);
}

/**
 * @brief setUpBoard
 * Handles user interactions for board setup - either tells Boggle board
 * to randomly generate the letters or asks for a valid input string from
 * the user for a custom board. Then creates instance of Boggle board for 
 * use throughout the rest of the game.
 * @param dictionary - Lexicon to pass to Boggle constructor.
 * @return - Returns Boggle instance
 */
Boggle setUpBoard(Lexicon& dictionary) {
    bool randomBoard = getYesOrNo("Do you want to generate a random board? ");
    string userBoardText = "";
    
    // user generated board text
    if(!randomBoard) {
        while(!isValidBoardInput(userBoardText)) {
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }
    
    return Boggle(dictionary, userBoardText);
}

/**
 * @brief isValidBoardInput
 * Prompts user for board string and checks for the validity of the input 
 * for the custom board.
 * @param input - User input string, passed by reference from setUpBoard.
 * @return - Returns whether input string is valid, and therefore whether
 * setUpBoard should continue prompting the user.
 */
bool isValidBoardInput(string& input) {
    input = getLine("Type the 16 letters to appear on the board: ");
    
    // check length of string
    if (input.length() != Boggle::BOARD_SIZE) return false;
    
    // check that all characters are letters
    input = toUpperCase(input);
    for (int i = 0; i < input.length(); i++) {
        if(! ((input[i] >= 'A') && (input[i] <= 'Z'))) return false;
    }
    
    // passed all checks!
    return true;
}

/**
 * @brief printUserState
 * Prints user's current state in the game - numbers of words, list of used
 * words, and score.
 * @param board - Boggle instance being used for the current game.
 */
void printHumanState(Boggle &board) {
    cout << "Your words (" << board.getNumHumanWords() << "): " 
         << board.getHumanWords() << endl;
    cout << "Your score: " << board.humanScore() << endl;
    BoggleGUI::setScore(board.humanScore(), BoggleGUI::HUMAN);
}

/**
 * @brief printToConsoleAndGUI
 * Prints the given message to the console and the GUI.
 * @param message
 */
void printToConsoleAndGUI(string message) {
    cout << message << endl;
    BoggleGUI::setStatusMessage(message);
}
