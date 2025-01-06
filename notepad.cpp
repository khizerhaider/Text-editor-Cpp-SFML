#include    "notepad.h" // Include the header file for the notepad functionality
#include    <iostream>    // Include iostream for input and output
#include    <fstream>  // Include the header for file handling
#include    "cstringMethods.h" // Include a user defined class to handle array operations
using namespace std; // Use the standard namespace for convenience

// Global stack for undo operations to store the commands for undo functionality
stack<Command> undoStack;

// Function to clear the undo stack
void TextEditor::clearstack() {
    // Clear the undo stack
    while(!undoStack.empty()){
        undoStack.pop();
    }
}
// Command struct's setCommand method that initializes the operation type and associated character
// This method is used to set the type of operation (e.g., ADD, DELETE) and the associated character.
void Command::setCommand(Operation operation, char character) {
    op = operation;  // Set the operation type (ADD, DELETE, NEXTLINE ,CursorLeft,CursorRight,CursorUp,CursorDown)
    text = character; // Set the character related to the operation (the character that was added or deleted)
    cout<<"\nSet command called operation->"<<op<<" character->"<<text<<endl;
}
void Command::setCursorCommand(Operation operation){
    op = operation;
    text = {};
    cout<<"\nSet Command Cursor called operation->"<<op<<" character->"<<text<<endl;
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
        // customMemcpy(newLine, line, length); // copy the old data to the new space
        customMemcpy(newLine, line, length);
        newLine[length] = '\0'; // null-terminate the string
        delete[] line; // free the old memory to avoid memory leak
        line = newLine; // point to the newly allocated memory
        capacity = newCapacity; // update the capacity to the new capacity
    }

// function to move the cursor left by one position
void LineNode::moveCursorLeft() {
        if (cursorPos > 0) {  // check if the cursor is not at the start of the line
            cursorPos--;  // move the cursor left by one position
            Command cmd;
            cmd.setCursorCommand(Command::CursorLeft);
            undoStack.push(cmd);
            return ;
        }
         
    }

// function to move the cursor right by one position
void LineNode::moveCursorRight() {
        if (cursorPos < length) {  // check if the cursor is not at the end of the line
            cursorPos++;  // move the cursor right by one position
            Command cmd;
            cmd.setCursorCommand(Command::CursorRight);
            undoStack.push(cmd);
            return;
        }
    }



