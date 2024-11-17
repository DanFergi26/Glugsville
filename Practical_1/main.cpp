#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

// Control keys for players
const Keyboard::Key controls[4] = {
    Keyboard::A,   // Player1 UP
    Keyboard::Z,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down // Player2 Down
};

const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;
const float velocityMultiplier = 1.1f;

// Ball and paddles
CircleShape ball;
RectangleShape paddles[2];

// Game state variables
bool isPlayer1Serving = false;
Vector2f ballVelocity; 
int scorePlayer1 = 0;
int scorePlayer2 = 0;

Font font;   // Declare the sf::Font object
Text text;   // Declare the sf::Text object for displaying scores

// Load resources and initialize game
void Load() {
    // Set size and origin of paddles
    for (auto &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
	
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius); // Origin is half the radius for both X and Y

    // Reset paddle positions
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));

    // Reset ball position and velocity
    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
	
		 // Load font-face from res dir
		 
		if (!font.loadFromFile("C:\\Users\\Callum\\Games-year-3-\\res\\fonts\\ariblk.ttf")) {
    cerr << "Error: Failed to load font. Path might be incorrect or file might be corrupted." << endl;
    // Optionally, check if the file exists using the filesystem or other means
}

	// Set text element to use font
		text.setFont(font);
	// set the character size to 24 pixels
		text.setCharacterSize(24);
}

// Reset the game elements
void Reset() {
    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));  // Center ball
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));  // Reset left paddle
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));  // Reset right paddle
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY }; // Reset ball velocity
	// Update score text
    text.setString("Player 1: " + to_string(scorePlayer1) + "  Player 2: " + to_string(scorePlayer2));
    // Keep Score Text Centered
    text.setPosition((gameWidth * 0.5f) - (text.getLocalBounds().width * 0.5f), 20); // Adjust Y position as needed
}

// Update game logic (called every frame)
void Update(RenderWindow &window) {
    // Reset clock, recalculate delta time
    static Clock clock;
    float dt = clock.restart().asSeconds();

    // Check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // Handle paddle movement
    float direction1 = 0.0f;  // Player 1
    float direction2 = 0.0f;  // Player 2
    if (Keyboard::isKeyPressed(controls[0])) {
        direction1--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction1++;
    }
    if (Keyboard::isKeyPressed(controls[2])) {
        direction2--;
    }
    if (Keyboard::isKeyPressed(controls[3])) {
        direction2++;
    }
    paddles[0].move(Vector2f(0.f, direction1 * paddleSpeed * dt));
    paddles[1].move(Vector2f(0.f, direction2 * paddleSpeed * dt));

    // Move the ball
    ball.move(ballVelocity * dt);

    // Check ball collisions
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;

    // Bottom wall collision
    if (by > gameHeight - ballRadius) {
        ballVelocity.y *= -velocityMultiplier;
        ball.setPosition(bx, gameHeight - ballRadius); // Prevent sticking
    }
    // Top wall collision
    else if (by < ballRadius) {
        ballVelocity.y *= -velocityMultiplier;
        ball.setPosition(bx, ballRadius); // Prevent sticking
    }
    // Right wall collision (score for Player 1)
    else if (bx > gameWidth) {
        scorePlayer1++;
        isPlayer1Serving = true;  // Set next serve for Player 1
        Reset();
    }
    // Left wall collision (score for Player 2)
    else if (bx < 0) {
        scorePlayer2++;
        isPlayer1Serving = false;  // Set next serve for Player 2
        Reset();
    }

    // Paddle collisions (left paddle)
    if (
        bx < paddleSize.x + paddleOffsetWall && 
        by > paddles[0].getPosition().y - (paddleSize.y / 2.f) && 
        by < paddles[0].getPosition().y + (paddleSize.y / 2.f)) {
        // Bounce off left paddle
        ballVelocity.x *= -velocityMultiplier;
        ball.setPosition(paddleSize.x + paddleOffsetWall + ballRadius, by); // Prevent sticking
    }
    // Paddle collisions (right paddle)
    else if (
        bx > gameWidth - paddleSize.x - paddleOffsetWall &&
        by > paddles[1].getPosition().y - (paddleSize.y / 2.f) &&
        by < paddles[1].getPosition().y + (paddleSize.y / 2.f)) {
        // Bounce off right paddle
        ballVelocity.x *= -velocityMultiplier;
        ball.setPosition(gameWidth - paddleSize.x - paddleOffsetWall - ballRadius, by); // Prevent sticking
    }

    // Constrain paddles within window bounds
    for (int i = 0; i < 2; ++i) {
        if (paddles[i].getPosition().y - paddleSize.y / 2.f < 0) {
            paddles[i].setPosition(paddles[i].getPosition().x, paddleSize.y / 2.f);
        }
        if (paddles[i].getPosition().y + paddleSize.y / 2.f > gameHeight) {
            paddles[i].setPosition(paddles[i].getPosition().x, gameHeight - paddleSize.y / 2.f);
        }
    }
}

// Render game elements
void Render(RenderWindow &window) {
    window.clear();  // Clear previous frame
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);  // Draw the ball
    // Optionally, you can add code to display the score here
	window.draw(text);  // Draw the score text
    window.display();  // Present the frame
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        Update(window);
        Render(window);
    }
    return 0;
}
