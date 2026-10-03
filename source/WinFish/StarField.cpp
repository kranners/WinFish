#include <SexyAppFramework/DDImage.h>

#include "StarField.h"
#include "WinFishApp.h"
#include "Board.h"
#include "ModConfig.h"

using namespace Sexy;

Sexy::StarField::StarField()
{
	mStarList.clear();
	mMaxStars = 0;
	mNebulaImage = 0;
}

Sexy::StarField::~StarField()
{
	if (mNebulaImage)
		delete mNebulaImage;
}

void Sexy::StarField::Init(int theMaxStars)
{
	mMaxStars = theMaxStars;
	if (mNebulaImage == nullptr)
	{
		// Scaled up like the tank backgrounds so it fills the HD screen without seams.
		Image* aNebulaImage = gSexyApp->GetImage("images/nebula1");
		if (aNebulaImage != nullptr)
		{
			mNebulaImage = ((WinFishApp*)gSexyApp)->ModScaleImage(aNebulaImage, MOD_BG_CROP_Y);
			delete aNebulaImage;
		}
	}

	mStarList.clear();
	if (theMaxStars > 0)
	{
		for (int i = 0; i < theMaxStars; ++i)
		{
			int x = Rand() % MOD_SCREEN_WIDTH;
			int y = Rand() % MOD_SCREEN_HEIGHT;

			AddStar(x, y);
		}
	}
}

void Sexy::StarField::AddStar(int theX, int theY)
{
	mStarList.emplace_back();

	Star& newStar = mStarList.back();

	newStar.mX = theX;
	newStar.mY = theY;
	newStar.mVY = 0;

	int aVal = rand();
	int aVal2 = aVal / 3;
	aVal %= 3;
	if (aVal == 0)
	{
		newStar.mColorValue = 0x404040;
		newStar.mVX = -0.6f;
	}
	else if (aVal == 1)
	{
		newStar.mColorValue = 0x909090;
		newStar.mVX = -1.8f;
	}
	else if (aVal == 2)
	{
		newStar.mColorValue = 0xFFFFFF;
		newStar.mVX = -2.7f;
	}
}

void Sexy::StarField::Update()
{
	StarList::iterator it = mStarList.begin();
	while (it != mStarList.end())
	{
		Star& currentStar = *it;
		currentStar.mX += currentStar.mVX;
		currentStar.mY += currentStar.mVY;

		if (currentStar.mX < 0.0f)
			it = mStarList.erase(it);
		else
			++it;
	}

	for (int i = mStarList.size(); i < mMaxStars; i++)
	{
		AddStar(MOD_SCREEN_WIDTH, Rand() % MOD_SCREEN_HEIGHT);
	}
}

void Sexy::StarField::Draw(Graphics* g, bool flag)
{
	Board* aBoard = ((WinFishApp*)gSexyApp)->mBoard;
	if (!mNebulaImage)
	{
		g->SetColor(Color::Black);
		g->FillRect(0, 0, MOD_SCREEN_WIDTH, MOD_SCREEN_HEIGHT);
	}
	else
	{
		int aImgWdth = mNebulaImage->mWidth;
		int aImgHght = mNebulaImage->mHeight;
		int aX = (aImgWdth - aBoard->mGameUpdateCnt / 2 % aImgWdth)-1;
		for (int aY = 0; aY < MOD_SCREEN_HEIGHT; aY += aImgHght)
		{
			g->DrawImage(mNebulaImage, aX - aImgWdth, aY);
			g->DrawImage(mNebulaImage, aX, aY);
			g->DrawImage(mNebulaImage, aX + aImgWdth, aY);
		}
		if (flag)
		{
			for (int aY = 0; aY < MOD_SCREEN_HEIGHT; aY += aImgHght)
			{
				aBoard->Unk06(g, mNebulaImage, aX - aImgWdth, aY, 8.0);
				aBoard->Unk06(g, mNebulaImage, aX, aY, 8.0);
				aBoard->Unk06(g, mNebulaImage, aX + aImgWdth, aY, 8.0);
			}
		}
	}
	StarList::iterator it;
	for (it = mStarList.begin(); it != mStarList.end(); ++it)
	{
		g->SetColor(Color(it->mColorValue));
		g->FillRect(it->mX, it->mY, 1, 1);
	}
}