// texteditor definitions
// constructor to initialize the text editor with a new line and load the font
TextEditor::TextEditor() : currentLine(new LineNode()), cursorVisible(true),head(currentLine) {
        // try to load the font file, and if it fails, throw an error
        if (!font.loadFromFile("Roboto-Light.ttf")) {
            throw runtime_error("Font file not found");  // font not found exception
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
            case Command :: CursorDown:
                cout<<" Cursor Down";
                break;
            case Command :: CursorLeft:
                cout<<" Cursor Left";
                break;
            case Command :: CursorUp:
                cout<<" Cursor Up";
                break;
            case Command :: CursorRight:
                cout<<" Cursor Right";
                break;
        }
        cout << " with character: " << lastCommand.text << "\n";  // Log the character affected

        // If the operation was an ADD operation (i.e., a character was added)
        if (lastCommand.op == Command::ADD) {
            // Ensure cursor is at a valid position before deletion
            if (currentLine->cursorPos > 0|| currentLine->cursorPos == 0) {
                if(currentLine->cursorPos == 0){
                    cout<<"\nThe pos is 0 now\n";
                    if(currentLine->prev != nullptr){
                    currentLine = currentLine->prev;
                    currentLine->cursorPos = currentLine->length;
                    }
                }
                else if (currentLine->cursorPos > 0) {  // check if the cursor is not at the start of the line
                        currentLine->cursorPos--;  // move the cursor left by one position
                }
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
            // Case 1: Cursor is at the beginning of the line and there is a previous line
            if (currentLine->cursorPos == 0 && currentLine->prev && currentLine->length > 0) {
                LineNode* prevLine = currentLine->prev; // Get the previous line

                // Move the cursor to the end of the previous line
                prevLine->cursorPos = prevLine->length;

                // Append the content of the current line to the end of the previous line
                size_t i = 0;
                while (i < currentLine->length) {
                    prevLine->line[prevLine->length] = currentLine->line[i]; // Copy character
                    prevLine->length++;
                    currentLine->line[i] = '\0'; // Clear the character from the current line
                    i++;
                }
                prevLine->line[prevLine->length] = '\0'; // Null-terminate the updated previous line

                // Update the doubly linked list pointers
                if (currentLine->next != nullptr) {
                    prevLine->next = currentLine->next;
                    currentLine->next->prev = prevLine;
                } else {
                    prevLine->next = nullptr;
                }

                // Delete the current line and move the cursor to the previous line
                delete currentLine;
                currentLine = prevLine;

                // Debug: Log the updated cursor position and line state
                cout << "Cursor position after merging lines: " << currentLine->cursorPos << "\n";
                cout << "Line after merging: " << currentLine->line << "\n";
            } 
            // Case 2: Regular DELETE undo (restore character)
            else {
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
                if (currentLine->cursorPos == currentLine->length) {
                    if (currentLine->next != nullptr) {
                        currentLine = currentLine->next;
                        currentLine->cursorPos = 0;
                    }
                } else if (currentLine->cursorPos < currentLine->length) {  
                    currentLine->cursorPos++;  // Move the cursor right by one position
                }

                // Debug: Log the updated cursor position and line state
                cout << "Cursor position after undoing DELETE: " << currentLine->cursorPos << "\n";
                cout << "Line after undoing DELETE: " << currentLine->line << "\n";
            }
        }

        // If the operation was a NEXTLINE (i.e., a new line was created)
        else if (lastCommand.op == Command::NEXTLINE) {
            // Ensure the previous line exists
            if (currentLine->prev) {
                LineNode* toDelete = currentLine;  // Store reference to the current line
                currentLine = currentLine->prev;  // Move to the previous line

                // Merge the content of the current line into the previous line
                size_t originalLength = currentLine->length;  // Store original length
                size_t newLength = currentLine->length + toDelete->length;  // Calculate new length

                // Ensure the previous line has enough capacity
                if (newLength >= currentLine->capacity) {
                    currentLine->resize(newLength * 2);  // Resize as necessary
                }

                // Append content of the deleted line to the previous line
                for (size_t i = 0; i < toDelete->length; ++i) {
                    currentLine->line[originalLength + i] = toDelete->line[i];
                }
                currentLine->length = newLength;  // Update the length
                currentLine->line[currentLine->length] = '\0';  // Null-terminate the string

                // Update links to bypass the deleted line
                currentLine->next = toDelete->next;
                if (toDelete->next) {
                 toDelete->next->prev = currentLine;
                }

                // Free the memory of the deleted line
                delete toDelete;

                // Move the cursor to the end of the previous line
                currentLine->cursorPos = originalLength;

                // Debug: Log the updated cursor position and merged line content
                cout << "Cursor position after undoing NEXTLINE: " << currentLine->cursorPos << "\n";
                cout << "Merged line content: " << currentLine->line << "\n";
            }
        }

                else if(lastCommand.op == Command::CursorUp){
            if (currentLine->next != nullptr) {
            // Move to the previous line
            currentLine->next->cursorPos=currentLine->cursorPos;
            currentLine = currentLine->next;
            } else {
            cout << "\nNo Next Line line\n"; // Debugging output
            }
        }
        else if(lastCommand.op == Command::CursorDown){
             if (currentLine->prev != nullptr) {
            // Move to the previous line
            currentLine->prev->cursorPos=currentLine->cursorPos;
            currentLine = currentLine->prev;
            } else {
                cout << "\nNo previous line\n"; // Debugging output
                }
        }
        else if(lastCommand.op == Command::CursorRight){
            if(currentLine->cursorPos == 0){
                cout<<"\nThe pos is 0 now\n";
                if(currentLine->prev != nullptr){
                    currentLine = currentLine->prev;
                    currentLine->cursorPos = currentLine->length;
                }
            }
            else if (currentLine->cursorPos > 0) {  // check if the cursor is not at the start of the line
                currentLine->cursorPos--;  // move the cursor left by one position
            }
            
        }
        else if(lastCommand.op == Command::CursorLeft){
            if(currentLine->cursorPos == currentLine->length){
                if(currentLine->next != nullptr){
                    currentLine = currentLine->next;
                    currentLine->cursorPos = 0;
                }

            }
            else if (currentLine->cursorPos < currentLine->length) {  // check if the cursor is not at the end of the line
                currentLine->cursorPos++;  // move the cursor right by one position
            }
        }
    } 
        // If the undo stack is empty, no operatioDELETEn to undo
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
            // Case 1: Cursor is at the beginning of the line (cursorPos == 0)
            if (currentLine->cursorPos == 0 && currentLine->prev) {
                LineNode* prevLine = currentLine->prev; // Get the previous line

                // Move the cursor to the end of the previous line
                prevLine->cursorPos = prevLine->length;

                // Append the content of the current line to the end of the previous line
                size_t i = 0;
                while (i < currentLine->length) {
                    prevLine->line[prevLine->length] = currentLine->line[i]; // Copy character
                    prevLine->length++;
                    currentLine->line[i] = '\0'; // Clear the character from the current line
                    i++;
                }
                prevLine->line[prevLine->length] = '\0'; // Null-terminate the updated previous line

                // Update the doubly linked list pointers
                if (currentLine->next != nullptr) {
                    prevLine->next = currentLine->next;
                    currentLine->next->prev = prevLine;
                } else {
                    prevLine->next = nullptr;
                }

                // Delete the current line and move the cursor to the previous line
                delete currentLine;
                currentLine = prevLine;

                // Add DELETE operation to undo stack
                Command cmd;
                cmd.setCommand(Command::DELETE, '\n'); // '\n' represents the removed line break
                undoStack.push(cmd);
            } 
            // Case 2: Cursor is not at the beginning of the line
            else if (currentLine->cursorPos > 0) {
                // Store the deleted character in the undo stack
                char deletedChar = currentLine->line[currentLine->cursorPos - 1];
                Command cmd;
                cmd.setCommand(Command::DELETE, deletedChar);
                undoStack.push(cmd);

                // Shift characters to the left to remove the character at cursorPos - 1
                for (size_t i = currentLine->cursorPos - 1; i < currentLine->length - 1; i++) {
                    currentLine->line[i] = currentLine->line[i + 1];
                }
                currentLine->line[currentLine->length - 1] = '\0'; // Null-terminate
                currentLine->length--;

                // Update the cursor position
                currentLine->cursorPos--;
            }
        }
        
        // If the user presses Enter (Unicode values '\r' or '\n')
        else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
            // Create a new line node when the user presses Enter
            LineNode* newLine = new LineNode(); // Allocate a new line node dynamically

            if (currentLine->cursorPos < currentLine->length) {
                // Cut the characters present after the cursor and paste them into the new line
                size_t j = 0;
                for (size_t i = currentLine->cursorPos; i < currentLine->length; i++, j++) {
                    newLine->line[j] = currentLine->line[i]; // Copy characters to the new line
                    currentLine->line[i] = '\0'; // Clear characters from the current line
                }
                newLine->line[j] = '\0';
                newLine->length = j; // Update the length of the new line
                currentLine->length = currentLine->cursorPos; // Update the current line length
            }

            // Update the doubly linked list pointers
            newLine->prev = currentLine; // Set the new line's previous pointer to the current line
            newLine->next = currentLine->next; // Set the new line's next pointer to the current line's next

            if (currentLine->next) {
                currentLine->next->prev = newLine; // Update the previous pointer of the next line
            }
            currentLine->next = newLine; // Link the current line to the new line

            // Move the cursor to the start of the new line
            currentLine = newLine;
            currentLine->cursorPos = 0;

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
            moveCursorLeft();
        } 
        
        // If the Right arrow key is pressed, move the cursor to the right
        else if (event.key.code == sf::Keyboard::Right) {
            moveCursorRight();
        } 
        else if(event.key.code == sf::Keyboard::Up) {
            moveCursorUp();
        }
        else if(event.key.code == sf::Keyboard::Down) {
            moveCursorDown();
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
    float y = 50.f;  // Vertical position to start drawing text
    int lineNumber = 1;  // Line number starts from 1

    // Render each line in the linked list
    while (node) {
        // Render the line number
        sf::Text lineNumberText;  // Create a text object to display the line number
        lineNumberText.setFont(font);  // Set the font for the line number
        lineNumberText.setString(std::to_string(lineNumber));  // Convert line number to string
        lineNumberText.setCharacterSize(24);  // Set the font size
        lineNumberText.setFillColor(sf::Color::Blue);  // Set the text color for line numbers
        lineNumberText.setPosition(10.f, y);  // Position the line number
        window.draw(lineNumberText);  // Draw the line number

        // Render the actual line content
        sf::Text text;  // Create a text object to display the current line
        text.setFont(font);  // Set the font for the text
        text.setString(node->line);  // Set the string to the content of the line
        text.setCharacterSize(24);  // Set the font size for the text

        // Highlight the current line with a background rectangle
        if (node == currentLine) {
            sf::RectangleShape highlight(sf::Vector2f(window.getSize().x - 20.f, 30.f));  // Rectangle covering the line
            highlight.setPosition(50.f, y);  // Position the highlight at the line's position (adjusted for line numbers)
            highlight.setFillColor(sf::Color(200, 200, 255, 100));  // Light blue highlight
            window.draw(highlight);  // Draw the highlight rectangle

            // Render the cursor if it's visible
            if (cursorVisible) {
                sf::Text cursorText;
                cursorText.setFont(font);
                cursorText.setString(node->line);
                cursorText.setCharacterSize(24);
                cursorText.setPosition(50.f, y);
                
                sf::Vector2f cursorPos = cursorText.findCharacterPos(currentLine->cursorPos);

                // Create a rectangle shape to represent the cursor
                sf::RectangleShape cursor(sf::Vector2f(2.f, 24.f));  // A vertical line as the cursor
                cursor.setPosition(cursorPos.x, cursorPos.y);  // Position the cursor
                cursor.setFillColor(sf::Color::Black);  // Set the cursor color
                window.draw(cursor);  // Draw the cursor onto the window
            }
        }

        text.setFillColor(sf::Color::Black);  // Set the text color to black
        text.setPosition(50.f, y);  // Position the text on the screen (adjusted for line numbers)
        window.draw(text);  // Draw the text onto the window

        // Update vertical position for the next line
        y += 30.f;

        // Move to the next line in the linked list
        node = node->next;
        lineNumber++;  // Increment the line number
    }
}


