#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine_2.0.h"

using namespace std;

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Example";
	}

	olc::Sprite* sprDemo = nullptr;
	olc::Decal* decDemo = nullptr;

	olc::Sprite* sprBlock = nullptr;
	olc::Decal* decBlock = nullptr;
	olc::Decal* decBlock2 = nullptr;

	vector<olc::Sprite*> vBG;
	vector<pair<olc::Decal*, float> > vBackgrounds;

	olc::vf2d vChar;
	float fAcceleration;
	float fVelocity;
	float fPosition;
	float fLevelPosition;

	float fBlockPos, fBlockPos2;

	int counter = 0;
	int bestRecord = 0;
	int x = 0;

	float fSectionWidth;
	list<int> listSection;

	bool bJumpedOnce = true;
	bool bOver = false;
	bool bSecondBox = false;
	bool bReset = false;

public:
	bool OnUserCreate() override {

		sprDemo = new olc::Sprite("assets/pixelChar.png");
		decDemo = new olc::Decal(sprDemo);

		sprBlock = new olc::Sprite("assets/pixelBlock.png");
		decBlock = new olc::Decal(sprBlock);
		decBlock2 = new olc::Decal(sprBlock);

		fBlockPos = 894.0f;
		fBlockPos2 = 900.0f;

		vChar = { 0.0f, 480.0f };

		vBG.push_back(new olc::Sprite("assets/bg1.png"));
		vBG.push_back(new olc::Sprite("assets/bg2.png"));
		vBG.push_back(new olc::Sprite("assets/bg3.png"));
		vBG.push_back(new olc::Sprite("assets/bg4.png"));

		vBackgrounds.push_back(make_pair(new olc::Decal(vBG[0]), 0.0f));
		vBackgrounds.push_back(make_pair(new olc::Decal(vBG[1]), 298.0f));
		vBackgrounds.push_back(make_pair(new olc::Decal(vBG[2]), 596.0f));
		vBackgrounds.push_back(make_pair(new olc::Decal(vBG[3]), 894.0f));

		listSection = { 0,0,0,0 };
		fSectionWidth = (float)ScreenWidth() / (float)(listSection.size() - 1);

		fAcceleration = 0.0f;
		fPosition = ScreenHeight() - 200;
		fVelocity = 0.0f;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {

		if (bReset) {

			fAcceleration = 0.0f;
			fPosition = ScreenHeight() - 200;
			fVelocity = 0.0f;

			if (counter > bestRecord)
				bestRecord = counter;
			counter = 0;

			bOver = false;
			bReset = false;

		}

		if (!bOver) {

			// CLEAR SCREEN
			Clear(olc::BLUE);

			// DRAWING BACKGROUND
			for (int j = 0; j < vBackgrounds.size(); ++j) {

				DrawDecal({ vBackgrounds[j].second, 0.0f }, vBackgrounds[j].first);
			}

			for (int i = 0; i < vBackgrounds.size(); ++i) {
				if (vBackgrounds[i].second == -298.0f) {
					vBackgrounds[i].second = 894.0f;
				}
				vBackgrounds[i].second -= 0.5f;
			}

			// SEE IF THE KEY WAS PRESSED
			if (GetKey(olc::Key::SPACE).bPressed && fVelocity >= 200.0f / 10.0f && bJumpedOnce) {

				fAcceleration = 0.0f;
				fVelocity = -200.0f;

				counter++;

				bJumpedOnce = false;
				x = 0;
			}
			else {
				fAcceleration += 500.0f*fElapsedTime;
			}

			if (fAcceleration >= 400.0f) fAcceleration = 400.0f;

			fVelocity += fAcceleration*fElapsedTime*1.1;
			fPosition += fVelocity*fElapsedTime;
			fLevelPosition += 14.0f*fElapsedTime;

			if (fPosition >= 480) {
				fPosition = 480;
				bJumpedOnce = true;
			}

			if (fLevelPosition > fSectionWidth) {

				fLevelPosition -= fSectionWidth;
				listSection.pop_front();
				listSection.push_back(20);

			}

			// DRAW BLOCK
			
			DrawDecal({ fBlockPos, 530.0f }, decBlock, { 0.15f, 0.15f });
			DrawDecal({ fBlockPos2, 530.0f }, decBlock2, { 0.15f, 0.15f });
			

			// CALCULATING BLOCK POSITIONS
			fBlockPos -= 0.5f;
			if (fBlockPos == -100.0f)
				fBlockPos = rand() % 250 + 856;
			else if (fBlockPos == 300) {
				fBlockPos2 = 894.0f;
				bSecondBox = true;
			}

			if (bSecondBox)
				fBlockPos2 -= 0.5f;

			vChar.y = fPosition;


			// DRAW OUR DECAL CHARACTER
			DrawDecal(vChar, decDemo, { 0.2f, 0.2f });


			// COLLISION DETECTION
			if ((vChar.x == fBlockPos - 100 || vChar.x - 100 == fBlockPos - 100 || vChar.x - 100 == fBlockPos - 50) && vChar.y > 480.0f - 80 && x != 1) {
				bOver = true;
				x = 1;
			}
			if ((vChar.x == fBlockPos2 - 100 || vChar.x - 100 == fBlockPos2 - 100 || vChar.x - 100 == fBlockPos2 - 50) && vChar.y > 480.0f - 80 && x != 2) {
				bOver = true;
				x = 2;
			}


			// DRAW TEXT
			DrawString({ 8,8 }, "Score: " + to_string(counter), olc::BLACK, 3);
			DrawString({ 10,10 }, "Score: " + to_string(counter), olc::WHITE, 3);
			DrawString({ 8,38 }, "Best score: " + to_string(bestRecord), olc::BLACK, 2);
			DrawString({ 10,40 }, "Best score: " + to_string(bestRecord), olc::WHITE, 2);

			
			return true;
		}
		else {
			
			Clear(olc::WHITE);
			DrawString({80,240},"GAME OVER", olc::BLACK, 10);
			DrawString({210,360}, "PRESS [SPACE] TO PLAY AGAIN", olc::BLACK, 2);

			if (GetKey(olc::Key::SPACE).bPressed) {
				bReset = true;
			}

		}

	}
};