#include "Theme.h"


void AppTheme::SetMainTitleStyle(CStatic* pStatic)
    {
        // Create a new font
        CFont newFont;
        newFont.CreateFont(
            100,                         // Font height (18 points)
            0,                          // Font width (0 for default)
            0,                          // Escapement (0 for default)
            0,                          // Orientation (0 for default)
            FW_BOLD,                    // Weight (bold)
            FALSE,                      // Italic
            FALSE,                      // Underline
            0,                          // Strikeout
            ANSI_CHARSET,               // Character set
            OUT_DEFAULT_PRECIS,         // Output precision
            CLIP_DEFAULT_PRECIS,        // Clipping precision
            DEFAULT_QUALITY,            // Quality
            DEFAULT_PITCH | FF_SWISS,   // Pitch and family
            _T("Times New Roman")                 // Font name
        );

        // Set the new font to the static control
        pStatic->SetFont(&newFont);

        // Redraw the static control to apply changes
        pStatic->RedrawWindow();

    }
