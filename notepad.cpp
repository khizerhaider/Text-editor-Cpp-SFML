#include "notepad.h" // Include the header file for the notepad functionality
#include<iostream>    // Include iostream for input and output
#include <fstream>  // Include the header for file handling
using namespace std; // Use the standard namespace for convenience

// Global stack for undo operations to store the commands for undo functionality
stack<Command> undoStack;

// Command struct's setCommand method that initializes the operation type and associated character
// This method is used to set the type of operation (e.g., ADD, DELETE) and the associated character.
void Command::setCommand(Operation operation, char character) {
    op = operation;  // Set the operation type (e.g., ADD, DELETE)
    text = character; // Set the character related to the operation (the character that was added or deleted)
}

// LineNode definitions

// Constructor to initialize a new line node with a specified capacity
LineNode::LineNode(size_t cap)
        : capacity(cap), length(0), cursorPos(0), prev(nullptr), next(nullptr) {
        line = new char[capacity];  // Dynamically allocate memory for the line with the specified capacity
        line[0] = '\0'; // Null-terminate the string initially to indicate an empty line
    }

// Destructor to free the dynamically allocated memory for the line
LineNode::~LineNode() {
    delete[] line; // Delete the dynamically allocated memory for the line
}

// Method to append a character to the current line and handle undo operation
void LineNode::appendChar(char c) {
        // If there is not enough space in the current line, double the capacity
        if (length + 1 >= capacity) {
            resize(capacity * 2); // Resize the line to double the current capacity if necessary
        }

        // Shift characters to the right to make space for the new character
        for (size_t i = length; i > cursorPos; --i) {
            line[i] = line[i - 1]; // Shift the characters to the right starting from the cursor position
        }

        // Insert the new character at the cursor position
        line[cursorPos] = c;

        // Create a Command object to store the current operation (ADD)
        Command cmd;
        cmd.setCommand(Command::ADD, c); // Set the command type to ADD and store the character
        undoStack.push(cmd); // Push the command onto the undo stack to allow undoing later

        length++; // Update the length of the line after adding the character
        cursorPos++; // Move the cursor forward after inserting the character

        // Null-terminate the string after appending the character
        line[length] = '\0';
}


// function to remove a character at the cursor position
void LineNode::removeChar() {
        // check if the cursor position is greater than 0 (i.e., not at the start of the line)
        if (cursorPos > 0) {
            char removedChar = line[cursorPos - 1];  // store the character that will be removed
            cout << "deleted character is " << removedChar << endl;  // print the deleted character
            Command cmd;
            cmd.setCommand(Command::DELETE, removedChar); // store the DELETE operation
            undoStack.push(cmd); // push the DELETE operation onto the undo stack for later undo
            // shift all characters after the cursor position to the left to remove the character
            for (size_t i = cursorPos; i < length; ++i) {
                line[i - 1] = line[i];  // shift each character to the left
            }
            length--;  // reduce the length of the line after removing a character
            cursorPos--;  // move the cursor position back by one
            line[length] = '\0'; // null-terminate the string after removing the character
        }
    }

// function to resize the line to a new capacity
void LineNode::resize(size_t newCapacity) {
        char* newLine = new char[newCapacity];  // create a new array with the new capacity
        std::memcpy(newLine, line, length); // copy the old data to the new space
        newLine[length] = '\0'; // null-terminate the string
        delete[] line; // free the old memory to avoid memory leak
        line = newLine; // point to the newly allocated memory
        capacity = newCapacity; // update the capacity to the new capacity
    }

// function to move the cursor left by one position
void LineNode::moveCursorLeft() {
        if (cursorPos > 0) {  // check if the cursor is not at the start of the line
            cursorPos--;  // move the cursor left by one position
            return ;
        }
         
    }

// function to move the cursor right by one position
void LineNode::moveCursorRight() {
        if (cursorPos < length) {  // check if the cursor is not at the end of the line
            cursorPos++;  // move the cursor right by one position
            return;
        }
    }


// texteditor definitions
// constructor to initialize the text editor with a new line and load the font
TextEditor::TextEditor() : currentLine(new LineNode()), cursorVisible(true) {
        // try to load the font file, and if it fails, throw an error
        if (!font.loadFromFile("Roboto-Light.ttf")) {
            throw std::runtime_error("Font file not found");  // font not found exception
        }
    }

