#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <SFML/Graphics.hpp>  // Include for graphical components (like rendering text, shapes, etc.)
#include <SFML/Window.hpp>    // Include for handling window events and input
#include <iostream>            // Include for input/output operations
#include <stack>               // Include for stack data structure used for undo functionality

// Command struct to store operations
// This struct is used to store the type of operation (ADD, DELETE, NEXTLINE) 
// and the associated character that is part of that operation
struct Command {
    enum Operation { ADD, DELETE, NEXTLINE ,CursorLeft,CursorRight,CursorUp,CursorDown} op; // Enum for command operations
    char text; // Character associated with the operation (e.g., the added or deleted character)

    // Function to set the command's operation and text
    void setCommand(Operation operation, char character);
    void setCursorCommand(Operation Operation);
};

// LineNode struct to represent a line of text
// Each line of text is stored as a linked list node. The struct holds the data for one line, 
// along with pointers to the next and previous lines, facilitating navigation through the text.
class LineNode {
    public:
    char* line;           // A dynamic array to store characters in the line of text
    size_t capacity;      // Maximum capacity of the line (allocated memory size)
    size_t length;        // Current number of characters in the line
    size_t cursorPos;     // The position of the cursor within the line
    LineNode* prev;       // Pointer to the previous line node
    LineNode* next;       // Pointer to the next line node
    int lineNumber;

    // Constructor to initialize a LineNode with a specified capacity (default 128)
    LineNode(size_t cap =128);
    // Destructor to clean up the dynamic memory used by the line
    ~LineNode();
    // Method to append a character to the line
    void appendChar(char c);
    // Method to remove the character at the current cursor position
    void removeChar();
    // Method to resize the line's storage capacity
    void resize(size_t newCapacity);
    // Methods to move the cursor left and right within the line
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorUp();
    void moveCursorDown();
};

// TextEditor class to handle input, update, render, and undo operations
// This class represents the notepad's main functionality, including handling user input,
// updating the editor state, rendering the text on the screen, and performing undo operations.
class TextEditor {
private:
    LineNode* currentLine; // Pointer to the current line where text is being typed
    LineNode* head;
    sf::Font font;         // Font used for rendering text
    sf::Clock clock;       // Clock used to control timing (e.g., blinking cursor)
    bool cursorVisible;    // Boolean to manage cursor visibility (for blinking cursor)

public:
    // Constructor to initialize the text editor and the initial line
    TextEditor();
    // Destructor to clean up allocated memory and resources
    ~TextEditor();

    // Method to handle input events (key presses, text input, etc.)
    void handleInput(sf::Event& event);
    // Method to update the editor (e.g., move cursor, handle time)
    void update();
    // Method to render the text editor (draw text and cursor on the window)
    void render(sf::RenderWindow& window);
    // Method to perform an undo operation (reverts the last change)
    void undo();
    // Method to save text to a
    void saveToFile(const std::string& filename);
    // method to read from a .txt file
    void readFromFile(const std::string& filename);
    // method to move the cursor to the above line
    void moveCursorUp();
    // method to move the cursor to the below line
    void moveCursorDown();
    // method to move the cursor freely through left key press
    void moveCursorLeft();
    
    void moveCursorRight();

    void adjustViewport();
    
    void UpdateLineNumber(); 

    void clearstack();

};

#endif // NOTEPAD_H
