#pragma once

//二十解放とかを防ぐ
template <class T>
inline void safe_delete(T*& p)
{
	delete p;
	p = nullptr;
}