// destructor to clean up memory and delete each line in the text editor
TextEditor::~TextEditor() {
        LineNode* node = currentLine;  // start from the current line
        // iterate through all lines in the editor
        while (node) {
            LineNode* toDelete = node;  // store the current node for deletion
            node = node->prev;  // move to the previous line
            delete toDelete;  // free the memory for the current line
        }
    }


void TextEditor::undo() {
    // Check if there are any commands in the undo stack to undo
    if (!undoStack.empty()) {
        // Get the last command from the undo stack
        Command lastCommand = undoStack.top();
        undoStack.pop();

        // Debug: Log the operation being undone
        cout << "Undo operation: ";
        
        // Check the type of operation (ADD, DELETE, NEXTLINE)
        switch (lastCommand.op) {
            case Command::ADD: 
                cout << "ADD";  // If the operation was an addition
                break;
            case Command::DELETE: 
                cout << "DELETE";  // If the operation was a deletion
                break;
            case Command::NEXTLINE: 
                cout << "NEXTLINE";  // If the operation was a new line creation
                break;
        }
        cout << " with character: " << lastCommand.text << "\n";  // Log the character affected

        // If the operation was an ADD operation (i.e., a character was added)
        if (lastCommand.op == Command::ADD) {
            // Ensure cursor is at a valid position before deletion
            if (currentLine->cursorPos > 0) {
                currentLine->moveCursorLeft(); // Move cursor left before deletion
                // Shift all characters after the cursor one position to the left
                for (size_t i = currentLine->cursorPos; i < currentLine->length; ++i) {
                    currentLine->line[i] = currentLine->line[i + 1];
                }
                currentLine->length--;  // Decrease the length of the line
                currentLine->line[currentLine->length] = '\0';  // Null-terminate the string

                // Debug: Log the updated cursor position and line state
                cout << "Cursor position after undoing ADD: " << currentLine->cursorPos << "\n";
                cout << "Line after undoing ADD: " << currentLine->line << "\n";
            }
        } 
        // If the operation was a DELETE (i.e., a character was deleted)
        else if (lastCommand.op == Command::DELETE) {
            // Ensure the line has enough space to restore the character
            if (currentLine->length + 1 >= currentLine->capacity) {
                currentLine->resize(currentLine->capacity * 2);  // Resize if necessary
            }
            // Shift characters after the cursor one position to the right
            for (size_t i = currentLine->length; i > currentLine->cursorPos; --i) {
                currentLine->line[i] = currentLine->line[i - 1];
            }
            currentLine->line[currentLine->cursorPos] = lastCommand.text;  // Insert the deleted character
            currentLine->length++;  // Increase the length of the line
            currentLine->line[currentLine->length] = '\0';  // Null-terminate the string

            // Move the cursor to the right after restoring the character
            currentLine->moveCursorRight();

            // Debug: Log the updated cursor position and line state
            cout << "Cursor position after undoing DELETE: " << currentLine->cursorPos << "\n";
            cout << "Line after undoing DELETE: " << currentLine->line << "\n";
        } 
        // If the operation was a NEXTLINE (i.e., a new line was created)
        else if (lastCommand.op == Command::NEXTLINE) {
            // Ensure the previous line exists
            if (currentLine->prev) {
                // Remove the current line and restore the previous line
                LineNode* toDelete = currentLine;
                currentLine = currentLine->prev;
                currentLine->next = toDelete->next;  // Link the previous line to the next line
                if (toDelete->next) {
                    toDelete->next->prev = currentLine;  // Update the next line's previous pointer
                }
                delete toDelete;  // Free the memory of the deleted line

                // Move the cursor to the end of the restored line
                while (currentLine->cursorPos < currentLine->length) {
                    currentLine->moveCursorRight();
                }

                // Debug: Log the updated cursor position and line content
                cout << "Cursor position after undoing NEXTLINE: " << currentLine->cursorPos << "\n";
                cout << "Current line content: " << (currentLine ? currentLine->line : "nullptr") << "\n";
            }
        }
    } 
    // If the undo stack is empty, no operation to undo
    else {
        // Debug: Log when there is no operation to undo
        cout << "Undo stack is empty. No operation to undo.\n";
    }
}


