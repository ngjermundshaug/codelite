#ifndef WXTERMINALCOLOURHANDLER_H
#define WXTERMINALCOLOURHANDLER_H

#include <wx/textctrl.h>
#include <unordered_map>
#include <map>
#include <vector>

enum class eColourHandlerState {
    kNormal = 0,
    kInEscape, // found ESC char
    kInOsc,    // Operating System Command
    kInCsi,    // Control Sequence Introducer
    kFoundCR,  // Found CR
};

class wxTerminalColourHandler
{
    wxTextCtrl* m_ctrl = nullptr;
    wxString m_escapeSequence;
    std::unordered_map<int, wxColour> m_colours;
    wxTextAttr m_defaultAttr;

protected:
    eColourHandlerState m_state = eColourHandlerState::kNormal;
    wxColour GetColour(long colour_number);

protected:
    void Append(const wxString& buffer);
    void SetStyleFromEscape(const wxString& escape);
    void Clear();
    void FlushBuffer(std::vector<std::string>& buf, std::string& incompleteLine);
    
public:
    wxTerminalColourHandler();
    wxTerminalColourHandler(wxTextCtrl* ctrl);
    ~wxTerminalColourHandler();

    wxTerminalColourHandler& operator<<(const wxString& buffer);
    void SetCtrl(wxTextCtrl* ctrl);
};

#endif // WXTERMINALCOLOURHANDLER_H
