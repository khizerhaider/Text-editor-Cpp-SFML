Text Editor Project
Overview
This project is a text editor built using C++ and SFML (Simple and Fast Multimedia Library). The text editor allows users to type and edit text, with support for multi-line input, cursor navigation, and basic undo functionality. The editor is designed to be simple, lightweight, and responsive.
Key Features:
    • Multi-line text editor: Manage and edit text across multiple lines.
    • Cursor movement: Navigate the text with left and right cursor movement.
    • Undo functionality: Revert changes made during the current editing session.
    • File operations: Save and read text files for persistence.
    • Custom Font Rendering: Render text using a custom font (Roboto-Light.ttf).

Requirements
To build and run this project, the following software must be installed:
    • C++ Compiler: A C++ compiler like GCC, Clang, or MSVC is required.
    • SFML Library: The project uses SFML for graphical rendering and window handling.
        ◦ You can download SFML from here.
    • Font File: This project uses Roboto-Light.ttf for text rendering. Make sure to place the font file in the project directory or specify its path correctly.

Installation
    • Clone the repository:
           git clone <your-repository-url>
		cd <your-project-directory>
    • Install SFML:
      Windows: Download SFML from the SFML download page, then link it to your C++ project according to the provided instructions.
    • Linux: Install SFML via the package manager. On Ubuntu, you can use:
      sudo apt-get install libsfml-dev
    • macOS: If using Homebrew, install SFML with:
      brew install sfml
    • Build the Project: Compile the project using your preferred C++ build system (e.g., Make, IDE, or directly through command line with g++).
Usage:
    • Running the Text Editor: After building the project, you can run the text editor executable:
      ./text_editor
Features:
    • Text Rendering: Displays the text typed by the user, supporting multiple
	lines.
    • Cursor Movement: Allows the user to move the cursor left and right within the current line.
    • Undo Operations: Supports undo functionality for the last added or deleted character.
    • File Operations: Save and load text files for persistence.
Controls:
    • Left Arrow Key: Move the cursor left within the current line.
    • Right Arrow Key: Move the cursor right within the current line.
    • Text Typing: Simply type to add characters to the current line.
    • Undo: The undo functionality can revert the last change made to the text (character add or delete).
    • Save/Load: Save your work or load previously saved text files.
Project Structure
The project is organized as follows:
/TextEditorProject
│   ├── TextEditor.cpp        # Main class for text editor logic
│   ├── LineNode.cpp          # Definition of LineNode class (used to represent each line of text)
│   ├── main.cpp              # Entry point for the application
│   ├── TextEditor.h          # Header for TextEditor class
│   ├── LineNode.h            # Header for LineNode class
│   └── Roboto-Light.ttf      # Font file used for text rendering
│
├── README.md                 # Project documentation (this file)