void TextEditor::handleInput(sf::Event& event) {
    // Check if the event type is TextEntered (i.e., the user has typed something)
    if (event.type == sf::Event::TextEntered) {
        
        // If the user presses the Backspace key (Unicode value '\b')
        if (event.text.unicode == '\b') { 
            // Ensure the cursor is not at the beginning of the line
            if (currentLine->cursorPos > 0) {
                currentLine->removeChar(); // Remove the character at the cursor position
                
                // If the current line is empty and it is not the root line, move to the previous line
                if (currentLine->length == 0 && currentLine->prev) {
                    currentLine = currentLine->prev; // Move to the previous line
                    delete currentLine->next; // Delete the next line (which is now orphaned)
                    currentLine->next = nullptr; // Set the next pointer to nullptr
                    // Create a DELETE command for the undo stack
                    Command cmd;
                    cmd.setCommand(Command::DELETE, '\n'); // Store the DELETE operation
                    undoStack.push(cmd); // Push the command onto the undo stack
                }
            }
        }
        
        // If the user presses Enter (Unicode values '\r' or '\n')
        else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
            // Create a new line node when the user presses Enter
            LineNode* newLine = new LineNode();
            newLine->prev = currentLine; // Set the previous line to the current line
            newLine->next = currentLine->next; // Set the next line to the current line's next

            // If the current line has a next line, update its previous pointer to the new line
            if (currentLine->next) {
                currentLine->next->prev = newLine;
            }
            currentLine->next = newLine; // Set the current line's next pointer to the new line
            currentLine = newLine; // Move the current line pointer to the new line

            // Create a NEXTLINE command for the undo stack
            Command cmd;
            cmd.setCommand(Command::NEXTLINE, '\n'); // Store the NEXTLINE operation
            undoStack.push(cmd); // Push the command onto the undo stack
        }
        
        // If the user presses a printable character (excluding control characters like space or special chars)
        else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            // Convert the Unicode value to a char
            char insertedChar = static_cast<char>(event.text.unicode);

            // Append the character to the current line
            currentLine->appendChar(insertedChar);
        }
    }
    
    // Check if the event type is KeyPressed (i.e., a key on the keyboard is pressed)
    else if (event.type == sf::Event::KeyPressed) {
        
        // If the Left arrow key is pressed, move the cursor to the left
        if (event.key.code == sf::Keyboard::Left) {
            currentLine->moveCursorLeft();
        } 
        
        // If the Right arrow key is pressed, move the cursor to the right
        else if (event.key.code == sf::Keyboard::Right) {
            currentLine->moveCursorRight();
        } 
        
        // If Ctrl+Z is pressed (Undo operation)
        else if (event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            undo(); // Call the undo method to undo the last operation
        }
        else if (event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        saveToFile("notepad.txt"); // Save to file when Ctrl+S is pressed
    }
    }
}




void TextEditor::update() {
    // Check if 500 milliseconds have passed since the last update
    if (clock.getElapsedTime().asMilliseconds() >= 500) {
        cursorVisible = !cursorVisible; // Toggle the cursor visibility (make it blink)
        
        // Restart the clock to start counting from zero again
        clock.restart();
    }
}


