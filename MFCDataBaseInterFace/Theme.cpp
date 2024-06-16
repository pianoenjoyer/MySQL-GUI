#include "pch.h"
#include "Theme.h"


void AppTheme::SetMainTitleStyle(CStatic* pStatic)
    {
        CFont newFont;
        newFont.CreateFont(
            100,
            0,
            0,
            0,
            FW_BOLD,
            FALSE,
            FALSE,
            0,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS,
            _T("Times New Roman")
        );
        pStatic->SetFont(&newFont);
        pStatic->RedrawWindow();

    }
