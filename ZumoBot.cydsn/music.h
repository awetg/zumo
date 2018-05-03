/* ========================================
 *void PlayPDM(uint16* track, int size);
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef MUSIC_H
#define MUSIC_H
    
#include <stdbool.h>
#include <stdint.h>
#include <project.h>
    
typedef struct note {
    char note;
    int octave;
    int duration;
    bool legato;
} Note;
    
//Play a single note
void play_note(Note note);
//Start playing a note asyncronously
void play_note_async(Note note);

//Play PDM sounds from a buffer (max size = 255)
void PlayBufferPDM(uint16* buffer, uint8 size);
//Play a full PDM track
void PlayPDM(uint16* track, int size);

//Play music asyncronously. Must be called in every step if the main cycle.
//The music to play must be set with set_music_async, and stopped with stop_music_async
void play_music_async();
void set_music_async(char* music, float base_duration);
void stop_music_async();

//Play a full track, pausing the execution for its duration
void play_music(char* music, float base_duration);
void play_music_with_base(char* music, char* baseline, float base_duration);

/*
To write the track:
Notes: a A b B C d D e E F g G, where the lower caps one are flat notes. There are no sharps.
Silence: S
Octave: Select the octave by using numbers -> 0 = first octave, 1 = second octave, ...
Length modifiers: = (semiquaver), - (quaver), default or space (crotchet), o (minim), O (semibreve)
The length modifier apply to multiple notes or silences, but get reset to default (crotchet) at every space
Legato: L (only in synchronous playing)


Example, Fur Elise: "3 E =EeEeEBDC A =S 2 =CE 3 =A B =S 2 =E 3 =aB C 2 E"
*/


#endif
/* [] END OF FILE */
