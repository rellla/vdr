/*
 * menu.h: The actual menu implementations
 *
 * See the main source file 'vdr.c' for copyright information and
 * how to reach the author.
 *
 * $Id: menu.h 1.6 2000/04/29 15:38:39 kls Exp $
 */

#ifndef _MENU_H
#define _MENU_H

#include "osd.h"

class cMenuMain : public cOsdMenu {
public:
  cMenuMain(void);
  virtual eOSState ProcessKey(eKeys Key);
  };
  
class cReplayControl : public cOsdBase {
private:
  bool visible, shown;
  void Show(void);
  void Hide(void);
  static char *fileName;
  static char *title;
public:
  cReplayControl(void);
  virtual ~cReplayControl();
  virtual eOSState ProcessKey(eKeys Key);
  bool Visible(void) { return visible; }
  static void SetRecording(const char *FileName, const char *Title);
  };

#endif //_MENU_H
