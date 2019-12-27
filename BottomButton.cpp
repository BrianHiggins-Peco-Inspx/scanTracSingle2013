#include "BottomButton.h"


CBottomButton::CBottomButton(void)
{
}


CBottomButton::~CBottomButton(void)
{
}

void CBottomButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	switch (lpDrawItemStruct->itemAction) //:Defines the drawing action required. This will be one or more of the following bits:
	{
		case ODA_DRAWENTIRE:// - This bit is set when the entire control needs to be drawn. 
			break;
		case ODA_FOCUS:// - This bit is set when the control gains or loses input focus. The itemState member should be checked to determine whether the control has focus. 
			break;
		case ODA_SELECT:// - This bit is set when only the selection status has changed. The itemState member should be checked to determine the new selection state
			break;
	}

	switch (lpDrawItemStruct->itemState)
	{
		case ODS_CHECKED:// - This bit is set if the menu item is to be checked. This bit is used only in a menu. 
			break;
		case ODS_DISABLED:// - This bit is set if the item is to be drawn as disabled 
			break;
		case ODS_FOCUS:// - This bit is set if the item has input focus
			break;
		case ODS_GRAYED:// - This bit is set if the item is to be dimmed. This bit is used only in a menu 
			break;
		case ODS_SELECTED:// - This bit is set if the item’s status is selected 
			break;
		case ODS_COMBOBOXEDIT:// - The drawing takes place in the selection field (edit control) of an ownerdrawn combo box
			break;
		case ODS_DEFAULT:// - The item is the default item
			break;
	}

	//LpDrawItemSturct->hDC
	/*
SetWindowRgn(rgn,true);
 // Construct your buttons region. This wont reflect in your view. 
 // But you can sense it by clicking the region area.

 CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
 // Get dc for the button

 switch(lpDrawItemStruct->itemAction) 
 {
    case ODA_SELECT:
    {
    }      // no break; for this case
    case ODA_DRAWENTIRE:
    {
       if(lpDrawItemStruct->itemState & ODS_SELECTED) 
       {
          pDC->FillRgn(CRgn::FromHandle(trgn), 
          CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
       }   
       // Draw button down state
       else
       {
         pDC->FillRgn(CRgn::FromHandle(trgn), 
                      CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
         HRGN r= ::CreateRectRgn(0,0,0,0);
         CombineRgn(r,trgn,0,RGN_COPY);
         OffsetRgn(r,2,2);
         pDC->FillRgn(CRgn::FromHandle(r), 
                      CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
       }
       break;
    }
    case ODA_FOCUS: 
    {
        pDC->FillRgn(CRgn::FromHandle(trgn),
                     CBrush::FromHandle((HBRUSH)GetStockObject(LTGRAY_BRUSH))); 

    }
    break;
  }
  // Draw button caption.
	*/
}

void CBottomButton::PreSubclassWindow()
{
	ModifyStyle(0,BS_OWNERDRAW);
}
