#pragma once

//��\����Ƃ���h��
template <class T>
inline void safedelete(T*& p)
{
	delete p;
	p = nullptr;
}