#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define UI_LINUX
#define UI_IMPLEMENTATION
#include "luigi.h"

typedef struct {
    uint8_t r, g, b, a;
} RGBA;

RGBA hsv_to_rgba(float hue, float saturation, float value) {
    RGBA result = { .a = 255 };

    // Red channel
    float k = fmodf((5.0f + hue/60.0f), 6);
    float t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    result.r = (unsigned char)((value - value*saturation*k)*255.0f);

    // Green channel
    k = fmodf((3.0f + hue/60.0f), 6);
    t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    result.g = (unsigned char)((value - value*saturation*k)*255.0f);

    // Blue channel
    k = fmodf((1.0f + hue/60.0f), 6);
    t = 4.0f - k;
    k = (t < k)? t : k;
    k = (k < 1)? k : 1;
    k = (k > 0)? k : 0;
    result.b = (unsigned char)((value - value*saturation*k)*255.0f);

    return result;
}

UIColorPicker* color_picker = NULL;
UITextbox* rgba_textbox = NULL;
UITextbox* hex_textbox = NULL;
UITextbox* cstyle_textbox = NULL;
UITextbox* hsv_textbox = NULL;

int color_picker_message(UIElement* element, UIMessage message, int di, void* dp) {
    switch (message) {
        case UI_MSG_VALUE_CHANGED: {
            int hue = color_picker->hue / 6.28 * 360;
            RGBA color = hsv_to_rgba(hue, color_picker->saturation, color_picker->value);

            char buf[1024] = {0};
            int len = snprintf(buf, 1024, "rgba(%d, %d, %d, %d)", color.r, color.g, color.b, color.a);
            rgba_textbox->carets[0] = 0;
            UITextboxReplace(rgba_textbox, buf, len, false);

            len = sprintf(buf, "#%02X%02X%02X%02X", color.r, color.b, color.g, color.a);
            hex_textbox->carets[0] = 0;
            UITextboxReplace(hex_textbox, buf, len, false);

            len = sprintf(buf, "0x%02X%02X%02X%02X", color.r, color.b, color.g, color.a);
            cstyle_textbox->carets[0] = 0;
            UITextboxReplace(cstyle_textbox, buf, len, false);

            len = sprintf(buf, "hsv(%d, %.2f, %.2f)", hue, color_picker->saturation, color_picker->value);
            hsv_textbox->carets[0] = 0;
            UITextboxReplace(hsv_textbox, buf, len, false);

            UIElementRefresh(&rgba_textbox->e);
            UIElementRefresh(&hex_textbox->e);
            UIElementRefresh(&cstyle_textbox->e);
            UIElementRefresh(&hsv_textbox->e);
        } break;
        default:
            break;
    }
    return 0;
}

int main(void) {
	UIInitialise(); 
	
	UIWindow* window = UIWindowCreate(NULL, 0, "My First Application", 640, 480);

    UIPanel* panel = UIPanelCreate(&window->e, UI_PANEL_GRAY | UI_PANEL_MEDIUM_SPACING);

    color_picker = UIColorPickerCreate(&panel->e, 0);
    color_picker->e.messageUser = color_picker_message;

    UISpacer* spacer1 = UISpacerCreate(&panel->e, 0, 0, 100);

    UIPanel* rgba_panel = UIPanelCreate(&panel->e, UI_PANEL_GRAY | UI_PANEL_MEDIUM_SPACING | UI_PANEL_HORIZONTAL);
    UILabel* rgba_label = UILabelCreate(&rgba_panel->e, 0, "RGBA: ", -1);
    rgba_textbox = UITextboxCreate(&rgba_panel->e, 0);

    UIPanel* hex_panel = UIPanelCreate(&panel->e, UI_PANEL_GRAY | UI_PANEL_MEDIUM_SPACING | UI_PANEL_HORIZONTAL);
    UILabel* hex_label = UILabelCreate(&hex_panel->e, 0, "HEX: ", -1);
    hex_textbox = UITextboxCreate(&hex_panel->e, 0);

    UIPanel* cstyle_panel = UIPanelCreate(&panel->e, UI_PANEL_GRAY | UI_PANEL_MEDIUM_SPACING | UI_PANEL_HORIZONTAL);
    UILabel* cstyle_label = UILabelCreate(&cstyle_panel->e, 0, "C-style HEX literal: ", -1);
    cstyle_textbox = UITextboxCreate(&cstyle_panel->e, 0);

    UIPanel* hsv_panel = UIPanelCreate(&panel->e, UI_PANEL_GRAY | UI_PANEL_MEDIUM_SPACING | UI_PANEL_HORIZONTAL);
    UILabel* hsv_label = UILabelCreate(&hsv_panel->e, 0, "HSV: ", -1);
    hsv_textbox = UITextboxCreate(&hsv_panel->e, 0);

	return UIMessageLoop();
}
