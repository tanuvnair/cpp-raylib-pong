#include "raylib.h"

struct Ball {
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw() {
		DrawCircle((int)x, (int)y, radius, WHITE);
	}
};

struct Paddle {
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect() {
		return Rectangle { x - width / 2 , y - height / 2 , width, height };
	}

	void Draw() {
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main () {
	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball;

	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	const char* winnerText = nullptr;

	while (!WindowShouldClose()) {
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) {
			ball.speedY = GetScreenHeight();
			ball.speedY *= -1;
		}

		if (IsKeyDown(KEY_W)) {
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S)) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP)) {
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN)) {
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		// checks collision
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			} 
		}
		
		if (ball.x < 0) {
			winnerText = "Right Player Wins!";
		}
		if (ball.x > GetScreenWidth()) {
			winnerText = "Left Player Wins!";
		}
		if (winnerText && IsKeyPressed(KEY_SPACE)) {
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 300;
			ball.speedY = 300;
			winnerText = nullptr;
		}

		BeginDrawing(); // begins rendering of our window
			ClearBackground(BLACK); // sets background

			// draws the ball
			ball.Draw();

			// draws the paddles
			leftPaddle.Draw();
			rightPaddle.Draw();

			if (winnerText) {
				int winnerTextWidth = MeasureText(winnerText, 60);
				int continueTextWidth = MeasureText("Press space to continue", 30);

				DrawText(winnerText, GetScreenWidth() / 2 - winnerTextWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
				DrawText("Press space to continue", GetScreenWidth() / 2 - continueTextWidth / 2, GetScreenHeight() / 2 + 45, 30, YELLOW);
			}

			DrawFPS(10, 10);
		EndDrawing(); // ends rendering and handles events like closing the window, etc.
	}

	CloseWindow();

	return 0;
}