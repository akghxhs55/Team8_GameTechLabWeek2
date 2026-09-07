#include "FEditor.h"

bool FEditor::SelectObject(UObject* Object)
{
    if (Object == nullptr)
        return false;
    SelectedObject = Object;
    return true;
}

void FEditor::UnSelectObject()
{
    SelectedObject = nullptr;
}

UObject* FEditor::GetSelectedObject()
{
    return SelectedObject;
}
