/*
 * dvbapi.h: Interface to the DVB driver
 *
 * See the main source file 'vdr.c' for copyright information and
 * how to reach the author.
 *
 * $Id: dvbapi.h 1.12 2000/07/30 15:01:01 kls Exp $
 */

#ifndef __DVBAPI_H
#define __DVBAPI_H

// FIXME: these should be defined in ../DVB/driver/dvb.h!!!
typedef unsigned int __u32;
typedef unsigned short __u16;
typedef unsigned char __u8;

#if defined(DEBUG_OSD) || defined(REMOTE_KBD)
#include <ncurses.h>
#endif
#include <stdio.h>
#include "../DVB/driver/dvb.h"

#define MenuLines   15
#define MenuColumns 40

enum eDvbColor { clrBackground,
#ifndef DEBUG_OSD
                 clrOBSOLETE, //FIXME apparently color '1' can't be used as FgColor with e.g. clrRed as BgColor???
                 clrBlack,
#else
                 clrBlack = clrBackground,
#endif
                 clrRed,
                 clrGreen,
                 clrYellow,
                 clrBlue,
                 clrMagenta,
                 clrCyan,
                 clrWhite,
               };
                  
class cDvbApi {
private:
  int videoDev;
  cDvbApi(const char *FileName);
public:
  ~cDvbApi();

#define MAXDVBAPI 2
  static int NumDvbApis;
private:
  static cDvbApi *dvbApi[MAXDVBAPI];
public:
  static cDvbApi *PrimaryDvbApi;
  static cDvbApi *GetDvbApi(int Ca = 0);
         // Selects a free DVB device, starting with the highest device number.
         // If Ca is nor 0, the device with the given number will be returned
         // if it is not currently recording.
  int Index(void);
         // Returns the index of this DvbApi.
  static bool Init(void);
         // Initializes the DVB API and probes for existing DVB devices.
         // Must be called before accessing any DVB functions.
  static void Cleanup(void);
         // Closes down all DVB devices.
         // Must be called at the end of the program.

  // On Screen Display facilities

private:
  enum { charWidth  = 12, // average character width
         lineHeight = 27  // smallest text height
       };
#ifdef DEBUG_OSD
  WINDOW *window;
  enum { MaxColorPairs = 16 };
  int colorPairs[MaxColorPairs];
  void SetColor(eDvbColor colorFg, eDvbColor colorBg = clrBackground);
#endif
  int cols, rows;
  void Cmd(OSD_Command cmd, int color = 0, int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0, const void *data = NULL);
public:
  void Open(int w, int h);
  void Close(void);
  void Clear(void);
  void Fill(int x, int y, int w, int h, eDvbColor color = clrBackground);
  void ClrEol(int x, int y, eDvbColor color = clrBackground);
  void Text(int x, int y, const char *s, eDvbColor colorFg = clrWhite, eDvbColor colorBg = clrBackground);

  // Progress Display facilities

private:
  int lastProgress, lastTotal;
  char *replayTitle;
public:
  bool ShowProgress(bool Initial = false);

  // Channel facilities

  bool SetChannel(int FrequencyMHz, char Polarization, int Diseqc, int Srate, int Vpid, int Apid, int Ca, int Pnr);

  // Record/Replay facilities

private:
  enum { dvbStop = 1, // let's not have 0 as a command
         dvbPause,
         dvbPlay,
         dvbForward,
         dvbBackward,
         dvbSkip,
         dvbGetIndex,
       };
  pid_t pidRecord, pidReplay;
  int fromRecord, toRecord;
  int fromReplay, toReplay;
  void SetReplayMode(int Mode);
public:
  bool Recording(void);
       // Returns true if we are currently recording.
  bool Replaying(void);
       // Returns true if we are currently replaying.
  bool StartRecord(const char *FileName);
       // Starts recording the current channel into the given file.
       // In order to be able to record longer movies,
       // a numerical suffix will be appended to the file name. The inital
       // value of that suffix will be larger than any existing file under
       // the given name, thus allowing an interrupted recording to continue
       // gracefully.
       // Returns true if recording was started successfully.
       // If there is already a recording session active, false will be
       // returned.
  void StopRecord(void);
       // Stops the current recording session (if any).
  bool StartReplay(const char *FileName, const char *Title = NULL);
       // Starts replaying the given file.
       // If there is already a replay session active, it will be stopped
       // and the new file will be played back.
       // If provided Title will be used in the progress display.
  void Stop(void);
       // Stops the current replay session (if any).
  void Pause(void);
       // Pauses the current replay session, or resumes a paused session.
  void Play(void);
       // Resumes normal replay mode.
  void Forward(void);
       // Runs the current replay session forward at a higher speed.
  void Backward(void);
       // Runs the current replay session backwards at a higher speed.
  void Skip(int Seconds);
       // Skips the given number of seconds in the current replay session.
       // The sign of 'Seconds' determines the direction in which to skip.
       // Use a very large negative value to go all the way back to the
       // beginning of the recording.
  bool GetIndex(int *Current, int *Total = NULL);
  };

#endif //__DVBAPI_H
