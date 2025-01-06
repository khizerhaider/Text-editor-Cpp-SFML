## Text Editor (C++ & SFML)

![image](https://github.com/user-attachments/assets/6d600a8a-db3d-470e-941a-2aa2013cc546)

![image](https://github.com/user-attachments/assets/451084a9-0fa5-49d0-8eaa-0bc7c57eabe7)

# Overview

This project is a simple text editor built using C++ and SFML (Simple and Fast Multimedia Library). It allows users to type and edit text in a multi-line environment with support for basic features like cursor movement, undo functionality, and file I/O. The editor uses SFML to handle window events and rendering, making it a lightweight and easy-to-use editor for basic text manipulation.
## Features

    Multi-line text editing: Manage text across multiple lines, with a dynamic list of lines represented using a linked list.
    Cursor navigation: Move the cursor left or right within the current line.
    Undo functionality: Revert the last change made (addition, deletion, or moving to the next line).
    File operations: Save the text to a file and load text from an existing file.
    Custom font rendering: Render text on the screen using the Roboto-Light font.

## Requirements

    SFML: The project uses the SFML library to handle graphical components, input, and window management.
    C++ Compiler: You will need a C++ compiler like GCC, Clang, or MSVC.
    Font File: The project uses Roboto-Light.ttf for text rendering.

# Installation
## Clone the Repository

Clone the project to your local machine using the following command:
git clone [https://github.com/khizerhaider/Simple-Text-editor-C-SFML.git](https://github.com/khizerhaider/Simple-Text-editor-C-SFML.git)

cd TextEditor

Install SFML

## Windows:

    Download SFML from the official site and follow the setup instructions for your compiler.

## Linux (Ubuntu):
	sudo apt-get install libsfml-dev
#macOS (Homebrew):
	brew install sfml
## Build the Project

You can compile and run the project manually or use a build tool like Make. Here's an example using g++ (for Linux or macOS):
g++ -o TextEditor main.cpp TextEditor.cpp LineNode.cpp -lsfml-graphics -lsfml-window -lsfml-system
./TextEditor

# Usage

Once the project is compiled, you can run the text editor using the following command:

./TextEditor

## Controls

    Left Arrow Key: Move the cursor left within the current line.
    Right Arrow Key: Move the cursor right within the current line.
    Text Input: Type text into the editor, which will appear in the current line.
    Undo: Reverts the last change made to the text (e.g., adding or removing characters).
    Save: Saves the current text into a .txt file.
    Load: Loads text from an existing .txt file.

# Project Structure

The project structure is as follows:

/TextEditor
├── src/
│   ├── TextEditor.cpp        # Core logic for text editing
│   ├── LineNode.cpp          # Linked list representation of lines
│   ├── main.cpp              # Application entry point
├── include/
│   ├── TextEditor.h          # Header for TextEditor class
│   ├── LineNode.h            # Header for LineNode class
├── assets/
│   └── Roboto-Light.ttf      # Font used for text rendering
├── README.md                 # Project documentation
└── Makefile                  # (Optional) Build script


# Main Classes

    TextEditor: The core class for handling input, rendering, and undo functionality. It also includes file handling for saving and loading text.
    LineNode: A linked list node that represents each line of text. It supports operations like adding and removing characters, resizing, and cursor movement.

## Code Breakdown
### TextEditor Class

    handleInput(sf::Event& event): Processes key presses and mouse events.
    update(): Updates the state of the editor, such as cursor visibility.
    render(sf::RenderWindow& window): Renders the text and cursor on the window.
    undo(): Reverts the last text modification.
    saveToFile(): Saves the current text to a file.
    loadFromFile(): Loads text from a file into the editor.

### LineNode Class

    line: A dynamic array that stores the characters in a line of text.
    cursorPos: The current cursor position within the line.
    appendChar(char c): Appends a character to the line.
    removeChar(): Removes a character from the line at the cursor position.
    moveCursorLeft(): Moves the cursor left within the line.
    moveCursorRight(): Moves the cursor right within the line.

### Future Enhancements

    Text Formatting: Add support for bold, italics, underline, and other text styles.
    Search and Replace: Implement functionality to search for text and replace it.
    Advanced Undo/Redo: Improve the undo/redo system to handle more complex operations.

# Contributing

If you'd like to contribute to the project, you can follow these steps:

    Fork the repository.
    Create a new branch (git checkout -b feature-branch).
    Make your changes and commit them (git commit -am 'Add new feature').
    Push to your branch (git push origin feature-branch).
    Open a pull request with a description of your changes.

# License

This project is licensed under the MIT License. See the LICENSE file for more details.
