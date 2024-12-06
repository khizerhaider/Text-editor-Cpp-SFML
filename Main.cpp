#include "notepad.h"  // Include the header file for the TextEditor class
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>  // For std::cout, std::cin (optional debug)

class FileInputPopup {
public:
    FileInputPopup(sf::Font& font) : isActive(false), fileName("") {
        // Create popup background
        popupBackground.setSize(sf::Vector2f(400.f, 200.f));
        popupBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent background
        popupBackground.setPosition(200.f, 150.f);

        // Create prompt text
        promptText.setFont(font);
        promptText.setString("Enter the filename:");
        promptText.setCharacterSize(24);
        promptText.setFillColor(sf::Color::White);
        promptText.setPosition(220.f, 170.f);

        // Create input text display
        fileNameText.setFont(font);
        fileNameText.setString("");
        fileNameText.setCharacterSize(24);
        fileNameText.setFillColor(sf::Color::White);
        fileNameText.setPosition(220.f, 210.f);

        // Create input box
        inputBox.setSize(sf::Vector2f(300.f, 40.f));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(2.f);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setPosition(220.f, 250.f);

        // Create OK button
        okButton.setSize(sf::Vector2f(100.f, 40.f));
        okButton.setFillColor(sf::Color::Green);
        okButton.setPosition(220.f, 300.f);

        okButtonText.setFont(font);
        okButtonText.setString("OK");
        okButtonText.setCharacterSize(24);
        okButtonText.setFillColor(sf::Color::White);
        okButtonText.setPosition(240.f, 310.f);

        // Create Cancel button
        cancelButton.setSize(sf::Vector2f(100.f, 40.f));
        cancelButton.setFillColor(sf::Color::Red);
        cancelButton.setPosition(320.f, 300.f);

        cancelButtonText.setFont(font);
        cancelButtonText.setString("Cancel");
        cancelButtonText.setCharacterSize(24);
        cancelButtonText.setFillColor(sf::Color::White);
        cancelButtonText.setPosition(340.f, 310.f);
    }

    void handleInput(sf::Event event) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !fileName.empty()) {
                fileName.pop_back();
            } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                fileName += static_cast<char>(event.text.unicode);
            }
        }
    }

    bool handleButtonClick(sf::Vector2i mousePos) {
        if (okButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            return true;  // OK button clicked
        }
        if (cancelButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            return false;  // Cancel button clicked
        }
        return false;
    }

    void render(sf::RenderWindow& window) {
        window.draw(popupBackground);
        window.draw(promptText);
        window.draw(fileNameText);
        window.draw(okButton);
        window.draw(okButtonText);
        window.draw(cancelButton);
        window.draw(cancelButtonText);
    }

    void setFileNameInput(const std::string& input) {
        fileName = input;
        fileNameText.setString(fileName);
    }

    std::string getFileName() {
        return fileName;
    }

    void show() {
        isActive = true;
    }

    void hide() {
        isActive = false;
    }

    bool isVisible() {
        return isActive;
    }

private:
    bool isActive;
    std::string fileName;

    // Popup elements
    sf::RectangleShape popupBackground;
    sf::Text promptText;
    sf::Text fileNameText;
    sf::RectangleShape inputBox;

    sf::RectangleShape okButton;
    sf::Text okButtonText;

    sf::RectangleShape cancelButton;
    sf::Text cancelButtonText;
};

int main() {
    // Create a window with a size of 800x600 and title "Simple Notepad with Blinking Cursor"
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simple Notepad with Blinking Cursor");

    // Set the frame rate limit to 60 FPS
    window.setFramerateLimit(60);

    // Create a TextEditor object to handle text input and rendering
    TextEditor editor;

    // Create a font object
    sf::Font font;
    if (!font.loadFromFile("Roboto-Light.ttf")) {  // Ensure a font is loaded
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    // Create the file input popup
    FileInputPopup fileInputPopup(font);

    // Create Open File button at the top-left corner
    sf::RectangleShape openFileButton(sf::Vector2f(120.f, 40.f));
    openFileButton.setFillColor(sf::Color::Blue);
    openFileButton.setPosition(10.f, 10.f);

    sf::Text openFileButtonText;
    openFileButtonText.setFont(font);
    openFileButtonText.setString("Open File");
    openFileButtonText.setCharacterSize(24);
    openFileButtonText.setFillColor(sf::Color::White);
    openFileButtonText.setPosition(15.f, 15.f);

    bool fileInputHandled = false;

    // Main event loop: keep the window open as long as it is not closed
    while (window.isOpen()) {
        sf::Event event;  // Create an event object to handle user inputs and system events

        // Poll all events from the window
        while (window.pollEvent(event)) {
            // If the window is closed, close the window
            if (event.type == sf::Event::Closed) {
                window.close();  // Close the window
            }

            // Handle file input popup events
            if (fileInputPopup.isVisible()) {
                if (event.type == sf::Event::TextEntered) {
                    // Handle the input directly
                    if (event.text.unicode == '\b' && !fileInputPopup.getFileName().empty()) {
                        fileInputPopup.setFileNameInput(fileInputPopup.getFileName().substr(0, fileInputPopup.getFileName().size() - 1));
                    } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        fileInputPopup.setFileNameInput(fileInputPopup.getFileName() + static_cast<char>(event.text.unicode));
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    // Check if the user clicked a button in the popup
                    bool proceed = fileInputPopup.handleButtonClick(sf::Mouse::getPosition(window));
                    if (proceed) {
                        // Try reading from the file
                        std::string fileName = fileInputPopup.getFileName();
                        if (!fileName.empty()) {
                            editor.readFromFile(fileName);  // Read from file
                        }
                        fileInputPopup.hide();  // Close the popup
                    } else {
                        fileInputPopup.hide();  // Close the popup without reading file
                    }
                }
            } else {
                // Handle "Open File" button click event
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (openFileButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        fileInputPopup.show();  // Show the file input popup
                    }
                }
                // Handle other input events (like text input, key presses)
                editor.handleInput(event);
            }
        }

        // Update the editor state (like blinking cursor)
        editor.update();

        // Clear the window with a white background color
        window.clear(sf::Color::White);

        // Render the "Open File" button
        window.draw(openFileButton);
        window.draw(openFileButtonText);

        // Render the file input popup if it's visible
        if (fileInputPopup.isVisible()) {
            fileInputPopup.render(window);
        } else {
            // Ensure the text editor starts below the "Open File" button
            editor.render(window);
        }

        // Display the updated window content
        window.display();
    }

    // Return 0 to indicate that the program ended successfully
    return 0;
}
