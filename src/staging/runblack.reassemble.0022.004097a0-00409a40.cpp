#include "SetupButton.h"
#include "SetupSlider.h"
#include "SetupControl.h"
#include "SetupThing.h"
#include "SetupBox.h"
#include "GatheringText.h"

#define CLAMP(v, min, max) (v > min ? v < max ? v : max : min)

static LH3DColor dword_C4CCD8;
static LH3DColor dword_C4CCE4;
static LH3DColor dword_C4CCF8;

SetupButton::SetupButton() {

}

// win1.41 004097a0 mac 100c7c90 SetupButton::Draw(bool, bool)
void SetupButton::Draw(bool hovered, bool selected)
{
    SetupThing::DrawBevBox(rect.p0.x, rect.p0.y, rect.p1.x, rect.p1.y, (hovered ? 1 : 0) + 1, 16, -1, 0xFFFFFFFF);

    int count;
    if (text_size == 0) {
        if (setup_box) {
            count = setup_box->default_text_size;
        } else {
            count = 10;
        }
    } else {
        count = text_size;
    }

    while (count > 10) {
        float width = SetupThing::GetTextWidth(label, count, 0, 1.0f);
        if (width <= rect.p1.x - rect.p0.x) {
            break;
        }
        count--;
    }

    LH3DColor* colour;
    if (hovered) {
        colour = &dword_C4CCE4;
    } else if (selected) {
        colour = &dword_C4CCF8;
    } else {
        colour = &dword_C4CCD8;
    }

    SetupThing::DrawText((rect.p0.x + rect.p1.x) / 2 + pressed * 2, 
                         (rect.p1.y + rect.p0.y) / 2 - count / 2 + pressed * 2, 
                         rect.p1.x - rect.p0.x, 
                         TEXTJUSTIFY_CENTRE, 
                         label, 
                         count, 
                         colour,
                         0);
}

// win1.41 004098b0 mac 103dd710 SetupButton::SetupButton(int, int, int, int, int, wchar_t *, int)
SetupButton::SetupButton(int id, int x, int y, int width, int height, const char16_t* label, int param_8) : 
    SetupControl(id, x, y, width, height, label) {
    pressed = 0;
    field_0x240 = param_8;
}

// win1.41 00409900 mac 101104d0 SetupButton::MouseDown(int, int, bool)
void SetupButton::MouseDown(int x, int y, bool param_3)
{
    pressed = 1;
}

// win1.41 00409910 mac 10172660 SetupButton::MouseUp(int, int, bool)
void SetupButton::MouseUp(int x, int y, bool param_3)
{
    pressed = 0;
}

// win1.41 00409920 mac 1034a2d0 SetupButton::KeyDown(int, int)
void SetupButton::KeyDown(LHKey key, LHKeyMod mod)
{
    if (setup_box) {
        setup_box->field_0x0(key, mod);
    }
}

// win1.41 00409940 mac 10594240 SetupButton::~SetupButton(void)
SetupButton::~SetupButton()
{
}

// win1.41 00409960 mac 10103d10 SetupSlider::KeyDown(int, int)
void SetupSlider::KeyDown(LHKey key, LHKeyMod mod)
{
    bool r6 = false;

    switch (key) {
        case LHKEY_HOME:
            value = 0.0f;
            r6 = true;
            break;
        case LHKEY_END:
            value = 1.0f;
            r6 = true;
            break;
        case LHKEY_LEFT:
            value -= 0.1f;
            r6 = true;
            break;
        case LHKEY_RIGHT:
            value += 0.1f;
            r6 = true;
            break;
    }

    value = CLAMP(value, 0.0f, 1.0f);
    drag_start_value = value;

    if (r6) {
        if (setup_box->field_0xb0) {
            setup_box->field_0xb0(4, setup_box, this, 0, 0);
        }
    }
}
