#pragma once

//“ñ\‰ğ•ú‚Æ‚©‚ğ–h‚®
template <class T>
inline void safe_delete(T*& p)
{
	delete p;
	p = nullptr;
}