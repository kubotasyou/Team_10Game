#pragma once

//二十解放とかを防ぐ
template <class T>
inline void safedelete(T*& p)
{
	delete p;
	p = nullptr;
}