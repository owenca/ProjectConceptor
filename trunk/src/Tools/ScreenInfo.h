//ScreenInfo.h
// Autor: Christian Lörchner

#ifndef _SCREEN_INFO_H
#define _SCREEN_INFO_H

#include <Screen.h>

class ScreenInfo : public BScreen
{
  public:
					ScreenInfo();
			bool 	UpdateInfo();
			int32 	GetX();
			int32 	GetY();
  private:
    display_mode	mode;
    int32			x, y;

};
#endif
