#pragma once

//“ñ\‰ğ•ú‚Æ‚©‚ğ–h‚®
template <class T>
inline void safedelete(T*& p)
{
	delete p;
	p = nullptr;
}