void TextEditor::render(sf::RenderWindow& window) {
    // Start from the current line and move to the first line in the linked list
    LineNode* node = currentLine;
    while (node->prev) {
        node = node->prev; // Move to the previous line until the first line is reached
    }

    // Initialize the starting position for rendering lines
    float y = 10.f;  // Vertical position to start drawing text

    // Render each line in the linked list
    while (node) {
        sf::Text text;  // Create a text object to display the current line
        text.setFont(font);  // Set the font for the text
        text.setString(node->line);  // Set the string to the content of the line
        text.setCharacterSize(24);  // Set the font size for the text
        text.setFillColor(sf::Color::Black);  // Set the text color to black
        text.setPosition(10.f, y);  // Set the position of the text on the screen
        window.draw(text);  // Draw the text onto the window

        y += 30.f;  // Move the vertical position down for the next line
        node = node->next;  // Move to the next line in the linked list
    }

    // If the cursor is visible, render it
    if (cursorVisible && currentLine) {
        sf::Text text;  // Create another text object to display the current line
        text.setFont(font);  // Set the font for the text
        text.setString(currentLine->line);  // Set the string to the current line
        text.setCharacterSize(24);  // Set the font size for the text
        text.setFillColor(sf::Color::Black);  // Set the text color to black
        text.setPosition(10.f, y - 30.f);  // Set the position of the text (adjusted for the last line)

        // Get the position of the cursor on the screen
        sf::Vector2f cursorPos = text.findCharacterPos(currentLine->cursorPos);

        // Create a rectangle shape to represent the cursor
        sf::RectangleShape cursor(sf::Vector2f(2.f, 24.f));  // A vertical line as the cursor
        cursor.setPosition(cursorPos.x, y - 30.f);  // Position the cursor at the appropriate location
        cursor.setFillColor(sf::Color::Black);  // Set the cursor color to black
        window.draw(cursor);  // Draw the cursor onto the window
    }
}



void TextEditor::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename);  // Open the file in write mode

    if (!outFile) {  // Check if the file was opened successfully
        std::cout << "Error opening file for writing: " << filename << std::endl;
        return;  // Exit the method if file opening fails
    }

    // Start with the first line (currentLine)
    LineNode* node = currentLine;
    while (node->prev) {
        node = node->prev;  // Move to the first line in the linked list
    }

    // Write each line's content to the file
    while (node) {
        outFile << node->line << "\n";  // Write the line followed by a newline character
        node = node->next;  // Move to the next line
    }

    outFile.close();  // Close the file after writing is done
    std::cout << "File saved successfully as: " << filename << std::endl;
}

void TextEditor::readFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::in);  // Open file in input mode
    if (!file.is_open()) {  // Check if file opened successfully
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    LineNode* prevLine = nullptr;  // To keep track of the previous line for linking nodes
    char ch;
    const size_t bufferSize = 128; // Initial buffer size (you can adjust this)
    size_t bufferIndex = 0; // Current index in the buffer
    char* lineBuffer = new char[bufferSize]; // Character array to store the line's text

    // Read the file character by character
    while (file.get(ch)) {
        if (ch == '\n' || bufferIndex == bufferSize - 1) {  // End of line or buffer is full
            LineNode* newLine = new LineNode();

            // Null-terminate the line and allocate memory for the line
            lineBuffer[bufferIndex] = '\0';
            newLine->line = new char[bufferIndex + 1];
            std::copy(lineBuffer, lineBuffer + bufferIndex + 1, newLine->line);

            newLine->length = bufferIndex;  // Store the length of the line
            newLine->cursorPos = newLine->length;  // Place the cursor at the end of the line

            // Link the new line node
            if (prevLine) {
                prevLine->next = newLine;
                newLine->prev = prevLine;
            } else {
                currentLine = newLine;  // Set as the first line if it's the beginning
            }

            prevLine = newLine;  // Move to the next line

            bufferIndex = 0;  // Reset the buffer index for the next line
        } else {
            lineBuffer[bufferIndex++] = ch;  // Add the character to the buffer
            if (bufferIndex >= bufferSize) {  // If buffer is full, increase its size
                size_t newBufferSize = bufferSize * 2;
                char* newBuffer = new char[newBufferSize];
                std::copy(lineBuffer, lineBuffer + bufferSize, newBuffer);
                delete[] lineBuffer;  // Free old buffer
                lineBuffer = newBuffer;
            }
        }
    }

    // If there are characters remaining in the buffer after the last line, process them
    if (bufferIndex > 0) {
        LineNode* newLine = new LineNode();
        lineBuffer[bufferIndex] = '\0'; // Null-terminate the last line
        newLine->line = new char[bufferIndex + 1];
        std::copy(lineBuffer, lineBuffer + bufferIndex + 1, newLine->line);
        newLine->length = bufferIndex;
        newLine->cursorPos = newLine->length;

        if (prevLine) {
            prevLine->next = newLine;
            newLine->prev = prevLine;
        } else {
            currentLine = newLine;
        }
    }

    delete[] lineBuffer; // Clean up the last buffer
    file.close();  // Close the file after reading
}
