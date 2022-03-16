#ifndef EDITOR_UI
#define EDITOR_UI

struct EDITOR_UI_SETTINGS
{
    int shadows{1};
    int bloom{1};
    int AO{1};
    int AOSamples{32};
    float AORadius{1.0f};
    int tone_mapping{1};
    int volumetrics{0};
    int motion_blur{1};
    int show_physics{0};
};

#endif
