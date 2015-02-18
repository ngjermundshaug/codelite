#ifndef WXCODECOMPLETIONBOX_H
#define WXCODECOMPLETIONBOX_H

#include "wxCodeCompletionBoxBase.h"
#include "wxCodeCompletionBoxEntry.h"
#include <wx/arrstr.h>
#include <wx/sharedptr.h>
#include <vector>
#include <list>
#include <wx/bitmap.h>
#include <wx/stc/stc.h>
#include <wx/font.h>
#include "entry.h"
#include <wx/event.h>
#include <wx/bitmap.h>

class CCBoxTipWindow;
class wxCodeCompletionBox;
class wxCodeCompletionBoxManager;

class WXDLLIMPEXP_SDK wxCodeCompletionBox : public wxCodeCompletionBoxBase
{
public:
    typedef std::vector<wxBitmap> BmpVec_t;
    friend class wxCodeCompletionBoxManager;

public:
    enum eOptions {
        kNone = 0,
        kInsertSingleMatch = (1 << 0),
        kRefreshOnKeyType = (1 << 1),
    };

protected:
    wxCodeCompletionBoxEntry::Vec_t m_allEntries;
    wxCodeCompletionBoxEntry::Vec_t m_entries;
    wxCodeCompletionBox::BmpVec_t m_bitmaps;

    int m_index;
    wxString m_displayedTip;
    wxStyledTextCtrl* m_stc;
    wxFont m_ccFont;
    int m_startPos;

    /// When firing the various "clCodeCompletionEvent"s, set the event object
    /// to this member. This help distinguish the real trigger of the code completion
    /// box
    wxEvtHandler* m_eventObject;

    /// Code completion tooltip that is shown next to the code completion box
    CCBoxTipWindow* m_tipWindow;

    /// The code completion box flags, see above enum for possible values
    size_t m_flags;

    /// Contains the scrollbar rectangle
    wxRect m_scrollArea;
    wxRect m_scrollTopRect;
    wxRect m_scrollBottomRect;

    /// Colours used by this class
    wxColour m_lightBorder;
    wxColour m_darkBorder;
    wxColour m_bgColour;
    wxColour m_textColour;
    wxColour m_selection;

    /// Scrollbar bitmaps
    wxBitmap m_bmpUp;
    wxBitmap m_bmpDown;
    wxBitmap m_bmpUpEnabled;
    wxBitmap m_bmpDownEnabled;

protected:
    void StcKeyDown(wxKeyEvent& event);
    void StcLeftDown(wxMouseEvent& event);
    void StcModified(wxStyledTextEvent& event);
    void StcCharAdded(wxStyledTextEvent& event);

    // Event handlers
    void OnLeftDClick(wxMouseEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnAppActivate(wxActivateEvent& event);
    void OnDismissBox(wxCommandEvent& event);

public:
    virtual ~wxCodeCompletionBox();
    /**
     * @brief construct a code completion box
     * @param parent the parent window
     * @param eventObject will be passed as the eventObject of the various clCodeCompletionEvent
     * events fired from this object
     */
    wxCodeCompletionBox(wxWindow* parent, wxEvtHandler* eventObject = NULL, size_t flags = 0);

    /**
     * @brief show the completion box
     */
    void ShowCompletionBox(wxStyledTextCtrl* ctrl, const wxCodeCompletionBoxEntry::Vec_t& entries);

    /**
     * @brief show the completion box
     */
    void ShowCompletionBox(wxStyledTextCtrl* ctrl, const TagEntryPtrVector_t& tags);

    void SetBitmaps(const wxCodeCompletionBox::BmpVec_t& bitmaps) { this->m_bitmaps = bitmaps; }
    const wxCodeCompletionBox::BmpVec_t& GetBitmaps() const { return m_bitmaps; }

    /**
     * @brief convert TagEntryPtr into Code Completion entry
     */
    static wxCodeCompletionBoxEntry::Ptr_t TagToEntry(TagEntryPtr tag);
    
    /// accessors
    void SetFlags(size_t flags) { this->m_flags = flags; }
    size_t GetFlags() const { return m_flags; }

protected:
    int GetSingleLineHeight() const;
    void FilterResults();
    void HideAndInsertSelection();

    // For backward compatability, we support initializing the list with TagEntryPtrVector_t
    // These 2 functions provide conversion between wxCodeCompletionBoxEntry and TagEntryPtr
    wxCodeCompletionBoxEntry::Vec_t TagsToEntries(const TagEntryPtrVector_t& tags);
    static int GetImageId(TagEntryPtr entry);
    void DoDisplayTipWindow();
    void DoUpdateList();
    void DoScrollDown();
    void DoScrollUp();
    void DoDestroy();
    void DoDrawBottomScrollButton(wxDC& dc);
    void DoDrawTopScrollButton(wxDC& dc);
    bool CanScrollDown();
    bool CanScrollUp();
};
#endif // WXCODECOMPLETIONBOX_H