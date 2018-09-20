#ifndef CLCONTROLWITHITEMS_H
#define CLCONTROLWITHITEMS_H

#include "clColours.h"
#include "clHeaderBar.h"
#include "clRowEntry.h"
#include "clScrolledPanel.h"
#include <array>

#ifdef __WXOSX__
#define SCROLL_TICK 2
#else
#define SCROLL_TICK 2
#endif

class clControlWithItems;
class clRowEntry;

// Search flags
#define wxTR_SEARCH_METHOD_EXACT (1 << 0)    // Use an exact string comparison method
#define wxTR_SEARCH_METHOD_CONTAINS (1 << 1) // Compare using wxString::Contains method
#define wxTR_SEARCH_VISIBLE_ITEMS (1 << 2)   // Search reachable items (i.e. they can be visible if scrolled enough)
#define wxTR_SEARCH_ICASE (1 << 3)           // Search incase-sensitive
#define wxTR_SEARCH_INCLUDE_CURRENT_ITEM \
    (1 << 4) // When calling the search API, FindNext/FindPrev include the 'starting' item
#define wxTR_SEARCH_DEFAULT \
    (wxTR_SEARCH_METHOD_CONTAINS | wxTR_SEARCH_VISIBLE_ITEMS | wxTR_SEARCH_ICASE | wxTR_SEARCH_INCLUDE_CURRENT_ITEM)

class WXDLLIMPEXP_SDK clSearchText
{
    wxString m_findWhat;
    bool m_enabled = false;

public:
    void OnKeyDown(const wxKeyEvent& event, clControlWithItems* control);
    void Reset();
    static bool Matches(const wxString& findWhat, size_t col, const wxString& text,
                        size_t searchFlags = wxTR_SEARCH_DEFAULT, clMatchResult* matches = nullptr);

    clSearchText();
    virtual ~clSearchText();
    void SetEnabled(bool enabled) { this->m_enabled = enabled; }
    bool IsEnabled() const { return m_enabled; }
    void Clear();
    const wxString& GetFindWhat() const;
    void SetFindWhat(const wxString& findWhat) { this->m_findWhat = findWhat; }
};

class WXDLLIMPEXP_SDK clControlWithItems : public clScrolledPanel
{
private:
    clHeaderBar m_viewHeader;
    clColours m_colours;
    clRowEntry* m_firstItemOnScreen = nullptr;
    int m_firstColumn = 0;
    int m_lineHeight = 0;
    int m_indent = 0;
    std::vector<wxBitmap> m_bitmaps;
    int m_scrollTick = SCROLL_TICK;
    clSearchText m_search;

protected:
    void DoInitialize();
    int GetNumLineCanFitOnScreen() const;
    virtual clRowEntry* GetFirstItemOnScreen();
    virtual void SetFirstItemOnScreen(clRowEntry* item);
    void RenderHeader(wxDC& dc);
    void RenderItems(wxDC& dc, const clRowEntry::Vec_t& items);
    void OnSize(wxSizeEvent& event);
    void DoUpdateHeader(clRowEntry* row);
    wxSize GetTextSize(const wxString& label) const;
    virtual void OnMouseScroll(wxMouseEvent& event);
    virtual bool DoKeyDown(const wxKeyEvent& event);
    clSearchText& GetSearch() { return m_search; }
    const clSearchText& GetSearch() const { return m_search; }

public:
    clControlWithItems(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize, long style = 0);
    virtual ~clControlWithItems();
    clControlWithItems();

    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0);
    void SetFindWhat(const wxString& what);
    const wxString& GetFindWhat() const;
    void ClearFindWhat();
    virtual int GetIndent() const { return m_indent; }

    virtual void SetFirstColumn(int firstColumn) { this->m_firstColumn = firstColumn; }
    virtual int GetFirstColumn() const { return m_firstColumn; }

    virtual void SetLineHeight(int lineHeight) { this->m_lineHeight = lineHeight; }
    virtual int GetLineHeight() const { return m_lineHeight; }

    virtual void SetBitmaps(const std::vector<wxBitmap>& bitmaps) { this->m_bitmaps = bitmaps; }
    virtual const std::vector<wxBitmap>& GetBitmaps() const { return m_bitmaps; }
    virtual std::vector<wxBitmap>& GetBitmaps() { return m_bitmaps; }

    void SetScrollTick(int scrollTick) { this->m_scrollTick = scrollTick; }
    int GetScrollTick() const { return m_scrollTick; }

    /**
     * @brief use native header drawings
     */
    void SetNativeHeader(bool b);

    /**
     * @brief return bitmap at a given index
     */
    const wxBitmap& GetBitmap(size_t index) const;

    /**
     * @brief set the item's indent size
     */
    virtual void SetIndent(int size) { m_indent = size; }

    /**
     * @brief return the items rect area, excluding header
     */
    wxRect GetItemsRect() const;

    /**
     * @brief draw the header + items
     */
    void Render(wxDC& dc);

    /**
     * @brief return the header bar (relevant when using columns)
     */
    const clHeaderBar& GetHeader() const { return m_viewHeader; }
    /**
     * @brief return the header bar (relevant when using columns)
     */
    clHeaderBar& GetHeader() { return m_viewHeader; }

    /**
     * @param header
     */
    void SetHeader(const clHeaderBar& header);

    /**
     * @brief should we show the header bar?
     */
    void SetShowHeader(bool b);
    /**
     * @brief is the heaer bar visible?
     */
    bool IsHeaderVisible() const;

    /**
     * @brief update the scrollbar with the current view status
     * subclass should call this method whenver the view changes (re-sized, items are expanding, collapsing etc)
     */
    void UpdateScrollBar();

    void SetColours(const clColours& colours)
    {
        this->m_colours = colours;
        Refresh();
    }

    const clColours& GetColours() const { return m_colours; }
    clColours& GetColours() { return m_colours; }

    // Horizontal scrolling implementation
    void ScollToColumn(int firstColumn);
    void ScrollColumns(int steps, wxDirection direction);

    //===-----------------------------------------
    //===-----------------------------------------

    /**
     * @brief override this
     */
    virtual bool IsEmpty() const = 0;
    /**
     * @brief return the total of numbers of items we can scroll
     * If the view has collpased items, the range _excludes_ them
     */
    virtual int GetRange() const = 0;

    /**
     * @brief return the row number of the first visible item in the view
     */
    virtual int GetFirstItemPosition() const = 0;
};

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_TREE_SEARCH_TEXT, wxTreeEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_TREE_CLEAR_SEARCH, wxTreeEvent);

#endif // CLCONTROLWITHITEMS_H