void TextEditor::saveToFile(const std::string& filename) {
    ofstream outFile(filename);  // Open the file in write mode

    if (!outFile) {  // Check if the file was opened successfully
        cout << "Error opening file for writing: " << filename << endl;
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
    cout << "File saved successfully as: " << filename << std::endl;
}

void TextEditor::readFromFile(const string& filename) {
    clearstack();
    std::ifstream file(filename, ios::in); // Open file in input mode
    if (!file.is_open()) { // Check if file opened successfully
        cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    LineNode* prevLine = nullptr; // To keep track of the previous line for linking nodes
    LineNode* firstLine = nullptr; // Pointer to the first line (root of the linked list)
    char ch;
    const size_t bufferSize = 128; // Initial buffer size (adjust as needed)
    size_t bufferIndex = 0; // Current index in the buffer
    char* lineBuffer = new char[bufferSize]; // Temporary buffer for line text

    if (!lineBuffer) {
        cerr << "Failed to allocate memory for line buffer." << endl;
        return;
    }

    // Read the file character by character
    while (file.get(ch)) {

        if (ch == '\n' || bufferIndex == bufferSize - 1) { // End of line or buffer is full
            LineNode* newLine = new LineNode(); // Create a new line node

            // Null-terminate the buffer and copy its content to the new line
            lineBuffer[bufferIndex] = '\0';
            newLine->line = new char[bufferIndex + 1];
            std::copy(lineBuffer, lineBuffer + bufferIndex + 1, newLine->line);

            newLine->length = bufferIndex; // Set the line length
            newLine->cursorPos = 0; // Initialize cursor position to the start of the line
    

            // Link the new line into the linked list
            if (prevLine) {
                prevLine->next = newLine;
                newLine->prev = prevLine;
            } else {
                firstLine = newLine; // Set as the first line if this is the start
            }

            prevLine = newLine; // Move to the newly created line
            bufferIndex = 0; // Reset the buffer index for the next line
    
        } else {
            lineBuffer[bufferIndex++] = ch; // Add character to the buffer
    

            // Resize the buffer if necessary
            if (bufferIndex >= bufferSize - 1) {
                size_t newBufferSize = bufferSize * 2;
                char* newBuffer = new char[newBufferSize];
                copy(lineBuffer, lineBuffer + bufferSize, newBuffer);
                delete[] lineBuffer; // Free the old buffer
                lineBuffer = newBuffer;
        
                // Update the buffer size
            }
        }
    }

    // Handle the last line if the file does not end with a newline
    if (bufferIndex > 0) {
        LineNode* newLine = new LineNode();
        // Null-terminate the buffer and copy its content to the new line
        lineBuffer[bufferIndex] = '\0';
        newLine->line = new char[bufferIndex + 1];
        copy(lineBuffer, lineBuffer + bufferIndex + 1, newLine->line);

        newLine->length = bufferIndex;
        newLine->cursorPos = 0;
        // Link the last line into the linked list
        if (prevLine) {
            prevLine->next = newLine;
            newLine->prev = prevLine;
        } else {
            // Set as the first line if this is the start
            firstLine = newLine;
        }
    }
    delete[] lineBuffer; // Free the temporary buffer
    file.close(); // Close the file

    // Set the editor's current line to the first line
    currentLine = firstLine;
}


void TextEditor::moveCursorUp() {
    if (currentLine->prev != nullptr) {
        // Move to the previous line
        currentLine->prev->cursorPos = currentLine->cursorPos;
        currentLine = currentLine->prev;

        Command cmd;
        cmd.setCursorCommand(Command::CursorUp);
        undoStack.push(cmd);

        // Debugging: Log current line state
        cout << "\nMoved cursor up. Current line: " << currentLine->line 
             << "\nCursor position: " << currentLine->cursorPos << "\n";
    } else {
        cout << "\nNo previous line\n"; // Debugging output
    }
}

void TextEditor::moveCursorDown() {
    if (currentLine->next != nullptr) {
        // Move to the next line
        currentLine->next->cursorPos = currentLine->cursorPos;
        currentLine = currentLine->next;

        Command cmd;
        cmd.setCursorCommand(Command::CursorDown);
        undoStack.push(cmd);

        // Debugging: Log current line state
        cout << "\nMoved cursor down. Current line: " << currentLine->line << "\nCursor position: " << currentLine->cursorPos << "\n";
    } else {
        cout << "\nNo next line\n"; // Debugging output
    }
}
void TextEditor::UpdateLineNumber() {
    //generate a funcnction to update the line number like it should increment the line number by 1

    LineNode* node = head;
    int number = 0;
    while (node != nullptr) { 
        node->lineNumber = number++;
        node = node->next;
    }
    // Debugging: Log current line state
    cout << "\nUpdated line numbers. Current line: " << currentLine->line << "\nCursor position: " << currentLine->cursorPos << "\n";
    // Debugging output
    
}

void TextEditor::moveCursorLeft() {
    if (currentLine->cursorPos == 0) {
        cout << "\nCursor position is 0 now\n"; // Debugging output
        if (currentLine->prev != nullptr) {
            currentLine = currentLine->prev;
            currentLine->cursorPos = currentLine->length;

            Command cmd;
            cmd.setCursorCommand(Command::CursorLeft);
            undoStack.push(cmd);
    
            // Debugging: Log current line state
            cout << "\nMoved cursor to the previous line. Current line: " << currentLine->line << "\nCursor position: " << currentLine->cursorPos << "\n";
        } else {
            cout << "\nNo previous line available to move left\n"; // Debugging output
        }
    } else {
        currentLine->moveCursorLeft();
        // Debugging: Log state after moving within the current line
        cout << "\nMoved cursor left. Current cursor position: " << currentLine->cursorPos << "\n"; // Debugging
    }
}

void TextEditor::moveCursorRight() {
    // Check if the cursor is at the end of the current line
    if (currentLine->cursorPos == currentLine->length) {
        // Move to the next line
        if (currentLine->next != nullptr) {
            currentLine = currentLine->next;
            currentLine->cursorPos = 0;

            Command cmd;
            cmd.setCursorCommand(Command::CursorRight);
            undoStack.push(cmd);

            // Debugging: Log state after moving to next line
            cout << "\nMoved cursor to the next line. Current line: " << currentLine->line << "\nCursor position: " << currentLine->cursorPos << "\n";
        } else {
            cout << "\nNo next line to move right\n"; // Debugging output
            return;
        }
    } else {
        currentLine->moveCursorRight();
        // Debugging: Log state after moving within the current line
        cout << "\nMoved cursor right. Current cursor position: " << currentLine->cursorPos << "\n";
    }
}
