// hack.  If god had meant programmers to mess with the menu bar, he'd have // provided a menu manager routine to accomplish this.//// Why is this code so skanky?// � it relies on static variable// � it relies on the complier auto initializing new variables to nil//   to ensure that the regions get corrected properly// � it relies on manipulating low memory globals to which there may be no//   access in future// � it doesn't really work too well (try a context switch, the grayrgn//   needs fixing up when you switch back)//#include <LowMem.h>#include <Windows.h>#include "tumbler_hidemenubar.h"static Boolean 		pMbarIsVisible = true ;static short 		pOldMBarHeight = 0 ; 		// saves the menu bar heightstatic RgnHandle	pOldGrayRgn = nil;static RgnHandle	pMenuRgn = nil ;static Rect			pMenuRect ;static RgnHandle	pCopyGrayRgn ;void ToggleMenuBar(void){	WindowRef			frontWindow ;		if((frontWindow = (WindowRef)FrontWindow()) == nil)		return ;		if( pOldGrayRgn == nil ) {		pOldGrayRgn = NewRgn() ;	}		if( pMenuRgn == nil ) {		pMenuRgn = NewRgn() ;	}			if( pOldMBarHeight == 0 ) {		pOldMBarHeight = LMGetMBarHeight() ;	}		if( pMbarIsVisible ) {		// hide the menubar				// store the grayrgn		CopyRgn(GetGrayRgn(), pOldGrayRgn);		// reset the menu bar height		LMSetMBarHeight(0) ;		// save the menu rect		pMenuRect = qd.screenBits.bounds;		pMenuRect.bottom = pOldMBarHeight;		RectRgn(pMenuRgn, &pMenuRect);		// and add this to the global grayrgn		pCopyGrayRgn = GetGrayRgn() ;		UnionRgn(pCopyGrayRgn, pMenuRgn, pCopyGrayRgn);		LMSetGrayRgn( pCopyGrayRgn ) ;				PaintBehind( frontWindow, pMenuRgn);		CalcVisBehind( frontWindow, pMenuRgn);	}	else {		// Show the menubar				// reset the menu bar height		LMSetMBarHeight( pOldMBarHeight );				// update the grayrgn		XorRgn(pCopyGrayRgn,pMenuRgn,pCopyGrayRgn) ;		LMSetGrayRgn( pCopyGrayRgn ) ;				// calc the rect occupied by the menu bar		pMenuRect = qd.screenBits.bounds;		pMenuRect.bottom = pOldMBarHeight;		RectRgn(pOldGrayRgn, &pMenuRect);		PaintBehind( frontWindow, pCopyGrayRgn);		CalcVisBehind( frontWindow, pCopyGrayRgn);		// draw the menubar in		DrawMenuBar();	}	pMbarIsVisible = !pMbarIsVisible ;		return ;}void FixGrayRgnAfterContextSwitch( void ) {	WindowRef			frontWindow ;		if((frontWindow = (WindowRef)FrontWindow()) == nil)		return ;	if(!pMbarIsVisible) {		// store the grayrgn		CopyRgn(GetGrayRgn(), pOldGrayRgn);		// reset the menu bar height		LMSetMBarHeight(0) ;		// save the menu rect		pMenuRect = qd.screenBits.bounds;		pMenuRect.bottom = pOldMBarHeight;		RectRgn(pMenuRgn, &pMenuRect);		// and add this to the global grayrgn		pCopyGrayRgn = GetGrayRgn() ;		UnionRgn(pCopyGrayRgn, pMenuRgn, pCopyGrayRgn);		LMSetGrayRgn( pCopyGrayRgn ) ;				PaintBehind( frontWindow, pMenuRgn);		CalcVisBehind( frontWindow, pMenuRgn);		}}