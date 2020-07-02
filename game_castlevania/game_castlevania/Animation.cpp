#include "Animation.h"

void Animation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = Sprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void Animation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}

	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void Animation::RenderFrame(float x, float y, int frame, int alpha)
{
	if (frame != -1)
		frames[frame]->GetSprite()->Draw(x, y, alpha);
}

int Animation::GetcurrenFrame()
{
	return currentFrame;
}

void Animation::SetCurrenFrame(int currenframe)
{
	currentFrame = currenframe;
}

int Animation::size()
{
	return frames.size();
}
