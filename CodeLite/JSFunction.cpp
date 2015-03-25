#include "JSFunction.h"
#include "file_logger.h"
#include <algorithm>
#include <wx/crt.h>
#include "JSLookUpTable.h"

JSFunction::JSFunction(const JSLookUpTable* lookup)
    : JSObject(lookup)
{
}

JSFunction::~JSFunction() {}

void JSFunction::Print(int depth)
{
    wxString indent(' ', (size_t)depth);
    CL_DEBUG("%s{[Function] Name: \"%s\", Path: \"%s\", Signature: \"%s\"\n", indent, m_name, m_path, m_signature);
    if(!m_properties.empty()) {
        std::for_each(m_properties.begin(), m_properties.end(), [&](const std::pair<wxString, JSObject::Ptr_t>& p) {
            (p.second)->Print(depth + 2);
        });
    }
}

void JSFunction::AddVariable(JSObject::Ptr_t var)
{
    JSObject::Map_t::iterator iter = m_variables.find(var->GetName());
    if(iter != m_variables.end()) m_variables.erase(iter);

    m_variables.insert(std::make_pair(var->GetName(), var));
}

JSObject::Ptr_t JSFunction::NewInstance(const wxString& name, const JSLookUpTable* lookup)
{
    JSObject::Ptr_t inst = lookup->NewFunction();
    inst->SetName(name);
    inst->SetType(GetType());
    inst->SetPath(GetPath());
    return inst;
}