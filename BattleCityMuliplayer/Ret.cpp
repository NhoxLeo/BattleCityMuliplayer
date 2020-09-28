#include "Ret.h"

Size::Size(int h, int w)
{
	height = h;
	width = w;
}

bool Ret::Collision(Ret ret)
{
	RECT rect1;
	rect1.left = ret.getPosition().x - ret.getSize().getwidth()/2;
	rect1.top = ret.getPosition().y - ret.getSize().getHeight() / 2;
	rect1.right = ret.getPosition().x + ret.getSize().getwidth()/2;
	rect1.bottom = ret.getPosition().y + ret.getSize().getHeight()/2;

	RECT rect2;
	rect2.left = this->getPosition().x - this->getSize().getwidth()/2;
	rect2.top = this->getPosition().y - this->getSize().getHeight()/2;
	rect2.right = this->getPosition().x + this->getSize().getwidth()/2;
	rect2.bottom = this->getPosition().y + this->getSize().getHeight()/2;

	RECT rect;

	return IntersectRect(&rect, &rect1, &rect2);
}

bool Ret::Collision(int counter,D3DXVECTOR3 direction,Ret ret)
{
	bool x;
	for (int i = 0; i < counter;i++)
	{
		RECT rect1;
		rect1.left = ret.getPosition().x - ret.getSize().getwidth() / 2;
		rect1.top = ret.getPosition().y - ret.getSize().getHeight() / 2;
		rect1.right = ret.getPosition().x + ret.getSize().getwidth() / 2;
		rect1.bottom = ret.getPosition().y + ret.getSize().getHeight() / 2;

		RECT rect2;
		rect2.left = this->getPosition().x + direction.x*i - this->getSize().getwidth() / 2;
		rect2.top = this->getPosition().y + direction.y*i - this->getSize().getHeight() / 2;
		rect2.right = this->getPosition().x + direction.x*i + this->getSize().getwidth() / 2;
		rect2.bottom = this->getPosition().y + direction.y*i + this->getSize().getHeight() / 2;

		RECT rect;

		x = IntersectRect(&rect, &rect1, &rect2);
		if (x)
		{
			return x;
		}
	}
	return false;
}

D3DXVECTOR3 Ret::getPosition()
{
	return r_position;
}

Size Ret::getSize()
{
	return r_size;
}

void Ret::setHeight(int h)
{
	r_size.setHeight(h);
}

void Ret::setwidth(int w)
{
	r_size.setWeight(w);
}

void Ret::setPosition(D3DXVECTOR3 position)
{
	r_position = position;
}

Ret::Ret(Size size, D3DXVECTOR3 position)
{
	r_size = size;
	r_position = position;
}